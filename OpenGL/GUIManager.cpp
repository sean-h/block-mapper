#include "GUIManager.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Window.h"
#include "ApplicationContext.h"
#include "FileSelector.h"

GUIManager::GUIManager(Window* window)
{
	ImGui_ImplGlfwGL3_Init(window->GLFWWindow(), true);

	windowLocations["MainMenuBar"] = GUILocation(0.0f, 0.0f, 1000.0f, 10.0f);
	windowLocations["Tools"] = GUILocation(10.0f, 40.0f, 150.0f, 200.0f);
	windowLocations["ActiveTool"] = GUILocation(10.0f, 250.0f, 150.0f, 200.0f);
	windowLocations["BlockManager"] = GUILocation(10.0f, 550.0f, 275.0f, 300.0f);
	windowLocations["Scene"] = GUILocation(0.0f, 40.0f, 150.0f, 200.0f);

	mainMenuOpen = false;
}

GUIManager::~GUIManager()
{
	ImGui_ImplGlfwGL3_Shutdown();
}

void GUIManager::StartFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void GUIManager::Draw(ApplicationContext* context)
{
	float windowWidth = (float)context->ApplicationWindow()->Width();
	float windowHeight = (float)context->ApplicationWindow()->Height();
	windowLocations["BlockManager"] = GUILocation(10.0f, windowHeight - 310.0f, 275.0f, 300.0f);
	windowLocations["MainMenuBar"] = GUILocation(0.0f, 0.0f, windowWidth, 20.0f);
	windowLocations["Scene"] = GUILocation(windowWidth - 160.0f, 40.0f, 150.0f, 200.0f);

	mainMenuOpen = false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			mainMenuOpen = true;
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{

			}
			if (ImGui::MenuItem("Open..", "Ctrl+O"))
			{
				auto filenames = context->ApplicationFileManager()->SavedSceneFilenames();

				std::function<void(std::string)> acceptFunction = std::bind(&GUIManager::AcceptFileSelector, this, context, std::placeholders::_1);
				std::function<void()> cancelFunction = std::bind(&GUIManager::CloseFileSelector, this);

				fileSelector = std::make_unique<FileSelector>(filenames, acceptFunction, cancelFunction);
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				context->ApplicationScene()->SaveScene(context);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Ctrl+Q"))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	context->ApplicationToolManager()->DrawGUI(context);
	context->ApplicationBlockManager()->DrawGUI(context);

	bool sceneWindowOpen = true;
	ImGui::Begin("Scene", &sceneWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(windowLocations["Scene"].xPosition, windowLocations["Scene"].yPosition));
	ImGui::SetWindowSize(ImVec2(windowLocations["Scene"].width, windowLocations["Scene"].height));
	context->ApplicationScene()->DrawGUI(context);
	ImGui::End();

	if (fileSelector)
	{
		bool showFileSelector = true;
		ImGui::Begin("File Selector", &showFileSelector, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(ImVec2(windowWidth / 2.0f - 200.0f, windowHeight / 2.0f - 150.0f));
		ImGui::SetWindowSize(ImVec2(400.0f, 300.0f));
		fileSelector->DrawGUI();
		ImGui::End();
	}

	if (context->ApplicationDebug()->ItemCount() > 0)
	{
		bool showDebugWindow = true;
		ImGui::Begin("Debug", &showDebugWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		float height = context->ApplicationDebug()->ItemCount() * 20.0f;
		ImGui::SetWindowPos(ImVec2(windowWidth - 410.0f, windowHeight - height - 10.0f));
		ImGui::SetWindowSize(ImVec2(400.0f, height));
		context->ApplicationDebug()->DrawGUI(context);
		ImGui::End();
	}

	ImGui::Render();
}

bool GUIManager::MouseOverGUIElement(float mouseX, float mouseY)
{
	if (fileSelector)
	{
		return true;
	}

	for (auto& windowLocation : windowLocations)
	{
		GUILocation winLoc = windowLocation.second;

		if (mouseX >= winLoc.xPosition && mouseX <= winLoc.xPosition + winLoc.width &&
			mouseY >= winLoc.yPosition && mouseY <= winLoc.yPosition + winLoc.height)
		{
			return true;
		}
	}

	return mainMenuOpen;
}

GUILocation GUIManager::WindowLocation(std::string windowName)
{
	return windowLocations[windowName];
}

void GUIManager::OpenFileSelector()
{
}

void GUIManager::AcceptFileSelector(ApplicationContext* context, std::string filename)
{
	fileSelector = nullptr;
	std::string filepath = context->ApplicationFileManager()->SaveFilePath() + "/" + filename;
	context->ApplicationScene()->LoadScene(context, filepath);
}

void GUIManager::CloseFileSelector()
{
	fileSelector = nullptr;
}

GUILocation::GUILocation()
{
	xPosition = 0.0f;
	yPosition = 0.0f;
	width = 0.0f;
	height = 0.0f;
}
