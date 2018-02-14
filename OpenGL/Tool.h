#pragma once
class ApplicationContext;

class Tool
{
public:
	virtual ~Tool() {}
	virtual void Update(ApplicationContext* context) { }
	virtual void DrawGUI(ApplicationContext* context) { }
	virtual void DisableTool(ApplicationContext* context) { }
};