#include "ApplicationContext.h"

ApplicationContext::ApplicationContext()
{
	time = std::make_unique<Time>(Time());
	input = std::make_unique<Input>(Input());
	window = std::make_unique<Window>(Window(1200, 800));
	window->SetCallbackInput(input.get());
	window->SetCallbackWindow(window.get());
	fileManager = std::make_unique<FileManager>(FileManager());
	renderer = std::make_unique<Renderer>(Renderer(fileManager.get()));
	physics = std::make_unique<Physics>(Physics());
	scene = std::make_unique<Scene>(Scene());
	blockManager = std::make_unique<BlockManager>(BlockManager(fileManager.get()));
	guiManager = std::make_unique<GUIManager>(GUIManager(window.get()));
	toolManager = std::make_unique<ToolManager>(ToolManager(this));
}
