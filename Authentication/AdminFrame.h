#pragma once
#include "wx/wx.h"
#include <wx/regex.h>

#include "User.h"
#include "MainFrame.h"

#include <fstream>
#include <vector>
#include <algorithm>

using std::vector;
using std::string;

const int ID_CHANGE = 1;
const int ID_BTN = 2;
const int ID_CHAGE_PSW = 3;
const int ID_LOGOUT = 4;
const int ID_MINMAX = 100;

class ChangePswDlg;
class AddNewUserDlg;
class SetMinMaxDlg;

// Класс окна админа
class AdminFrame : public wxFrame
{
public:
	AdminFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxASCII_STR(wxFrameNameStr));
	~AdminFrame();

	void OnMenuExit(wxMenuEvent& event);
	void OnMenuAbout(wxMenuEvent& event);
	void OnChangePsw(wxMenuEvent& event);
	void OnLogOut(wxMenuEvent& event);
	void OnAddNew(wxCommandEvent& event);
	void OnSelect(wxCommandEvent& event);
	//void OnSave(wxCommandEvent& event);
	void OnMinMax(wxCommandEvent& event);
	void OnDClick(wxCommandEvent& event);

	void ExitFromSystem(wxString login);	// Функция для аудита выходов из систему
	void CnangeUserData(wxString operation, wxString username);	// Функция для аудита изменений в файле учетных записей

	void OnChange_block(wxCommandEvent& event);
	void OnChange_limit(wxCommandEvent& event);


private:
	wxListBox* list;	// Листбокс для отображения имен пользователей
	wxCheckBox* block;	
	wxCheckBox* limit;
	ChangePswDlg* change_dlg;
	AddNewUserDlg* addnew_dlg;
	SetMinMaxDlg* setminmax_dlg;
public:
	std::vector<User> users;		// Переписать под мапу (быстрее поиск)
};


// Класс диалога первого входа в систему
class FirstEnterDlg : public wxDialog
{
public:
	FirstEnterDlg(wxWindow* parent);

	void OnOkBtn(wxCommandEvent& event);

	wxString GetFirstPsw() { return first_psw->GetValue(); }
	wxString GetConfirmPsw() { return confirm_psw->GetValue(); }

	wxString userNameForSearch;

private:
	wxTextCtrl* first_psw;
	wxTextCtrl* confirm_psw;
};

// Класс диалога смены пароля
class ChangePswDlg : public wxDialog
{
public:
	ChangePswDlg(wxWindow* parent);

	void OnOkBtn(wxCommandEvent& event);

	wxString GetOldPsw() { return old_psw->GetValue(); }
	wxString GetNewPsw() { return new_psw->GetValue(); }
	wxString GetConfirmPsw() { return confirm_psw->GetValue(); }

private:
	wxTextCtrl* old_psw;
	wxTextCtrl* new_psw;
	wxTextCtrl* confirm_psw;
};

// Класс диалога добавления нового пользователя админом
class AddNewUserDlg : public wxDialog
{
public:
	AddNewUserDlg();

public:
	wxTextCtrl* new_username;
	wxCheckBox* block;
	wxCheckBox* limit;
};

// Класс диалога установки минимального и максимального времени действия пароля
class SetMinMaxDlg : public wxDialog
{
public:
	SetMinMaxDlg();

public:
	wxTextCtrl* min;
	wxTextCtrl* max;
};
