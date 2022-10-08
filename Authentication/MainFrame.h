#pragma once
#include "wx/wx.h"
#include <wx/regex.h>

#include <fstream>
#include <vector>

#include "User.h"
#include "AdminFrame.h"
#include "UserFrame.h"

#include <ctime>
#include <chrono>

using std::vector;
using std::string;

class UserFrame;
class AdminFrame;
class FirstEnterDlg;

/*
����� ���������� ����
*/
class MainFrame : public wxFrame
{	
public:
	MainFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxASCII_STR(wxFrameNameStr));
	~MainFrame();

	void OnMenuExit(wxMenuEvent& event);
	void OnMenuAbout(wxMenuEvent& event);
	void OnCancelClick(wxCommandEvent& event);
	void OnOkClick(wxCommandEvent& event);

	wxString GetUserName() { return user_name->GetValue(); }
	wxString GetUserPsw() { return password->GetValue(); }

	std::vector<User> start_users;	// ���������� ��� ���� (������� �����)

	void EnterToSystem(bool isSucces, wxString login);	// ������� ��� ������ ������ � �������

private:
	wxTextCtrl* user_name;	// ���� � �������
	wxTextCtrl* password;	// ���� � �������
	UserFrame* user_frame;
	AdminFrame* admin_frame;
	FirstEnterDlg* first_enter;

	int num_mis;			// ���������� ������������ ������ ������
	bool isFirstStart;		// ���� ������� ����� � �������
};

