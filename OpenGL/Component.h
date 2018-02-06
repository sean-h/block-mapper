#pragma once

class Entity;
class ApplicationContext;

class Component
{
public:
	Entity* Owner() const { return entity; }
	void Owner(Entity* entity) { this->entity = entity; }
	virtual void Update(ApplicationContext* context);

private:
	Entity* entity;
};