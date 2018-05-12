#include "MergeGroupWindow.h"
#include "ApplicationContext.h"

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
		std::unique_ptr<MergeGroup> mergeGroup(new MergeGroup(context->ApplicationScene()->NextMergeGroupID(), newMergeGroupName));
		auto& mergeGroups = context->ApplicationScene()->MergeGroups();
		mergeGroups.push_back(std::move(mergeGroup));

		strcpy_s(newMergeGroupName, inputTextLength, "MergeGroup");
		CheckNewMergeGroupName(context->ApplicationScene());
	}

	ImGui::Separator();

	for (auto& mergeGroup : context->ApplicationScene()->MergeGroups())
	{
		if (ImGui::SmallButton(mergeGroup->CollapseButtonName()))
		{
			mergeGroup->InputGroupCollapsed(!mergeGroup->InputGroupCollapsed());
		}
		ImGui::SameLine();
		ImGui::Text(mergeGroup->Name().c_str());

		if (!mergeGroup->InputGroupCollapsed())
		{
			ImGui::Spacing();
			ImGui::SameLine();
			ImGui::Text("Push Object Type");
			ImGui::SetCursorPosX(50.0f);
			ImGui::InputText(mergeGroup->PushObjectTypeInputName(), mergeGroup->PushObjectType(), MergeGroup::TextLength);

			ImGui::Spacing();
			ImGui::SameLine();
			ImGui::Text("Direction");
			ImGui::SetCursorPosX(50.0f);
			ImGui::InputText(mergeGroup->DirectionXInputName(), mergeGroup->DirectionX(), MergeGroup::TextLength);
			ImGui::SetCursorPosX(50.0f);
			ImGui::InputText(mergeGroup->DirectionYInputName(), mergeGroup->DirectionY(), MergeGroup::TextLength);
			ImGui::SetCursorPosX(50.0f);
			ImGui::InputText(mergeGroup->DirectionZInputName(), mergeGroup->DirectionZ(), MergeGroup::TextLength);

			ImGui::Spacing();
			ImGui::SameLine();
			ImGui::Text("Distance");
			ImGui::SetCursorPosX(50.0f);
			ImGui::InputText(mergeGroup->DistanceInputName(), mergeGroup->Distance(), MergeGroup::TextLength);
		}
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
