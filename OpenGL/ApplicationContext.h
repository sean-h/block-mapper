#pragma once
#include <memory>
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Renderer.h"
#include "Physics.h"
#include "Time.h"
#include "FileManager.h"
#include "BlockManager.h"

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
	BlockManager* ApplicationBlockManager() const { return blockManager.get(); }
	FileManager* ApplicationFileManager() const { return fileManager.get(); }

private:
	std::unique_ptr<Time> time;
	std::unique_ptr<Input> input;
	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Physics> physics;
	std::unique_ptr<Scene> scene;
	std::unique_ptr<FileManager> fileManager;
	std::unique_ptr<BlockManager> blockManager;
};