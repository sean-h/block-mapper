#pragma once

class Entity;

class Component
{
public:
	Entity* Owner() const { return entity; }
	void Update();

private:
	Entity* entity;
};