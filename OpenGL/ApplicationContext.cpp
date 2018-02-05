#include "ApplicationContext.h"

ApplicationContext::ApplicationContext()
{
	time = std::make_unique<Time>(Time());
	input = std::make_unique<Input>(Input());
	window = std::make_unique<Window>(Window(800, 600));
	renderer = std::make_unique<Renderer>(Renderer());
	physics = std::make_unique<Physics>(Physics());
	scene = std::make_unique<Scene>(Scene());
}
