#pragma once
#include "AdminFrame.h"
#include "MainFrame.h"
#include <wx/regex.h>

#include <fstream>
#include <vector>

#include "User.h"

using std::vector;

const int ID_CHANGE_US = 5;
const int ID_CHANGE_US_PSW = 6;
const int ID_US_LOGOUT = 7;

class ChangeUsPswDlg;

class UserFrame : public wxFrame
{
public:
	UserFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxASCII_STR(wxFrameNameStr));
	~UserFrame();

	void OnChangePsw(wxMenuEvent& event);
	void OnLogOut(wxMenuEvent& event);

	void ExitFromSystem(wxString login);	// ������� ��� ������ ������� �� �������

	vector<User> user_users;
	wxStaticText* hello;

public:
	wxString UserName;

private:
	ChangeUsPswDlg* change_dlg;

};


class ChangeUsPswDlg : public wxDialog
{
public:
	ChangeUsPswDlg(wxWindow* parent);

	void OnOkBtn(wxCommandEvent& event);

	wxString GetOldPsw() { return old_psw->GetValue(); }
	wxString GetNewPsw() { return new_psw->GetValue(); }
	wxString GetConfirmPsw() { return confirm_psw->GetValue(); }

private:
	wxTextCtrl* old_psw;
	wxTextCtrl* new_psw;
	wxTextCtrl* confirm_psw;
};