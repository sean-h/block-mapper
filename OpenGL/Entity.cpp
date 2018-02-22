#include "Entity.h"

Entity::Entity(int id) : id(id)
{
	entityHandle = std::make_shared<EntityHandle>(EntityHandle(this));
}
