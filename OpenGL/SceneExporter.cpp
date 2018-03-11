#include "SceneExporter.h"
#include "glm\gtc\matrix_transform.hpp"
#include <vector>
#include <set>

void SceneExporter::Export(ApplicationContext * context)
{
	std::string exportFilePath = context->ApplicationFileManager()->ExportFilePath() + context->ApplicationScene()->SceneName() + ".dae";
	aiScene exportScene;

	exportScene.mRootNode = new aiNode();

	exportScene.mMaterials = new aiMaterial*[1];
	exportScene.mNumMaterials = 1;
	exportScene.mMaterials[0] = new aiMaterial();

	// Determine which meshes are used in the scene
	std::set<std::pair<std::string, int>> usedMeshes;
	for (auto& entity : context->ApplicationScene()->Entities())
	{
		if (!entity->HasProperty("Block"))
		{
			continue;
		}

		if (entity->MeshName() != "")
		{
			usedMeshes.insert(std::make_pair<std::string, int>(entity->MeshName(), entity->MeshColorIndex()));
		}

		if (entity->ColliderMeshName() != "")
		{
			usedMeshes.insert(std::make_pair<std::string, int>(entity->ColliderMeshName(), 0));
		}
	}

	exportScene.mMeshes = new aiMesh*[usedMeshes.size()];
	exportScene.mMeshes[0] = nullptr;
	exportScene.mNumMeshes = usedMeshes.size();

	// Load mesh data
	auto allModels = context->ApplicationRenderer()->Models();
	int meshIndex = 0;
	for (auto& usedMesh : usedMeshes)
	{
		Mesh* mesh = allModels[usedMesh.first]->GetMesh(usedMesh.second);
		exportScene.mMeshes[meshIndex] = new aiMesh();
		ExportMeshData meshData = this->GetMeshData(mesh);
		std::string meshName = usedMesh.first + std::to_string(usedMesh.second);
		exportMeshes[meshName] = std::make_shared<ExportMeshData>(meshData);
		exportMeshesList.push_back(exportMeshes[meshName]);
		this->AddMeshDataToScene(&exportScene, &meshData, meshIndex);
		meshIndexes[usedMesh.first][usedMesh.second] = meshIndex;
		meshIndex++;
	}

	for (auto &e : context->ApplicationScene()->Entities())
	{
		if (e->HasProperty("Block"))
		{
			CreateExportObject(e.get());
		}
	}

	aiNode** children = new aiNode*[exportObjects.size()];
	for (int i = 0; i < exportObjects.size(); i++)
	{
		children[i] = new aiNode();
		children[i]->mMeshes = new unsigned int[1];
		children[i]->mMeshes[0] = this->GetExportMeshDataID(exportObjects[i].exportMeshData);
		children[i]->mNumMeshes = 1;
		children[i]->mName = exportObjects[i].name;
		children[i]->mTransformation = exportObjects[i].transformMatrix;
	}
	exportScene.mRootNode->addChildren(exportObjects.size(), children);

	Assimp::Exporter exporter;
	exporter.Export(&exportScene, "collada", exportFilePath);
}

ExportMeshData SceneExporter::GetMeshData(Mesh * mesh)
{
	ExportMeshData meshData;

	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		aiVector3D vertPos;
		vertPos.x = mesh->vertices[i].Position.x;
		vertPos.y = mesh->vertices[i].Position.y;
		vertPos.z = mesh->vertices[i].Position.z;
		meshData.vertexPositions.push_back(vertPos);
	}

	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		aiVector3D vertNormal;
		vertNormal.x = mesh->vertices[i].Normal.x;
		vertNormal.y = mesh->vertices[i].Normal.y;
		vertNormal.z = mesh->vertices[i].Normal.z;
		meshData.vertexNormals.push_back(vertNormal);
	}

	for (int i = 0; i < mesh->vertices.size(); i++)
	{
		aiVector3D vertTexCoords;
		vertTexCoords.x = mesh->vertices[i].TexCoords.x;
		vertTexCoords.y = 1.0f - mesh->vertices[i].TexCoords.y;
		vertTexCoords.z = 0.0f;
		meshData.vertexTextureCoords.push_back(vertTexCoords);
	}

	int faceIndex = 0;
	for (int i = 0; i < mesh->indices.size(); i += 3)
	{
		aiFace face;
		face.mNumIndices = 3;
		face.mIndices = new unsigned int[3];
		face.mIndices[0] = mesh->indices[i];
		face.mIndices[1] = mesh->indices[i + 1];
		face.mIndices[2] = mesh->indices[i + 2];
		meshData.faces.push_back(face);
		faceIndex++;
	}

	return meshData;
}

void SceneExporter::AddMeshDataToScene(aiScene * scene, ExportMeshData * meshData, int meshIndex)
{
	// Vertex Positions
	scene->mMeshes[meshIndex]->mNumVertices = meshData->vertexPositions.size();
	scene->mMeshes[meshIndex]->mVertices = new aiVector3D[meshData->vertexPositions.size()];
	for (int i = 0; i < meshData->vertexPositions.size(); i++)
	{
		scene->mMeshes[meshIndex]->mVertices[i] = meshData->vertexPositions[i];
	}

	// Vertex Normals
	scene->mMeshes[meshIndex]->mNormals = new aiVector3D[meshData->vertexNormals.size()];
	for (int i = 0; i < meshData->vertexNormals.size(); i++)
	{
		scene->mMeshes[meshIndex]->mNormals[i] = meshData->vertexNormals[i];
	}

	// Texture Coordinates
	scene->mMeshes[meshIndex]->mTextureCoords[0] = new aiVector3D[meshData->vertexTextureCoords.size()];
	for (int i = 0; i < meshData->vertexTextureCoords.size(); i++)
	{
		scene->mMeshes[meshIndex]->mTextureCoords[0][i] = meshData->vertexTextureCoords[i];
	}

	// Faces
	scene->mMeshes[meshIndex]->mNumFaces = meshData->faces.size();
	scene->mMeshes[meshIndex]->mFaces = new aiFace[meshData->faces.size()];
	for (int i = 0; i < meshData->faces.size(); i++)
	{
		scene->mMeshes[meshIndex]->mFaces[i] = meshData->faces[i];
	}
}

void SceneExporter::CreateExportObject(Entity * entity)
{
	ExportObject exportObject;

	std::string meshName = entity->MeshName() + std::to_string(entity->MeshColorIndex());
	exportObject.exportMeshData = this->exportMeshes[meshName];
	exportObject.name = entity->MeshName() + "_" + std::to_string(entity->ID());

	aiMatrix4x4 transformation = TransformMatrix(
		entity->ObjectTransform()->Position(),
		entity->ObjectTransform()->RotationQuaternion(),
		entity->ObjectTransform()->Scale());
	exportObject.transformMatrix = transformation;
	this->exportObjects.push_back(exportObject);

	if (entity->ColliderMeshName() != "")
	{
		ExportObject colliderObject;
		std::string colliderMeshName = entity->ColliderMeshName() + "0";
		colliderObject.exportMeshData = this->exportMeshes[colliderMeshName];
		colliderObject.name = "Collider_" + entity->MeshName() + "_" + std::to_string(entity->ID());
		colliderObject.transformMatrix = transformation;
		this->exportObjects.push_back(colliderObject);
	}
}

int SceneExporter::GetExportMeshDataID(std::shared_ptr<ExportMeshData> exportMeshData)
{
	for (int i = 0; i < exportMeshesList.size(); i++)
	{
		if (exportMeshData == exportMeshesList[i])
		{
			return i;
		}
	}

	return -1;
}

void ExportMeshData::TransformVertexPositions(glm::mat4 transformMatrix)
{
	for (auto& vertexPosition : vertexPositions)
	{
		glm::vec4 v(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0f);
		glm::vec4 newPosition = transformMatrix * v;

		vertexPosition.x = newPosition.x;
		vertexPosition.y = newPosition.y;
		vertexPosition.z = newPosition.z;
	}
}

aiMatrix4x4 TransformMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	aiMatrix4x4 translationMatrix;
	aiVector3D entityPosition(position.x, position.y, position.z);
	aiMatrix4x4::Translation(entityPosition, translationMatrix);

	aiMatrix4x4 rotationMatrix;
	float rotationAngle = glm::angle(rotation);
	glm::vec3 rotationAxis = glm::axis(rotation);
	aiVector3D aiRotationAxis(rotationAxis.x, rotationAxis.y, rotationAxis.z);
	aiMatrix4x4::Rotation(rotationAngle, aiRotationAxis, rotationMatrix);

	aiMatrix4x4 transformation = translationMatrix * rotationMatrix;

	return transformation;
}
