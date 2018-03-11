#include "SceneExporter.h"
#include "glm\gtc\matrix_transform.hpp"
#include <vector>
#include <set>

void SceneExporter::Export(ApplicationContext * context)
{
	// Load Meshes
	for (auto& model : context->ApplicationRenderer()->Models())
	{
		for (int i = 0; i < model.second->UVIndexCount(); i++)
		{
			std::string meshName = model.first + std::to_string(i);
			this->meshes[meshName] = model.second->GetMesh(i);
		}
	}

	// Create Export Meshes
	for (auto& mesh : this->meshes)
	{
		ExportMeshData meshData = this->CreateExportMeshData(mesh.second);
		meshData.name = mesh.first;
		exportMeshes[mesh.first] = std::make_shared<ExportMeshData>(meshData);
		exportMeshUseCount[mesh.first] = 0;
	}

	// Create Export Objects
	std::vector<ExportObject> exportMeshObjects;
	for (auto &e : context->ApplicationScene()->Entities())
	{
		if (e->HasProperty("Block"))
		{
			this->exportObjects.push_back(CreateMeshExportObject(e.get()));
			this->exportObjects.push_back(CreateCollisionExportObject(e.get()));
		}
	}

	// TODO: Merge Export Objects / Create merged Export Meshes

	// Determine how many times each export mesh is used
	for (auto& exportObject : this->exportObjects)
	{
		std::string meshName = exportObject.exportMeshData->name;
		this->exportMeshUseCount[meshName]++;
	}

	// Remove unused Export Meshes
	for (auto& meshCount : this->exportMeshUseCount)
	{
		if (meshCount.second == 0)
		{
			this->exportMeshes.erase(meshCount.first);
		}
	}

	// Create export meshes list
	for (auto& mesh : this->exportMeshes)
	{
		this->exportMeshesList.push_back(mesh.second);
	}

	// Create export scene
	std::string exportFilePath = context->ApplicationFileManager()->ExportFilePath() + context->ApplicationScene()->SceneName() + ".dae";
	aiScene exportScene;
	exportScene.mRootNode = new aiNode();
	exportScene.mMaterials = new aiMaterial*[1];
	exportScene.mNumMaterials = 1;
	exportScene.mMaterials[0] = new aiMaterial();

	// Create scene meshes
	exportScene.mMeshes = new aiMesh*[exportMeshesList.size()];
	exportScene.mMeshes[0] = nullptr;
	exportScene.mNumMeshes = exportMeshesList.size();

	int meshIndex = 0;
	for (auto& exportMesh : exportMeshesList)
	{
		this->AddMeshDataToScene(&exportScene, exportMesh.get(), meshIndex);
		meshIndex++;
	}

	// Place export objects in scene
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

ExportMeshData SceneExporter::CreateExportMeshData(Mesh * mesh)
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
	scene->mMeshes[meshIndex] = new aiMesh();

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

ExportObject SceneExporter::CreateMeshExportObject(Entity * entity)
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

	return exportObject;
}

ExportObject SceneExporter::CreateCollisionExportObject(Entity * entity)
{
	ExportObject colliderObject;
	std::string colliderMeshName = entity->ColliderMeshName() + "0";
	colliderObject.exportMeshData = this->exportMeshes[colliderMeshName];
	colliderObject.name = "Collider_" + entity->MeshName() + "_" + std::to_string(entity->ID());
	aiMatrix4x4 transformation = TransformMatrix(
		entity->ObjectTransform()->Position(),
		entity->ObjectTransform()->RotationQuaternion(),
		entity->ObjectTransform()->Scale());
	colliderObject.transformMatrix = transformation;
	return colliderObject;
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

std::shared_ptr<ExportMeshData> SceneExporter::GetExportMeshData(std::string meshName)
{
	if (exportMeshes.find(meshName) != exportMeshes.end())
	{
		return exportMeshes[meshName];
	}

	ExportMeshData meshData = this->CreateExportMeshData(this->meshes[meshName]);
	exportMeshes[meshName] = std::make_shared<ExportMeshData>(meshData);

	return exportMeshes[meshName];
}

ExportMeshData::ExportMeshData(std::vector<ExportMeshData> meshDataList)
{
	int vertexCounter = 0;
	for (auto& data : meshDataList)
	{
		for (auto& face : data.faces)
		{
			aiFace f = face;
			for (int i = 0; i < f.mNumIndices; i++)
			{
				f.mIndices[i] += vertexCounter;
			}
			this->faces.push_back(f);
		}

		for (auto& vertex : data.vertexPositions)
		{
			this->vertexPositions.push_back(vertex);
			vertexCounter++;
		}

		for (auto& normal : data.vertexNormals)
		{
			this->vertexNormals.push_back(normal);
		}

		for (auto& texCoord : data.vertexTextureCoords)
		{
			this->vertexTextureCoords.push_back(texCoord);
		}
	}
}

void ExportMeshData::TransformVertexPositions(aiMatrix4x4 transformMatrix)
{
	for (auto& vertexPosition : vertexPositions)
	{
		vertexPosition = transformMatrix * vertexPosition;
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
