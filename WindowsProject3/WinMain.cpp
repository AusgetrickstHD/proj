#include "App.h"

int CALLBACK wWinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrectInstance,
		LPWSTR lpCmdShow,
		int nCmdShow) {

	App app;
	try {

		app.Go();

	}
	catch (const CustomException& e) {
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBoxA(nullptr, "No details available", "Unkown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}