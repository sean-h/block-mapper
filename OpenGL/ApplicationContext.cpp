#include "ApplicationContext.h"

ApplicationContext::ApplicationContext()
{
	time = std::make_unique<Time>(Time());
	input = std::make_unique<Input>(Input());
	window = std::make_unique<Window>(Window(800, 600));
	window->SetCallbackInput(input.get());
	window->SetCallbackWindow(window.get());
	fileManager = std::make_unique<FileManager>(FileManager());
	renderer = std::make_unique<Renderer>(Renderer(fileManager.get()));
	physics = std::make_unique<Physics>(Physics());
	scene = std::make_unique<Scene>(Scene());
	blockManager = std::make_unique<BlockManager>(BlockManager(fileManager.get()));
}
