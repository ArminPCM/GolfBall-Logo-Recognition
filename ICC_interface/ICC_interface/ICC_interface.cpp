// ICC_interface.cpp : main project file.

#include "stdafx.h"
#include "ControlWindow.h"

using namespace ICC_interface;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	ControlWindow^ window = gcnew ControlWindow();
	// Create the main window and run it
	if(window->Init())
		Application::Run(window);
	return 0;
}
