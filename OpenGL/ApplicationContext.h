#pragma once
#include <memory>
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Renderer.h"
#include "Physics.h"
#include "Time.h"

class ApplicationContext
{
public:
	ApplicationContext();

	Time* ApplicationTime() const { return time.get(); }
	Input* ApplicationInput() const { return input.get(); }
	Window* ApplicationWindow() const { return window.get(); }
	Renderer* ApplicationRenderer() const { return renderer.get(); }
	Physics* ApplicationPhysics() const { return physics.get(); }
	Scene* ApplicationScene() const { return scene.get(); }

private:
	std::unique_ptr<Time> time;
	std::unique_ptr<Input> input;
	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Physics> physics;
	std::unique_ptr<Scene> scene;
};