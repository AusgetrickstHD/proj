#include "Window.h"

int main() {
	Window test(500, 500, L"test");

	MSG msg = {};
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}