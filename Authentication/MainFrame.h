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
class ChangeUsPswDlg;

/*
Класс стартового окна
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

	std::vector<User> start_users;	// Переписать под мапу (быстрее поиск)

	void EnterToSystem(bool isSucces, wxString login);	// Функция для аудита входов в систему

private:
	wxTextCtrl* user_name;	// Поле с логином
	wxTextCtrl* password;	// Поле с паролем
	UserFrame* user_frame;
	AdminFrame* admin_frame;
	FirstEnterDlg* first_enter;
	ChangeUsPswDlg* change_dlg;

	int num_mis;			// Количество неправильных вводов пароля
	bool isFirstStart;		// Флаг первого входа в систему
};

string wxString_to_lowercase(const wxString& str);
