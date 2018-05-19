#pragma once
#include "Entity.h"
#include <string>
#include <unordered_map>
#include <memory>

class MergeGroup
{
public:
	MergeGroup(std::string name);
	std::string Name() const { return name; }
	void AddProperty(EntityProperty property, std::string propertyValue);
	void RemoveProperty(EntityProperty property);
	bool HasProperty(EntityProperty property) const;
	std::string PropertyValue(EntityProperty property) const;
	std::unordered_map<EntityProperty, std::string> Properties() const;

private:
	std::string name;
	std::unordered_map<EntityProperty, std::string> properties;
};