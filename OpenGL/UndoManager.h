#pragma once
#include "UndoAction.h"
#include <memory>
#include <stack>

class ApplicationContext;

class UndoManager
{
public:
	void PushUndoAction(std::unique_ptr<UndoAction> action);
	void Undo(ApplicationContext* context);
	void Update(ApplicationContext* context);

private:
	std::stack<std::unique_ptr<UndoAction>> undoStack;
};