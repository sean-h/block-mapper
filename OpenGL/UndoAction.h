#pragma once
#include "Entity.h"
#include <memory>
#include <variant>
#include <vector>

class ApplicationContext;

struct UndoAction
{
	// Action Types
	struct PlaceEntities
	{
		std::vector<int> entityIDs;
	};

	struct DeleteEntities {};

	std::variant <PlaceEntities, DeleteEntities> action;

	// Action Methods
	void operator()(PlaceEntities& action, ApplicationContext* context);
	void operator()(DeleteEntities& action, ApplicationContext* context);
};