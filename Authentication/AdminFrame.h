#pragma once
#include "wx/wx.h"
#include <wx/regex.h>
#include <wx/grid.h>

#include "User.h"
#include "MainFrame.h"

#include <fstream>
#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>

using std::vector;
using std::string;

const int ID_CHANGE = 1;
const int ID_BTN = 2;
const int ID_CHAGE_PSW = 3;
const int ID_LOGOUT = 4;
const int ID_MINMAX = 100;
const int ID_AUDIT_OPERATIONS = 101;
const int ID_AUDIT = 102;
const int ID_OK_NAMES = 103;
const int ID_BTN2 = 104;
const int ID_CLEAR = 105;

class ChangePswDlg;
class AddNewUserDlg;
class SetMinMaxDlg;
class AuditOperationsDlg;
class AuditDlg;
class AuditsFilesNames;

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
	void OnAuditOperations(wxMenuEvent& event);
	void OnAudit(wxMenuEvent& event);
	void OnLogOut(wxMenuEvent& event);
	void OnAddNew(wxCommandEvent& event);
	void OnSelect(wxCommandEvent& event);
	void OnMinMax(wxCommandEvent& event);
	void OnDClick(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);

	void ExitFromSystem(wxString login);	// Функция для аудита выходов из системы
	void CnangeUserData(wxString operation, wxString username);	// Функция для аудита изменений в файле учетных записей

	void OnChange_block(wxCommandEvent& event);
	void OnChange_limit(wxCommandEvent& event);
	void OnChange_spin(wxCommandEvent& event);
	void OnChange_auditspin(wxCommandEvent& event);


private:
	wxListBox* list;	// Листбокс для отображения имен пользователей
	wxCheckBox* block;	
	wxCheckBox* limit;
	wxSpinCtrl* spin;
	wxButton* spin_btn;
	wxSpinCtrl* spin_length_audit;
	wxButton* spin_length_audit_btn;
	ChangePswDlg* change_dlg;
public:
	AuditOperationsDlg* auditop_dlg;
private:
	AuditDlg* audit_dlg;
	AddNewUserDlg* addnew_dlg;
	SetMinMaxDlg* setminmax_dlg;

	AuditsFilesNames* aud_names_dlg;
public:
	std::vector<User> users;	
	wxString AdminName;

	// файлы аудита
	string audfile_1;
	string audfile_2;
};

// Класс диалога первого входа Админа систему
class FirstAdminEnterDlg : public wxDialog
{
public:
	FirstAdminEnterDlg(wxWindow* parent);

	void OnOkBtn(wxCommandEvent& event);

	wxString GetFirstPsw() { return first_psw->GetValue(); }
	wxString GetConfirmPsw() { return confirm_psw->GetValue(); }

	wxString userNameForSearch;

private:
	wxTextCtrl* first_psw;
	wxTextCtrl* confirm_psw;

	wxTextCtrl* enter_audit;
	wxTextCtrl* changes_audit;
public:
	string EncriptionPsw(wxString str, char b);

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
public:
	string EncriptionPsw(wxString str, char b);
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
public:
	string EncriptionPsw(wxString str, char b);					// Функция шифрования пароля
	string Decode(wxString str1, wxString str2, char b);		// Функция расшифрования пароля
};

// Класс диалога установки имен для файлов аудита
class AuditsFilesNames : public wxDialog
{
public:
	AuditsFilesNames(wxWindow* parent);

	wxString GetEnterAuditName() { return enter_audit->GetValue(); }
	wxString GetChangesAuditName() { return changes_audit->GetValue(); }

public:
	wxTextCtrl* enter_audit;
	wxTextCtrl* changes_audit;
};

class Audit 
{
public:
	Audit(wxString op, wxString un, wxString dt) {
		adminOperation = op;
		userName = un;
		dateTime = dt;
	}
public:
	wxString adminOperation;
	wxString userName;
	wxString dateTime;
};

// Класс диалога аудита изменений в файле учетных записей
class AuditOperationsDlg : public wxDialog
{
public:
	AuditOperationsDlg(wxWindow* parent, wxString f_name, string file_name, bool isEnterAaudit = false);

	virtual void OnSortCol(wxGridEvent& event);
	virtual void OnQuery(wxCommandEvent& event);
	virtual void OnReset(wxCommandEvent& event);

	void OnSaveFile(wxCommandEvent& event);
	void OnClearFile(wxCommandEvent& event);

protected:
	wxGrid* grid;
	vector<Audit> audits;

	wxTextCtrl* tc_operation;
	wxTextCtrl* tc_name;
	wxTextCtrl* tc_datetime_start;
	wxTextCtrl* tc_datetime_end;
	wxButton* btn_query;
	wxButton* btn_reset;

	wxButton* save_file;
	wxButton* clear_file;

	wxString _file_name;
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
	wxSpinCtrl* min;
	wxSpinCtrl* max;
};
