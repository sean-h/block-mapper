#include "BlenderPostProcessor.h"
#include <Windows.h>
#include <fstream>
#include <string>
#include <regex>

BlenderPostProcessor::BlenderPostProcessor(std::string exportFilepath)
{
	// Load PostProcess.py
	std::ifstream postProcessFileStream("PostProcess.py");
	std::string postProcessStringTemp;
	postProcessFileStream.seekg(0, std::ios::end);
	postProcessStringTemp.resize(postProcessFileStream.tellg());
	postProcessFileStream.seekg(0, std::ios::beg);
	postProcessFileStream.read(&postProcessStringTemp[0], postProcessStringTemp.size());
	postProcessFileStream.close();

	// Remove trailing NULL terminators
	std::string postProcessString(postProcessStringTemp.begin(), std::find(postProcessStringTemp.begin(), postProcessStringTemp.end(), '\0'));

	// Replace import variables
	std::string exportFilepathFormatted = std::regex_replace(exportFilepath, std::regex("\\\\"), "/");
	std::string postProcessScript = std::regex_replace(postProcessString, std::regex("#map_import_path"), exportFilepathFormatted);

	// Write script to file
	std::string exportScriptFilePath = std::regex_replace(exportFilepathFormatted, std::regex(".dae"), ".py");
	std::ofstream outputStream;
	outputStream.open(exportScriptFilePath);
	outputStream << postProcessScript;
	outputStream.close();

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	char* command = new char[128];
	std::string commandString = "blender --background --python " + exportScriptFilePath;
	strcpy_s(command, 128, commandString.c_str());
	CreateProcess(NULL, command, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	delete[] command;
} 
