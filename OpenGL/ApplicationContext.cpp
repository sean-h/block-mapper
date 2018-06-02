#include "ApplicationContext.h"

ApplicationContext::ApplicationContext()
{
	time = std::unique_ptr<Time>(new Time());
	input = std::unique_ptr<Input>(new Input());
	window = std::unique_ptr<Window>(new Window(1200, 800));
	window->SetCallbackInput(input.get());
	window->SetCallbackWindow(window.get());
	fileManager = std::unique_ptr<FileManager>(new FileManager());
	renderer = std::unique_ptr<Renderer>(new Renderer(fileManager.get()));
	physics = std::unique_ptr<Physics>(new Physics(fileManager.get()));
	scene = std::unique_ptr<Scene>(new Scene());
	blockManager = std::unique_ptr<BlockManager>(new BlockManager(fileManager.get()));
	guiManager = std::unique_ptr<GUIManager>(new GUIManager(window.get()));
	toolManager = std::unique_ptr<ToolManager>(new ToolManager(this));
	entitySelectionManager = std::unique_ptr<EntitySelectionManager>(new EntitySelectionManager());
	debug = std::unique_ptr<Debug>(new Debug());
	undoManager = std::unique_ptr<UndoManager>(new UndoManager());
}
