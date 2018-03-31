#include "SceneExporter.h"
#include "BlenderPostProcessor.h"
#include "glm\gtc\matrix_transform.hpp"
#include <vector>
#include <set>
#include <stack>

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
	for (auto &e : context->ApplicationScene()->Entities())
	{
		if (e->HasProperty(EntityProperty::Block))
		{
			this->exportObjects.push_back(CreateMeshExportObject(e.get()));
			this->exportObjects.push_back(CreateCollisionExportObject(e.get()));
		}
	}

	// Merge Export Objects / Create merged Export Meshes
	this->MergeMeshes();

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

	BlenderPostProcessor postProcessor(exportFilePath);
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
	exportObject.gridPosition = entity->ObjectTransform()->GridPosition();

	exportObject.objectType = ExportObject::ObjectType::Mesh;

	aiMatrix4x4 transformation = TransformMatrix(
		entity->ObjectTransform()->Position(),
		entity->ObjectTransform()->RotationQuaternion(),
		entity->ObjectTransform()->Scale());
	exportObject.transformMatrix = transformation;

	if (entity->HasProperty(EntityProperty::MergeGroup))
	{
		exportObject.mergeGroup = entity->PropertyValue(EntityProperty::MergeGroup);
	}

	return exportObject;
}

ExportObject SceneExporter::CreateCollisionExportObject(Entity * entity)
{
	ExportObject colliderObject;
	std::string colliderMeshName = entity->ColliderMeshName() + "0";
	colliderObject.exportMeshData = this->exportMeshes[colliderMeshName];
	colliderObject.name = "Collider_" + entity->MeshName() + "_" + std::to_string(entity->ID());
	colliderObject.gridPosition = entity->ObjectTransform()->GridPosition();

	colliderObject.objectType = ExportObject::ObjectType::Collider;

	aiMatrix4x4 transformation = TransformMatrix(
		entity->ObjectTransform()->Position(),
		entity->ObjectTransform()->RotationQuaternion(),
		entity->ObjectTransform()->Scale());
	colliderObject.transformMatrix = transformation;

	if (entity->HasProperty(EntityProperty::MergeGroup))
	{
		colliderObject.mergeGroup = entity->PropertyValue(EntityProperty::MergeGroup);
	}

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

void SceneExporter::MergeMeshes()
{
	ExportMap meshObjectsMap;
	ExportMap collisionObjectsMap;
	for (auto& obj : exportObjects)
	{
		if (obj.objectType == ExportObject::ObjectType::Mesh)
		{
			if (meshObjectsMap.find(obj.gridPosition) != meshObjectsMap.end())
			{
				int a = 0;
			}

			meshObjectsMap[obj.gridPosition] = &obj;
		}
		else if (obj.objectType == ExportObject::ObjectType::Collider)
		{
			collisionObjectsMap[obj.gridPosition] = &obj;
		}
	}

	auto mergedMeshObjects = MergeExportObjects(meshObjectsMap, "MergedMesh");
	auto mergedCollisionObjects = MergeExportObjects(collisionObjectsMap, "ColliderMergedMesh");

	this->exportObjects.clear();
	this->exportObjects.insert(this->exportObjects.end(), mergedMeshObjects.begin(), mergedMeshObjects.end());
	this->exportObjects.insert(this->exportObjects.end(), mergedCollisionObjects.begin(), mergedCollisionObjects.end());
}

std::vector<glm::ivec3> SceneExporter::AdjacentPlanePositions(glm::ivec3 position, SceneExporter::Planes plane)
{
	std::vector<glm::ivec3> adjacent;

	if (plane == SceneExporter::Planes::XY)
	{
		adjacent.push_back(position + glm::ivec3(1, 0, 0));
		adjacent.push_back(position + glm::ivec3(-1, 0, 0));
		adjacent.push_back(position + glm::ivec3(0, 1, 0));
		adjacent.push_back(position + glm::ivec3(0, -1, 0));
	}
	else if (plane == SceneExporter::Planes::XZ)
	{
		adjacent.push_back(position + glm::ivec3(1, 0, 0));
		adjacent.push_back(position + glm::ivec3(-1, 0, 0));
		adjacent.push_back(position + glm::ivec3(0, 0, 1));
		adjacent.push_back(position + glm::ivec3(0, 0, -1));
	}
	else if (plane == SceneExporter::Planes::YZ)
	{
		adjacent.push_back(position + glm::ivec3(0, 1, 0));
		adjacent.push_back(position + glm::ivec3(0, -1, 0));
		adjacent.push_back(position + glm::ivec3(0, 0, 1));
		adjacent.push_back(position + glm::ivec3(0, 0, -1));
	}

	return adjacent;
}

SceneExporter::Planes SceneExporter::DetermineMergePlane(ExportMap exportMap, ExportObject anchorObject)
{
	glm::ivec3 anchorPosition = anchorObject.gridPosition;
	int xBias = 0;
	int yBias = 0;
	int zBias = 0;
	if (exportMap.find(anchorPosition + glm::ivec3(1, 0, 0)) != exportMap.end())
		xBias++;
	if (exportMap.find(anchorPosition + glm::ivec3(-1, 0, 0)) != exportMap.end())
		xBias++;
	if (exportMap.find(anchorPosition + glm::ivec3(0, 1, 0)) != exportMap.end())
		yBias++;
	if (exportMap.find(anchorPosition + glm::ivec3(0, -1, 0)) != exportMap.end())
		yBias++;
	if (exportMap.find(anchorPosition + glm::ivec3(0, 0, 1)) != exportMap.end())
		zBias++;
	if (exportMap.find(anchorPosition + glm::ivec3(0, 0, -1)) != exportMap.end())
		zBias++;

	int xyBias = xBias + yBias;
	int xzBias = xBias + zBias;
	int yzBias = yBias + zBias;

	Planes plane;

	if (xyBias >= xzBias && xyBias >= yzBias)
	{
		plane = Planes::XY;
	}
	else if (yzBias >= xyBias && yzBias >= xzBias)
	{
		plane = Planes::YZ;
	}
	else
	{
		plane = Planes::XZ;
	}

	return plane;
}

ExportMap SceneExporter::PlaneAdjacentExportObjects(ExportMap exportMap, glm::ivec3 startPosition, SceneExporter::Planes plane)
{
	ExportMap adjacentExportObjects;

	std::set<glm::ivec3> visited;
	std::stack<glm::ivec3> searchFrom;
	searchFrom.push(startPosition);

	while (searchFrom.size() > 0)
	{
		auto adjacent = AdjacentPlanePositions(searchFrom.top(), plane);
		searchFrom.pop();

		for (auto& pos : adjacent)
		{
			if (visited.find(pos) != visited.end())
			{
				continue;
			}

			visited.insert(pos);

			auto adjacentBlock = exportMap[pos];
			if (adjacentBlock)
			{
				int neighbourCount = 0;
				auto neighbours = AdjacentPlanePositions(pos, plane);
				for (auto& n : neighbours)
				{
					if (exportMap[n])
					{
						neighbourCount++;
					}
				}

				if (neighbourCount >= 3)
				{
					adjacentExportObjects[pos] = exportMap[pos];
					searchFrom.push(pos);
				}

			}
		}
	}

	return adjacentExportObjects;
}

std::vector<ExportObject> SceneExporter::MergeExportObjects(ExportMap exportMap, std::string meshnamePrefix)
{
	ExportMap remainingMap = exportMap;
	std::vector<ExportObject> mergedMeshObjects;

	// Merge user defined meshes first
	std::unordered_map<std::string, std::vector<ExportObject>> mergeGroups;
	for (auto& exportObject : exportMap)
	{
		if (exportObject.second->mergeGroup != "")
		{
			mergeGroups[exportObject.second->mergeGroup].push_back(*exportObject.second);
			remainingMap.erase(exportObject.first);
		}
	}

	for (auto& mergeGroup : mergeGroups)
	{
		std::vector<ExportMeshData> transformedMeshData;
		for (auto& obj : mergeGroup.second)
		{
			ExportMeshData meshData(*obj.exportMeshData.get());
			meshData.TransformVertexPositions(obj.transformMatrix);
			transformedMeshData.push_back(meshData);
		}

		ExportMeshData mergedMeshData(transformedMeshData);
		mergedMeshData.name = meshnamePrefix + std::to_string(mergedMeshObjects.size());
		exportMeshes[mergedMeshData.name] = std::make_shared<ExportMeshData>(mergedMeshData);
		exportMeshUseCount[mergedMeshData.name] = 0;

		ExportObject mergedMeshObject;
		mergedMeshObject.name = meshnamePrefix + "Object_" + mergeGroup.first;
		mergedMeshObject.exportMeshData = exportMeshes[mergedMeshData.name];
		mergedMeshObjects.push_back(mergedMeshObject);
	}

	while (remainingMap.size() > 0)
	{
		ExportObject* obj = remainingMap.begin()->second;
		Planes mergePlane = this->DetermineMergePlane(exportMap, *obj);
		ExportMap adjacentObjects = this->PlaneAdjacentExportObjects(exportMap, obj->gridPosition, mergePlane);

		int remainingAdjacentObjects = 0;
		for (auto& obj : adjacentObjects)
		{
			if (remainingMap.find(obj.first) == remainingMap.end())
			{
				// Object has already been merged
				continue;
			}
			remainingAdjacentObjects++;
		}

		if (remainingAdjacentObjects == 0)
		{
			// No adjacent objects, add original object with original data to object vector
			mergedMeshObjects.push_back(*obj);
			remainingMap.erase(remainingMap.begin()->first);
		}
		else
		{
			adjacentObjects[obj->gridPosition] = obj;
			std::vector<ExportMeshData> transformedMeshData;
			for (auto& obj : adjacentObjects)
			{
				if (remainingMap.find(obj.first) == remainingMap.end())
				{
					continue;
				}

				ExportMeshData meshData(*obj.second->exportMeshData);
				meshData.TransformVertexPositions(obj.second->transformMatrix);
				transformedMeshData.push_back(meshData);
			}

			ExportMeshData mergedMeshData(transformedMeshData);
			mergedMeshData.name = meshnamePrefix + std::to_string(mergedMeshObjects.size());
			exportMeshes[mergedMeshData.name] = std::make_shared<ExportMeshData>(mergedMeshData);
			exportMeshUseCount[mergedMeshData.name] = 0;

			ExportObject mergedMeshObject;
			mergedMeshObject.name = meshnamePrefix + "Object" + std::to_string(mergedMeshObjects.size());
			mergedMeshObject.exportMeshData = exportMeshes[mergedMeshData.name];
			mergedMeshObjects.push_back(mergedMeshObject);
		}

		for (auto& adjacentObject : adjacentObjects)
		{
			remainingMap.erase(adjacentObject.first);
		}
	}

	return mergedMeshObjects;
}

ExportMeshData::ExportMeshData(const ExportMeshData & meshData)
{
	this->name = meshData.name;
	this->vertexPositions = meshData.vertexPositions;
	this->vertexNormals = meshData.vertexNormals;
	this->vertexTextureCoords = meshData.vertexTextureCoords;
	this->faces = meshData.faces;
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
