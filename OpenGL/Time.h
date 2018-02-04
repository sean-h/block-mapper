#pragma once

class Time
{
public:
	Time();
	void StartFrame();
	double DeltaTime() const { return deltaTime; }

private:
	double deltaTime;
	double lastFrame;
};