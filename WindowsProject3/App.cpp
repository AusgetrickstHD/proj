#include "App.h"
#include <sstream>
#include <iomanip>
#include <iostream>

App::App() 
	:
	wnd(600,800,L"lulll")
{}

int App::Go() {

	while (true) {
		if ( const auto error = Window::ProcessMessages() ) {
			return *error;
		}
		
		UpdateDelta();
	}
}

void App::UpdateDelta() noexcept  {
	wnd.Gfx().ClearBuffer(0.7f, 0.2f, 0.4f);
	wnd.Gfx().EndFrame();
}