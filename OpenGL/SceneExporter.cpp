#include "SceneExporter.h"
#include <vector>

void SceneExporter::Export(ApplicationContext * context)
{
	std::string exportFilePath = context->ApplicationFileManager()->ExportFilePath() + context->ApplicationScene()->SceneName() + ".dae";
	aiScene exportScene;

	exportScene.mRootNode = new aiNode();

	exportScene.mMaterials = new aiMaterial*[1];
	exportScene.mNumMaterials = 1;
	exportScene.mMaterials[0] = new aiMaterial();

	exportScene.mMeshes = new aiMesh*[context->ApplicationRenderer()->ModelCount()];
	exportScene.mMeshes[0] = nullptr;
	exportScene.mNumMeshes = context->ApplicationRenderer()->ModelCount();

	// Load mesh data
	int meshIndex = 0;
	for (auto &modelTuple : context->ApplicationRenderer()->Models())
	{
		Model* model = modelTuple.second;
		for (int colorIndex = 0; colorIndex < model->UVIndexCount(); colorIndex++)
		{
			Mesh* mesh = model->GetMesh(colorIndex);

			exportScene.mMeshes[meshIndex] = new aiMesh();
			this->LoadMeshData(&exportScene, mesh, meshIndex);
			meshIndexes[modelTuple.first][colorIndex] = meshIndex;
			meshIndex++;
		}
	}

	// Add all entities as children to the root node
	int exportEntityCount = 0;
	for (auto &e : context->ApplicationScene()->Entities())
	{
		if (e->HasProperty("Block"))
		{
			exportEntityCount++;
		}
	}

	aiNode** children = new aiNode*[exportEntityCount];
	int entityIndex = 0;
	for (auto &e : context->ApplicationScene()->Entities())
	{
		if (!e->HasProperty("Block"))
		{
			continue;
		}

		children[entityIndex] = new aiNode();
		children[entityIndex]->mMeshes = new unsigned int[1];
		children[entityIndex]->mMeshes[0] = meshIndexes[e->MeshName()][e->MeshColorIndex()];
		children[entityIndex]->mNumMeshes = 1;

		children[entityIndex]->mName = e->MeshName() + "_" + std::to_string(e->ID());

		aiMatrix4x4 translationMatrix;
		glm::vec3 position = e->ObjectTransform()->Position();
		aiVector3D entityPosition(position.x, position.y, position.z);
		aiMatrix4x4::Translation(entityPosition, translationMatrix);

		aiMatrix4x4 rotationMatrix;
		glm::quat rotation = e->ObjectTransform()->RotationQuaternion();
		float rotationAngle = glm::angle(rotation);
		glm::vec3 rotationAxis = glm::axis(rotation);
		aiVector3D aiRotationAxis(rotationAxis.x, rotationAxis.y, rotationAxis.z);
		aiMatrix4x4::Rotation(rotationAngle, aiRotationAxis, rotationMatrix);

		aiMatrix4x4 transformation = translationMatrix * rotationMatrix;

		children[entityIndex]->mTransformation = transformation;
		entityIndex++;
	}

	exportScene.mRootNode->addChildren(exportEntityCount, children);

	Assimp::Exporter exporter;
	exporter.Export(&exportScene, "collada", exportFilePath);
}

void SceneExporter::LoadMeshData(aiScene* scene, Mesh * mesh, int meshIndex)
{
	scene->mMeshes[meshIndex]->mNumVertices = mesh->vertices.size();
	scene->mMeshes[meshIndex]->mVertices = new aiVector3D[mesh->vertices.size()];

	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		aiVector3D vertPos;
		vertPos.x = mesh->vertices[i].Position.x;
		vertPos.y = mesh->vertices[i].Position.y;
		vertPos.z = mesh->vertices[i].Position.z;
		scene->mMeshes[meshIndex]->mVertices[i] = vertPos;
	}

	scene->mMeshes[meshIndex]->mNormals = new aiVector3D[mesh->vertices.size()];
	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		aiVector3D vertNormal;
		vertNormal.x = mesh->vertices[i].Normal.x;
		vertNormal.y = mesh->vertices[i].Normal.y;
		vertNormal.z = mesh->vertices[i].Normal.z;
		scene->mMeshes[meshIndex]->mNormals[i] = vertNormal;
	}

	scene->mMeshes[meshIndex]->mTextureCoords[0] = new aiVector3D[mesh->vertices.size()];
	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		aiVector3D vertTexCoords;
		vertTexCoords.x = mesh->vertices[i].TexCoords.x;
		vertTexCoords.y = 1.0f - mesh->vertices[i].TexCoords.y;
		vertTexCoords.z = 0.0f;
		scene->mMeshes[meshIndex]->mTextureCoords[0][i] = vertTexCoords;
	}

	scene->mMeshes[meshIndex]->mNumFaces = (unsigned int)(mesh->indices.size() / 3);
	scene->mMeshes[meshIndex]->mFaces = new aiFace[mesh->indices.size() / 3];
	int faceIndex = 0;
	for (int i = 0; i < mesh->indices.size(); i += 3)
	{
		aiFace face;
		face.mNumIndices = 3;
		face.mIndices = new unsigned int[3];
		face.mIndices[0] = mesh->indices[i];
		face.mIndices[1] = mesh->indices[i + 1];
		face.mIndices[2] = mesh->indices[i + 2];
		scene->mMeshes[meshIndex]->mFaces[faceIndex] = face;
		faceIndex++;
	}
}
