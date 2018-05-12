#include "MergeGroup.h"
#include <string>

MergeGroup::MergeGroup(int id, std::string name)
	: id(id), name(name)
{
	pushObjectType = std::unique_ptr<char>(new char[TextLength]);
	directionX = std::unique_ptr<char>(new char[TextLength]);
	directionY = std::unique_ptr<char>(new char[TextLength]);
	directionZ = std::unique_ptr<char>(new char[TextLength]);
	distance = std::unique_ptr<char>(new char[TextLength]);

	collapseButtonName = std::unique_ptr<char>(new char[TextLength]);
	pushObjectTypeInputName = std::unique_ptr<char>(new char[TextLength]);
	directionXInputName = std::unique_ptr<char>(new char[TextLength]);
	directionYInputName = std::unique_ptr<char>(new char[TextLength]);
	directionZInputName = std::unique_ptr<char>(new char[TextLength]);
	distanceInputName = std::unique_ptr<char>(new char[TextLength]);

	strcpy_s(this->pushObjectType.get(), TextLength, "Default");
	strcpy_s(this->directionX.get(), TextLength, "0");
	strcpy_s(this->directionY.get(), TextLength, "1");
	strcpy_s(this->directionZ.get(), TextLength, "0");
	strcpy_s(this->distance.get(), TextLength, "1");

	strcpy_s(this->collapseButtonName.get(), TextLength, std::string("^##mergeGroupCollapse" + std::to_string(id)).c_str());
	strcpy_s(this->pushObjectTypeInputName.get(), TextLength, std::string("##pushObjectType" + std::to_string(id)).c_str());
	strcpy_s(this->directionXInputName.get(), TextLength, std::string("X##directionX" + std::to_string(id)).c_str());
	strcpy_s(this->directionYInputName.get(), TextLength, std::string("Y##directionY" + std::to_string(id)).c_str());
	strcpy_s(this->directionZInputName.get(), TextLength, std::string("Z##directionZ" + std::to_string(id)).c_str());
	strcpy_s(this->distanceInputName.get(), TextLength, std::string("##distance" + std::to_string(id)).c_str());
}

void MergeGroup::AddProperty(std::string propertyName, std::string propertyValue)
{
}

void MergeGroup::RemoveProperty(std::string propertyName)
{
}

bool MergeGroup::HasProperty(std::string propertyName) const
{
	return false;
}

std::string MergeGroup::PropertyValue(std::string propertyName) const
{
	return std::string();
}

std::unordered_map<std::string, std::string> MergeGroup::Properties() const
{
	return std::unordered_map<std::string, std::string>();
}
