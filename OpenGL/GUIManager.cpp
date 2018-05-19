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
	windowLocations["Tools"] = GUILocation(10.0f, 40.0f, 115.0f, 210.0f);
	windowLocations["BlockManager"] = GUILocation(10.0f, 550.0f, 275.0f, 400.0f);
	windowLocations["Scene"] = GUILocation(0.0f, 40.0f, 150.0f, 200.0f);
	windowLocations["SelectionManager"] = GUILocation(0.0f, 260.0f, 150.0f, 400.0f);

	mainMenuOpen = false;
	confirmSceneOverwrite = false;
	mergeGroupWindow = std::unique_ptr<MergeGroupWindow>(new MergeGroupWindow());
	mergeGroupWindow->Close();
}

GUIManager::~GUIManager()
{
	ImGui_ImplGlfwGL3_Shutdown();
}

void GUIManager::StartFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void GUIManager::OnSceneLoaded(ApplicationContext * context)
{
	mergeGroupWindow->OnSceneLoaded(context);
}

void GUIManager::Draw(ApplicationContext* context)
{
	float windowWidth = (float)context->ApplicationWindow()->Width();
	float windowHeight = (float)context->ApplicationWindow()->Height();
	windowLocations["BlockManager"] = GUILocation(10.0f, windowHeight - windowLocations["BlockManager"].height - 10.0f,
		windowLocations["BlockManager"].width, windowLocations["BlockManager"].height);
	windowLocations["MainMenuBar"] = GUILocation(0.0f, 0.0f, windowWidth, 20.0f);
	windowLocations["Scene"] = GUILocation(windowWidth - 160.0f, 40.0f, 150.0f, 200.0f);
	windowLocations["SelectionManager"] = GUILocation(windowWidth - 160.0f, windowLocations["SelectionManager"].yPosition,
		windowLocations["SelectionManager"].width, windowLocations["SelectionManager"].height);

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
				this->SaveScene(context);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Ctrl+Q"))
			{
				context->ApplicationWindow()->Close();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			mainMenuOpen = true;
			if (ImGui::MenuItem("Merge Groups"))
			{
				mergeGroupWindow->ToggleOpen();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (*mergeGroupWindow->OpenPtr())
	{
		ImGui::Begin(mergeGroupWindow->Title().c_str(), mergeGroupWindow->OpenPtr(), ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos(mergeGroupWindow->Position());
		ImGui::SetWindowSize(mergeGroupWindow->Size());
		mergeGroupWindow->Draw(context);
		ImGui::End();
	}

	bool toolsWindowOpen = true;
	ImGui::Begin("Tools", &toolsWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(windowLocations["Tools"].xPosition, windowLocations["Tools"].yPosition));
	ImGui::SetWindowSize(ImVec2(windowLocations["Tools"].width, windowLocations["Tools"].height));
	context->ApplicationToolManager()->DrawGUI(context);
	ImGui::End();

	context->ApplicationBlockManager()->DrawGUI(context);

	bool sceneWindowOpen = true;
	ImGui::Begin("Scene", &sceneWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(windowLocations["Scene"].xPosition, windowLocations["Scene"].yPosition));
	ImGui::SetWindowSize(ImVec2(windowLocations["Scene"].width, windowLocations["Scene"].height));
	context->ApplicationScene()->DrawGUI(context);
	ImGui::End();

	bool selectionManagerWindowOpen = true;
	ImGui::Begin("Selection Manager", &selectionManagerWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(windowLocations["SelectionManager"].xPosition, windowLocations["SelectionManager"].yPosition));
	ImGui::SetWindowSize(ImVec2(windowLocations["SelectionManager"].width, windowLocations["SelectionManager"].height));
	context->ApplicationEntitySelectionManager()->DrawGUI(context);
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

	if (confirmSceneOverwrite)
	{
		bool showConfirmSceneOverwrite = true;
		ImGui::Begin("Confirm Scene Overwrite", &showConfirmSceneOverwrite, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(ImVec2(windowWidth / 2.0f - 200.0f, windowHeight / 2.0f - 40.0f));
		ImGui::SetWindowSize(ImVec2(400.0f, 80.0f));
		std::string overwriteText = "Are you sure you want to overwrite \"" + context->ApplicationScene()->SceneName() + "\"?";
		ImGui::Text(overwriteText.c_str());
		ImGui::Spacing();
		if (ImGui::Button("No"))
		{
			confirmSceneOverwrite = false;
		}
		ImGui::SameLine(100.0f, 0.0f);
		if (ImGui::Button("Yes"))
		{
			lastSceneSaveName = context->ApplicationScene()->SceneName();
			this->SaveScene(context);
		}
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

	// Main Menu Keyboard Shortcuts
	Input* input = context->ApplicationInput();
	if (input->GetKey(Input::Keys::KEY_LEFT_CONTROL))
	{
		if (input->GetKeyDown(Input::Keys::KEY_Q))
		{
			context->ApplicationWindow()->Close();
		}
		if (input->GetKeyDown(Input::Keys::KEY_S))
		{
			this->SaveScene(context);
		}
		if (input->GetKeyDown(Input::Keys::KEY_O))
		{
			this->OpenFileSelector(context);
		}
	}

	ImGui::Render();
}

bool GUIManager::MouseOverGUIElement(float mouseX, float mouseY)
{
	if (fileSelector || confirmSceneOverwrite)
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

	if (*mergeGroupWindow->OpenPtr() &&
		mouseX >= mergeGroupWindow->Position().x && mouseX <= mergeGroupWindow->Position().x + mergeGroupWindow->Size().x &&
		mouseY >= mergeGroupWindow->Position().y && mouseY <= mergeGroupWindow->Position().y + mergeGroupWindow->Size().y)
	{
		return true;
	}

	return mainMenuOpen;
}

GUILocation GUIManager::WindowLocation(std::string windowName)
{
	return windowLocations[windowName];
}

void GUIManager::OpenFileSelector(ApplicationContext* context)
{
	auto filenames = context->ApplicationFileManager()->SavedSceneFilenames();

	std::function<void(std::string)> acceptFunction = std::bind(&GUIManager::AcceptFileSelector, this, context, std::placeholders::_1);
	std::function<void()> cancelFunction = std::bind(&GUIManager::CloseFileSelector, this);

	fileSelector = std::make_unique<FileSelector>(filenames, acceptFunction, cancelFunction);
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

void GUIManager::SaveScene(ApplicationContext * context)
{
	bool savedScene = context->ApplicationScene()->SaveSceneWithOverwriteConfirmation(context, lastSceneSaveName);
	confirmSceneOverwrite = !savedScene;

	if (savedScene)
	{
		lastSceneSaveName = context->ApplicationScene()->SceneName();
	}
}

GUILocation::GUILocation()
{
	xPosition = 0.0f;
	yPosition = 0.0f;
	width = 0.0f;
	height = 0.0f;
}
