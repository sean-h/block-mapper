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
	int MergeGroupTypeID() const { return mergeGroupTypeID; }
	void MergeGroupTypeID(int id) { this->mergeGroupTypeID = id; }
	int* MergeGroupTypeIDPtr() { return &mergeGroupTypeID; }
	void SetMergeGroupType(int typeID) { mergeGroupTypeID = typeID; }
	static const char* MergeGroupTypesString;
	enum MergeGroupType
	{
		PushObject = 0,
		Trigger = 1,
	};
	static std::vector<std::string> MergeGroupTypesVector();

private:
	std::string name;
	
	int mergeGroupTypeID;
	std::unordered_map<EntityProperty, std::string> properties;
};

