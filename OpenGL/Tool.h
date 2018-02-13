#pragma once
class ApplicationContext;

class Tool
{
public:
	virtual void Update(ApplicationContext* context) { }
	virtual void DrawGUI(ApplicationContext* context) { }
};