#include "UndoAction.h"
#include "ApplicationContext.h"

void UndoAction::operator()(PlaceEntities & action, ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();

	std::vector<std::shared_ptr<EntityHandle>> destroyEntities;

	for (auto& entity : scene->Entities())
	{
		for (auto entityID : action.entityIDs)
		{
			if (entity->ID() == entityID)
			{
				destroyEntities.push_back(entity->Handle());

				break;
			}
		}
	}

	for (auto& handle : destroyEntities)
	{
		scene->DestroyEntity(handle);
	}
}

void UndoAction::operator()(DeleteEntities & action, ApplicationContext * context)
{
}
