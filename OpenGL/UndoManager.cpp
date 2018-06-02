#include "UndoManager.h"
#include "ApplicationContext.h"

void UndoManager::PushUndoAction(std::unique_ptr<UndoAction> action)
{
	undoStack.push(std::move(action));
}

void UndoManager::Undo(ApplicationContext * context)
{
	if (undoStack.size() == 0)
	{
		return;
	}

	auto undoAction = std::move(undoStack.top());
	undoStack.pop();
	std::visit([context](auto& action) { UndoAction{}(action, context); }, undoAction->action);
}

void UndoManager::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();

	if (input->GetKey(Input::Keys::KEY_LEFT_CONTROL) && input->GetKeyDown(Input::Keys::KEY_Z))
	{
		Undo(context);
	}
}
