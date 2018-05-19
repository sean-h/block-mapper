#include "MergeGroupWindow.h"
#include "ApplicationContext.h"
#include <string>

MergeGroupWindow::MergeGroupWindow()
{
	this->position = ImVec2(300, 300);
	this->size = ImVec2(400, 300);
	this->title = "Merge Groups";

	Open();

	newMergeGroupName = new char[inputTextLength];
	strcpy_s(newMergeGroupName, inputTextLength, "MergeGroup");
	validNewMergeGroupName = true;
}

void MergeGroupWindow::Draw(ApplicationContext * context)
{
	// Save value in order to correctly pop the style after the InputText
	bool validName = validNewMergeGroupName;
	if (!validName)
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.7f, 0.0f, 0.0f, 1.0f));
	if (ImGui::InputText("##newMergeGroupName", newMergeGroupName, inputTextLength))
	{
		CheckNewMergeGroupName(context->ApplicationScene());
	}
	if (!validName)
		ImGui::PopStyleColor();

	ImGui::SameLine();

	if (ImGui::Button("Create") && validNewMergeGroupName)
	{
		std::unique_ptr<MergeGroup> mergeGroup(new MergeGroup(newMergeGroupName));
		std::unique_ptr<MergeGroupUIItem> mergeGroupUItem(new MergeGroupUIItem(mergeGroup.get(), mergeGroupUIItems.size()));
		mergeGroupUIItems.push_back(std::move(mergeGroupUItem));
		
		auto& mergeGroups = context->ApplicationScene()->MergeGroups();
		mergeGroups.push_back(std::move(mergeGroup));

		strcpy_s(newMergeGroupName, inputTextLength, "MergeGroup");
		CheckNewMergeGroupName(context->ApplicationScene());
	}

	ImGui::Separator();

	for (auto& mergeGroup : mergeGroupUIItems)
	{
		mergeGroup->Draw();
	}
}

void MergeGroupWindow::OnSceneLoaded(ApplicationContext * context)
{
	mergeGroupUIItems.clear();

	for (auto& mergeGroup : context->ApplicationScene()->MergeGroups())
	{
		std::unique_ptr<MergeGroupUIItem> mergeGroupUItem(new MergeGroupUIItem(mergeGroup.get(), mergeGroupUIItems.size()));

		mergeGroupUItem->SetPropertyValue(EntityProperty::PushObjectType, mergeGroup->PropertyValue(EntityProperty::PushObjectType));
		mergeGroupUItem->SetPropertyValue(EntityProperty::DirectionX, mergeGroup->PropertyValue(EntityProperty::DirectionX));
		mergeGroupUItem->SetPropertyValue(EntityProperty::DirectionY, mergeGroup->PropertyValue(EntityProperty::DirectionY));
		mergeGroupUItem->SetPropertyValue(EntityProperty::DirectionZ, mergeGroup->PropertyValue(EntityProperty::DirectionZ));
		mergeGroupUItem->SetPropertyValue(EntityProperty::Distance, mergeGroup->PropertyValue(EntityProperty::Distance));

		mergeGroupUIItems.push_back(std::move(mergeGroupUItem));
	}
}

void MergeGroupWindow::CheckNewMergeGroupName(Scene* scene)
{
	validNewMergeGroupName = true;

	std::string newName(newMergeGroupName);

	for (auto& mergeGroup : scene->MergeGroups())
	{
		if (newName == mergeGroup->Name())
		{
			validNewMergeGroupName = false;
			break;
		}
	}
}

MergeGroupUIItem::MergeGroupUIItem(MergeGroup * mergeGroup, int id)
	: mergeGroup(mergeGroup), id(id)
{
	inputBuffers[EntityProperty::PushObjectType] = std::unique_ptr<char>(new char[TextLength]);
	inputBuffers[EntityProperty::DirectionX] = std::unique_ptr<char>(new char[TextLength]);
	inputBuffers[EntityProperty::DirectionY] = std::unique_ptr<char>(new char[TextLength]);
	inputBuffers[EntityProperty::DirectionZ] = std::unique_ptr<char>(new char[TextLength]);
	inputBuffers[EntityProperty::Distance] = std::unique_ptr<char>(new char[TextLength]);

	collapseButtonName = std::unique_ptr<char>(new char[TextLength]);
	inputNames[EntityProperty::PushObjectType] = std::unique_ptr<char>(new char[TextLength]);
	inputNames[EntityProperty::DirectionX] = std::unique_ptr<char>(new char[TextLength]);
	inputNames[EntityProperty::DirectionY] = std::unique_ptr<char>(new char[TextLength]);
	inputNames[EntityProperty::DirectionZ] = std::unique_ptr<char>(new char[TextLength]);
	inputNames[EntityProperty::Distance] = std::unique_ptr<char>(new char[TextLength]);

	strcpy_s(inputBuffers[EntityProperty::PushObjectType].get(), TextLength, "Default");
	strcpy_s(inputBuffers[EntityProperty::DirectionX].get(), TextLength, "0");
	strcpy_s(inputBuffers[EntityProperty::DirectionY].get(), TextLength, "1");
	strcpy_s(inputBuffers[EntityProperty::DirectionZ].get(), TextLength, "0");
	strcpy_s(inputBuffers[EntityProperty::Distance].get(), TextLength, "1");

	strcpy_s(collapseButtonName.get(), TextLength, std::string("^##mergeGroupCollapse" + std::to_string(id)).c_str());
	strcpy_s(inputNames[EntityProperty::PushObjectType].get(), TextLength, std::string("##pushObjectType" + std::to_string(id)).c_str());
	strcpy_s(inputNames[EntityProperty::DirectionX].get(), TextLength, std::string("X##directionX" + std::to_string(id)).c_str());
	strcpy_s(inputNames[EntityProperty::DirectionY].get(), TextLength, std::string("Y##directionY" + std::to_string(id)).c_str());
	strcpy_s(inputNames[EntityProperty::DirectionZ].get(), TextLength, std::string("Z##directionZ" + std::to_string(id)).c_str());
	strcpy_s(inputNames[EntityProperty::Distance].get(), TextLength, std::string("##distance" + std::to_string(id)).c_str());
}

void MergeGroupUIItem::Draw()
{
	if (ImGui::SmallButton(collapseButtonName.get()))
	{
		itemCollapsed = !itemCollapsed;
	}
	ImGui::SameLine();
	ImGui::Text(mergeGroup->Name().c_str());

	if (!itemCollapsed)
	{
		ImGui::Spacing();
		ImGui::SameLine();
		ImGui::Text("Push Object Type");
		ImGui::SetCursorPosX(50.0f);
		DrawPropertyInput(EntityProperty::PushObjectType);

		ImGui::Spacing();
		ImGui::SameLine();
		ImGui::Text("Direction");
		ImGui::SetCursorPosX(50.0f);
		DrawPropertyInput(EntityProperty::DirectionX);
		ImGui::SetCursorPosX(50.0f);
		DrawPropertyInput(EntityProperty::DirectionY);
		ImGui::SetCursorPosX(50.0f);
		DrawPropertyInput(EntityProperty::DirectionZ);

		ImGui::Spacing();
		ImGui::SameLine();
		ImGui::Text("Distance");
		ImGui::SetCursorPosX(50.0f);
		DrawPropertyInput(EntityProperty::Distance);
	}
}

void MergeGroupUIItem::SetPropertyValue(EntityProperty property, std::string value)
{
	strcpy_s(inputBuffers[property].get(), TextLength, value.c_str());
}

void MergeGroupUIItem::DrawPropertyInput(EntityProperty property)
{
	if (ImGui::InputText(inputNames[property].get(), inputBuffers[property].get(), TextLength))
	{
		mergeGroup->AddProperty(property, inputBuffers[property].get());
	}
}
