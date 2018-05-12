#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class MergeGroup
{
public:
	MergeGroup(int id, std::string name);
	std::string Name() const { return name; }
	void AddProperty(std::string propertyName, std::string propertyValue);
	void RemoveProperty(std::string propertyName);
	bool HasProperty(std::string propertyName) const;
	std::string PropertyValue(std::string propertyName) const;
	std::unordered_map<std::string, std::string> Properties() const;
	bool InputGroupCollapsed() const { return inputGroupCollapsed; }
	void InputGroupCollapsed(bool collapsed) { inputGroupCollapsed = collapsed; }
	char* CollapseButtonName() { return collapseButtonName.get(); }
	char* PushObjectType() { return pushObjectType.get(); }
	char* PushObjectTypeInputName() { return pushObjectTypeInputName.get(); }
	char* DirectionX() { return directionX.get(); }
	char* DirectionY() { return directionY.get(); }
	char* DirectionZ() { return directionZ.get(); }
	char* Distance() { return distance.get(); }
	char* DirectionXInputName() { return directionXInputName.get(); }
	char* DirectionYInputName() { return directionYInputName.get(); }
	char* DirectionZInputName() { return directionZInputName.get(); }
	char* DistanceInputName() { return distanceInputName.get(); }

	static const int TextLength = 64;

private:
	int id;
	std::string name;
	std::unordered_map<std::string, std::string> properties;
	
	// UI Values
	bool inputGroupCollapsed;
	std::unique_ptr<char> collapseButtonName;
	std::unique_ptr<char> pushObjectType;
	std::unique_ptr<char> pushObjectTypeInputName;
	std::unique_ptr<char> directionX;
	std::unique_ptr<char> directionXInputName;
	std::unique_ptr<char> directionY;
	std::unique_ptr<char> directionYInputName;
	std::unique_ptr<char> directionZ;
	std::unique_ptr<char> directionZInputName;
	std::unique_ptr<char> distance;
	std::unique_ptr<char> distanceInputName;
};