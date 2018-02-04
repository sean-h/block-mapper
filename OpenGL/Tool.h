#pragma once
#include "Scene.h"
#include "Input.h"
#include "Physics.h"

class Tool
{
public:
	virtual void Update(Scene* scene, Input* input, Physics* physics) { }
};