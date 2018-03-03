#include "Entity.h"

Entity::Entity(int id) : id(id)
{
	entityHandle = std::make_shared<EntityHandle>(EntityHandle(this));
	renderID = 0;
}

void Entity::RemoveProperty(std::string propertyName)
{
	properties.erase(propertyName);
}
