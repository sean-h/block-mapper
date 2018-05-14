#include "SceneMetaDataExporter.h"
#include "Scene.h"
#include "tinyxml2.h"

SceneMetaDataExporter::SceneMetaDataExporter(std::string metaDataExportPath)
	: metaDataExportPath(metaDataExportPath)
{
}

void SceneMetaDataExporter::Export(Scene * scene)
{
	tinyxml2::XMLDocument xmlDoc;

	auto sceneNode = xmlDoc.NewElement("SceneMetaData");

	auto mergeGroupsNode = xmlDoc.NewElement("MergeGroups");
	for (auto& mergeGroup : scene->MergeGroups())
	{
		auto mergeGroupNode = xmlDoc.NewElement("MergeGroup");

		auto nameNode = xmlDoc.NewElement("Name");
		nameNode->SetText(mergeGroup->Name().c_str());
		mergeGroupNode->InsertEndChild(nameNode);

		auto pushObjectTypeNode = xmlDoc.NewElement("PushObjectType");
		pushObjectTypeNode->SetText(mergeGroup->PushObjectType());
		mergeGroupNode->InsertEndChild(pushObjectTypeNode);

		auto directionNode = xmlDoc.NewElement("Direction");
		directionNode->SetAttribute("X", mergeGroup->DirectionX());
		directionNode->SetAttribute("Y", mergeGroup->DirectionY());
		directionNode->SetAttribute("Z", mergeGroup->DirectionZ());
		mergeGroupNode->InsertEndChild(directionNode);

		auto distanceNode = xmlDoc.NewElement("Distance");
		distanceNode->SetText(mergeGroup->Distance());
		mergeGroupNode->InsertEndChild(distanceNode);

		mergeGroupsNode->InsertEndChild(mergeGroupNode);
	}
	sceneNode->InsertEndChild(mergeGroupsNode);

	xmlDoc.InsertEndChild(sceneNode);

	xmlDoc.SaveFile(metaDataExportPath.c_str());
}
