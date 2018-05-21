#include "SceneMetaDataExporter.h"
#include "Scene.h"
#include "MergeGroup.h"
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
	auto mergeGroupTypes = MergeGroup::MergeGroupTypesVector();
	for (auto& mergeGroup : scene->MergeGroups())
	{
		auto mergeGroupNode = xmlDoc.NewElement("MergeGroup");
		mergeGroupNode->SetAttribute("Type", mergeGroupTypes[mergeGroup->MergeGroupTypeID()].c_str());

		auto nameNode = xmlDoc.NewElement("Name");
		nameNode->SetText(mergeGroup->Name().c_str());
		mergeGroupNode->InsertEndChild(nameNode);

		switch ((MergeGroup::MergeGroupType)mergeGroup->MergeGroupTypeID())
		{
			case MergeGroup::MergeGroupType::PushObject:
			{
				auto pushObjectTypeNode = xmlDoc.NewElement("PushObjectType");
				pushObjectTypeNode->SetText(mergeGroup->PropertyValue(EntityProperty::PushObjectType).c_str());
				mergeGroupNode->InsertEndChild(pushObjectTypeNode);

				auto directionNode = xmlDoc.NewElement("Direction");
				directionNode->SetAttribute("X", mergeGroup->PropertyValue(EntityProperty::DirectionX).c_str());
				directionNode->SetAttribute("Y", mergeGroup->PropertyValue(EntityProperty::DirectionY).c_str());
				directionNode->SetAttribute("Z", mergeGroup->PropertyValue(EntityProperty::DirectionZ).c_str());
				mergeGroupNode->InsertEndChild(directionNode);

				auto distanceNode = xmlDoc.NewElement("Distance");
				distanceNode->SetText(mergeGroup->PropertyValue(EntityProperty::Distance).c_str());
				mergeGroupNode->InsertEndChild(distanceNode);
				break;
			}
		case MergeGroup::MergeGroupType::Trigger:
			auto targetsNode = xmlDoc.NewElement("Targets");

			auto targetNode = xmlDoc.NewElement("Target");
			targetNode->SetText(mergeGroup->PropertyValue(EntityProperty::Target).c_str());
			targetsNode->InsertEndChild(targetNode);

			mergeGroupNode->InsertEndChild(targetsNode);
			break;
		}

		mergeGroupsNode->InsertEndChild(mergeGroupNode);
	}
	sceneNode->InsertEndChild(mergeGroupsNode);

	xmlDoc.InsertEndChild(sceneNode);

	xmlDoc.SaveFile(metaDataExportPath.c_str());
}
