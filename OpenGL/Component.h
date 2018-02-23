#pragma once
#include <string>
#include <unordered_map>

class Entity;
class ApplicationContext;

class Component
{
public:
	Entity* Owner() const { return entity; }
	void Owner(Entity* entity) { this->entity = entity; }
	virtual std::string Type() const = 0;
	virtual std::unordered_map<std::string, std::string> Serialize() const = 0;
	virtual void Update(ApplicationContext* context);
	virtual void OnAttachToEntity() { }

private:
	Entity* entity;
};