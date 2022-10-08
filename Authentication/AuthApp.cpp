#include "AuthApp.h"
#include "MainFrame.h"

bool AuthApp::OnInit() {
	MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, wxT("¬ход"), wxPoint(-1, -1), wxSize(370, 225));
	mainFrame->Show(true);

	return true;
}
