#pragma once
#include "Window.h"
#include "Timer.h"

class App
{
public:
	App();

	int Go();
private:
	void UpdateDelta() noexcept;
private:
	Window wnd;
};