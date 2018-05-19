#include "MergeGroup.h"
#include <string>

MergeGroup::MergeGroup(std::string name)
	: name(name)
{
	AddProperty(EntityProperty::PushObjectType, "Default");
	AddProperty(EntityProperty::DirectionX, "0");
	AddProperty(EntityProperty::DirectionY, "1");
	AddProperty(EntityProperty::DirectionZ, "0");
	AddProperty(EntityProperty::Distance, "1");
}

void MergeGroup::AddProperty(EntityProperty property, std::string propertyValue)
{
	properties[property] = propertyValue;
}

void MergeGroup::RemoveProperty(EntityProperty property)
{
	properties.erase(property);
}

bool MergeGroup::HasProperty(EntityProperty property) const
{
	return properties.find(property) != properties.end();
}

std::string MergeGroup::PropertyValue(EntityProperty prop) const
{
	return properties.find(prop)->second;
}

std::unordered_map<EntityProperty, std::string> MergeGroup::Properties() const
{
	return std::unordered_map<EntityProperty, std::string>();
}
