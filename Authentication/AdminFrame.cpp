#include "AdminFrame.h"

AdminFrame::AdminFrame(wxWindow* parent,
	wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) : wxFrame(parent, id, title, pos, size, style, name)
{
	// ����
	wxMenuBar* menu_bar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT, wxT("�����...\tCtrl+e"));
	wxMenu* admin = new wxMenu;
	admin->Append(ID_CHANGE, wxT("����� ������...\tCtrl+n"));
	admin->Append(ID_LOGOUT, wxT("����� �� ������� ������"));
	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT, wxT("� ���������...\tCtrl+h"));
	menu_bar->Append(file, wxT("����"));
	menu_bar->Append(admin, wxT("�����"));
	menu_bar->Append(help, wxT("�������"));

	SetMenuBar(menu_bar);

	wxPanel* pnl = new wxPanel(this, -1);
	wxBoxSizer* h_box = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);

	list = new wxListBox(pnl, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1)/*, arr*/);
	h_box->Add(list, 1, wxEXPAND | wxRIGHT | wxLEFT, 15);

	wxButton* add_new = new wxButton(pnl, wxID_ADD, wxT("��������"));
	v_box->Add(add_new, 0, wxEXPAND | wxBOTTOM, 15);
	block = new wxCheckBox(pnl, wxID_ANY, wxT("����������"));
	v_box->Add(block, 0, wxEXPAND | wxBOTTOM, 15);
	block->Enable(false);
	limit = new wxCheckBox(pnl, wxID_ANY, wxT("����������� \n�� ������"));
	v_box->Add(limit, 0, wxEXPAND | wxBOTTOM, 15);
	limit->Enable(false);
	wxButton* set_minmax = new wxButton(pnl, ID_MINMAX, wxT("����������\n���, ����\n���� �������� ������"));
	v_box->Add(set_minmax, 0, wxEXPAND | wxBOTTOM, 15);
	wxButton* save = new wxButton(pnl, wxID_SAVE, wxT("���������"));
	v_box->Add(save, 0, wxEXPAND | wxBOTTOM, 15);
	h_box->Add(v_box, 1, wxEXPAND | wxRIGHT | wxLEFT, 15);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	around_box->Add(h_box, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	pnl->SetSizer(around_box);
	Centre();

	Connect(wxID_ABOUT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnMenuAbout));
	Connect(wxID_EXIT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnMenuExit));
	Connect(ID_CHANGE, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnChangePsw));
	Connect(ID_LOGOUT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnLogOut));
	Connect(wxID_ADD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AdminFrame::OnAddNew));
	//Connect(wxID_SAVE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AdminFrame::OnSave));	// ���� ��������, ����� ������ �������� ���� ����� ��������� checkbox
	Connect(block->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(AdminFrame::OnChange_block));
	Connect(limit->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(AdminFrame::OnChange_limit));
	Connect(ID_MINMAX, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AdminFrame::OnMinMax));
	Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(AdminFrame::OnSelect));
	Connect(list->GetId(), wxEVT_LISTBOX_DCLICK, wxCommandEventHandler(AdminFrame::OnDClick));

	string s_name;
	string s_psw;
	string list_pswds;
	string s_isblock;
	string s_islimit;
	string s_mintime;
	string s_maxtime;

	std::ifstream fin("users.txt");
	if (fin.is_open())
	{
		while (!fin.eof()) {
			getline(fin, s_name);
			getline(fin, s_psw);
			getline(fin, list_pswds);
			getline(fin, s_isblock);
			getline(fin, s_islimit);
			getline(fin, s_mintime);
			getline(fin, s_maxtime);

			wxString s1(s_isblock);
			wxString s2(s_islimit);
			wxString s3(s_mintime);
			wxString s4(s_maxtime);

			vector<wxString> vec;

			while (list_pswds.size()) {
				auto it = find(list_pswds.begin(), list_pswds.end(), '_');
				if (it != list_pswds.end()) {
					string s_length = list_pswds.substr(0, it - list_pswds.begin());
					int length = atoi(s_length.c_str());
					list_pswds.erase(0, it - list_pswds.begin() + 1);
					vec.push_back(list_pswds.substr(0, length));
					list_pswds.erase(0, length);
				}
			}

			// ���������, ��� � ����� �������������� ��� ������ ���� ???
			User user = User(s_name, s_psw, wxAtoi(s1), wxAtoi(s2), wxAtoi(s3), wxAtoi(s4), vec);
			users.push_back(user);
		}
		fin.close();
	}

	for (int i = 0; i < users.size(); ++i) {
		list->Append(wxString(users[i].name));
	}

	// ���� ��� ������ ��������� � ����� ������� �������
	std::ifstream fin_aud("changes_audit.txt");
	if (!fin_aud.is_open())
	{
		std::ofstream fout("changes_audit.txt");
		fout << "���� ������ ��������� � ����� ������� �������:\n";
		fout.close();
	}
}

void AdminFrame::OnMenuExit(wxMenuEvent& event) {
	Close();
}

void AdminFrame::OnMenuAbout(wxMenuEvent& event) {
	wxMessageBox(wxT("������ �.�. \n������ �-13�-19 \n�������� ������"), wxT("� ���������"));
}

void AdminFrame::OnChangePsw(wxMenuEvent& event) {
	change_dlg = new ChangePswDlg(this);
	change_dlg->ShowModal();
}

void AdminFrame::CnangeUserData(wxString operation, wxString username) {
	std::ofstream fin_aud("changes_audit.txt", std::ios_base::app);
	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char* dt = ctime(&now);
	fin_aud << "��������:" << operation << "\t" << "��� ������������:" << username << "\t" << "����/�����:" << dt;
}

void AdminFrame::ExitFromSystem(wxString login) {
	std::ofstream fin_aud("audit.txt", std::ios_base::app);
	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char* dt = ctime(&now);
	fin_aud << "�������� �����:" << "\t" << "��� ������������:" << login << "\t" << "����/�����:" << dt;
}

void AdminFrame::OnLogOut(wxMenuEvent& event) {
	ExitFromSystem(wxT("ADMIN"));

	MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, wxT("����"), wxPoint(-1, -1), wxSize(370, 225));
	mainFrame->start_users = users;
	Destroy();
	mainFrame->Show(true);
}

void AdminFrame::OnAddNew(wxCommandEvent& event) {
	addnew_dlg = new AddNewUserDlg();
	if (addnew_dlg->ShowModal() == wxID_OK) {
		wxString str = addnew_dlg->new_username->GetValue();
		auto it = find_if(users.begin(), users.end(), [str](User& u) { return u.name == str; });
		if (it != users.end()) {	// ���� ������������ � ����� ������ ��� ����
			wxMessageBox(wxT("������������ � ����� ������ ��� ���������� � �������!"));
		}
		else {	// ���� ������ ������������ ��� ���, �� ��������� ������ �� ������ ������������ � ������ � ListBox
			User u(addnew_dlg->new_username->GetValue(), wxT(""), addnew_dlg->block->GetValue(), addnew_dlg->limit->GetValue(), 0, 0, {});	// !!! ������ ���� ������������� ���������� ��������������� ��� � ���� �����, ���� ����
			users.push_back(u);

			CnangeUserData(wxT("���������� ������ ������������"), str);
			if (addnew_dlg->block->GetValue()) {
				CnangeUserData(wxT("��������� ����������"), str);
			}
			else {
				CnangeUserData(wxT("������ ����������"), str);
			}
			if (addnew_dlg->limit->GetValue()) {
				CnangeUserData(wxT("��������� ����������� �� ������"), str);
			}
			else {
				CnangeUserData(wxT("������ ����������� �� ������"), str);
			}

			list->Append(wxString(users[users.size() - 1].name));

			addnew_dlg->Destroy();
		}
	}
	else {
		addnew_dlg->Destroy();
	}
}

//void AdminFrame::OnSave(wxCommandEvent& event) {
//	bool par1 = block->GetValue();
//	bool par2 = limit->GetValue();
//
//	wxString str_name = list->GetStringSelection();
//
//	auto it = find_if(users.begin(), users.end(), [str_name](User& u) { return u.name == str_name; });
//	if (it != users.end()) {
//		it->is_block = par1;
//		it->is_limit = par2;
//	}
//}

void AdminFrame::OnChange_block(wxCommandEvent& event) {
	bool par = block->GetValue();
	
	wxString str_name = list->GetStringSelection();
	
	auto it = find_if(users.begin(), users.end(), [str_name](User& u) { return u.name == str_name; });
	if (it != users.end()) {
		it->is_block = par;	

		if (par) {
			CnangeUserData(wxT("��������� ����������"), str_name);
		}
		else {
			CnangeUserData(wxT("������ ����������"), str_name);
		}
	}
}

void AdminFrame::OnChange_limit(wxCommandEvent& event) {
	bool par = limit->GetValue();

	wxString str_name = list->GetStringSelection();

	auto it = find_if(users.begin(), users.end(), [str_name](User& u) { return u.name == str_name; });
	if (it != users.end()) {
		it->is_limit = par;

		if (par) {
			CnangeUserData(wxT("��������� ����������� �� ������"), str_name);
		}
		else {
			CnangeUserData(wxT("������ ����������� �� ������"), str_name);
		}
	}
}

void AdminFrame::OnMinMax(wxCommandEvent& event) {
	setminmax_dlg = new SetMinMaxDlg();
	if (setminmax_dlg->ShowModal() == wxID_OK) {
		int min = wxAtoi(setminmax_dlg->min->GetValue());
		int max = wxAtoi(setminmax_dlg->max->GetValue());
		if (!min) {
			min = 0;
		}
		for (int i = 0; i < users.size(); ++i) {
			users[i].min_pswtime = min;
		}

		if (!max) {
			max = 0;
		}
		for (int i = 0; i < users.size(); ++i) {
			users[i].max_pswtime = max;
		}

		setminmax_dlg->Destroy();
	}
	else {
		setminmax_dlg->Destroy();
	}
}

void AdminFrame::OnSelect(wxCommandEvent& event) {
	// ����� �������� ������������ � ������ ����������� �������� ��������
	// � ��� �������� ������������� ��������, ������� ������������� ���������� ������������
	wxString str = event.GetString();
	if (str == wxT("ADMIN")) {
		block->Enable(false);
		limit->Enable(false);
	}
	else {
		block->Enable(true);
		limit->Enable(true);
	}
	auto it = find_if(users.begin(), users.end(), [str](User& u) { return u.name == str; });
	if (it != users.end()) {
		block->SetValue(it->is_block);
		limit->SetValue(it->is_limit);
	}
}

void AdminFrame::OnDClick(wxCommandEvent& event) {
	wxString name = event.GetString();
	wxString list_pswds;

	if (name != wxT("ADMIN")) {
		auto it = find_if(users.begin(), users.end(), [name](User& u) { return u.name == name; });
		if (it != users.end()) {
			for (int i = 0; i < it->pswds_list.size(); ++i) {
				list_pswds += wxString::Format(wxT("%i: "), i + 1);
				list_pswds += it->pswds_list[i];
				list_pswds += "\n";
			}
		}
	}

	wxMessageBox(list_pswds, wxT("������ �������������� �������"));
	
}

AdminFrame::~AdminFrame() {		// ��� ����������� �������������� ���� � ����������� � �������������
	std::ofstream fout("users.txt");
	for (int i = 0; i < users.size() - 1; ++i) {
		fout << users[i].name << "\n";
		fout << users[i].psw << "\n";
		for (int j = 0; j < users[i].pswds_list.size(); ++j) {
			fout << users[i].pswds_list[j].size() << "_";
			fout << users[i].pswds_list[j];
		}
		fout << "\n";
		fout << users[i].is_block << "\n";
		fout << users[i].is_limit << "\n";
		fout << users[i].min_pswtime << "\n";
		fout << users[i].max_pswtime << "\n";
	}

	fout << users[users.size() - 1].name << "\n";
	fout << users[users.size() - 1].psw << "\n";
	for (int j = 0; j < users[users.size() - 1].pswds_list.size(); ++j) {
		fout << users[users.size() - 1].pswds_list[j].size() << "_";
		fout << users[users.size() - 1].pswds_list[j];
	}
	fout << "\n";
	fout << users[users.size() - 1].is_block << "\n";
	fout << users[users.size() - 1].is_limit << "\n";
	fout << users[users.size() - 1].min_pswtime << "\n";
	fout << users[users.size() - 1].max_pswtime;

	fout.close();
}

ChangePswDlg::ChangePswDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("����� ������"), wxPoint(-1, -1), wxSize(400, 225))
{
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* h_box = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* v_box1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* v_box2 = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_old = new wxStaticText(pnl, wxID_ANY, wxT("������ ������:"));
	wxStaticText* t_new = new wxStaticText(pnl, wxID_ANY, wxT("����� ������:"));
	wxStaticText* t_apply = new wxStaticText(pnl, wxID_ANY, wxT("����������� ������:"));
	v_box1->Add(t_old, 0, wxALIGN_LEFT | wxBOTTOM, 25);
	v_box1->Add(t_new, 0, wxALIGN_LEFT | wxBOTTOM, 25);
	v_box1->Add(t_apply, 0, wxALIGN_LEFT | wxBOTTOM, 18);
	wxButton* btn_ok = new wxButton(pnl, ID_CHAGE_PSW, wxT("��"));
	v_box1->Add(btn_ok, 0, wxALIGN_CENTRE | wxBOTTOM, 15);

	h_box->Add(v_box1, 1, wxEXPAND | wxALL, 15);

	old_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	new_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	confirm_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	v_box2->Add(old_psw, 0, wxALIGN_LEFT | wxBOTTOM, 15);
	v_box2->Add(new_psw, 0, wxALIGN_LEFT | wxBOTTOM, 15);
	v_box2->Add(confirm_psw, 0, wxALIGN_LEFT | wxBOTTOM, 15);
	wxButton* btn_cancel = new wxButton(pnl, wxID_CANCEL);
	v_box2->Add(btn_cancel, 0, wxALIGN_CENTRE | wxBOTTOM, 15);

	h_box->Add(v_box2, 1, wxEXPAND | wxALL, 15);

	pnl->SetSizer(h_box);

	Centre();

	Connect(ID_CHAGE_PSW, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ChangePswDlg::OnOkBtn));
}

void ChangePswDlg::OnOkBtn(wxCommandEvent& event) {
	if (this->GetOldPsw() == wxT("") || this->GetNewPsw() == wxT("") || this->GetConfirmPsw() == wxT("")) {
		wxMessageBox(wxT("��� ���� ������ ���� ���������"), wxT("������ ��� ����� ������"));
	}
	else {
		wxString oldStr = GetOldPsw();
		AdminFrame* p_wnd = (AdminFrame*)GetParent();
		auto it = find_if(p_wnd->users.begin(), p_wnd->users.end(), [](User& u) { return u.name == wxT("ADMIN"); });
		if (it != p_wnd->users.end()) {
			if (it->psw != oldStr) {
				wxMessageBox(wxT("����������� ������ ������ ������"), wxT("������ ��� ����� ������"));
			}
			else {
				if (this->GetNewPsw() != this->GetConfirmPsw()) {
					wxMessageBox(wxT("������ �� ���������"), wxT("������ ��� ����� ������"));
				}
				else {
					wxMessageBox(wxT("������ ������� �������"), wxT("����� ������"));
					it->psw = this->GetNewPsw();
					Destroy();
				}
			}
		}
	}
}

AddNewUserDlg::AddNewUserDlg() : wxDialog(NULL, wxID_ANY, wxT("����� ������������"), wxPoint(-1, -1), wxSize(300, 250))
{
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* t_name = new wxStaticText(pnl, wxID_ANY, wxT("��� ������������:"));
	new_username = new wxTextCtrl(pnl, wxID_ANY);
	v_box->Add(t_name, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);
	v_box->Add(new_username, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	block = new wxCheckBox(pnl, wxID_ANY, wxT("����������"));
	limit = new wxCheckBox(pnl, wxID_ANY, wxT("����������� \n�� ������"));

	h_box1->Add(block, 1, wxLEFT | wxRIGHT, 15);
	h_box1->Add(limit, 1, wxRIGHT, 15);

	v_box->Add(h_box1, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxButton* btn_ok = new wxButton(pnl, wxID_OK);
	wxButton* btn_cancel = new wxButton(pnl, wxID_CANCEL);
	h_box2->Add(btn_ok, 1, wxLEFT | wxRIGHT, 15);
	h_box2->Add(btn_cancel, 1, wxRIGHT, 15);

	v_box->Add(h_box2, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	around_box->Add(v_box, 1, wxLEFT | wxRIGHT, 15);

	pnl->SetSizer(around_box);

	Centre();
}

FirstEnterDlg::FirstEnterDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("������ ���� � �������"), wxPoint(-1, -1), wxSize(400, 270)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box3 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* info = new wxStaticText(pnl, wxID_ANY, wxT("�� ������� ����� � �������.\n���������� ���� ������."));
	v_box->Add(info, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_psw1 = new wxStaticText(pnl, wxID_ANY, wxT("���������� ������:"));
	first_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box1->Add(t_psw1, 0, wxEXPAND | wxRIGHT, 15);
	h_box1->Add(first_psw, 0, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box1, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_psw2 = new wxStaticText(pnl, wxID_ANY, wxT("��������� ������:"));
	confirm_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box2->Add(t_psw2, 0, wxEXPAND | wxRIGHT, 15);
	h_box2->Add(confirm_psw, 0, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box2, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxButton* ok_btn = new wxButton(pnl, ID_BTN, wxT("��"));
	wxButton* cnl_btn = new wxButton(pnl, wxID_CANCEL, wxT("������"));
	h_box3->Add(ok_btn, 0, wxEXPAND | wxRIGHT, 10);
	h_box3->Add(cnl_btn, 0, wxEXPAND | wxRIGHT, 10);
	v_box->Add(h_box3, 0, wxALIGN_RIGHT);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	around_box->Add(v_box, 1, wxLEFT | wxALL, 15);

	pnl->SetSizer(around_box);

	Centre();

	Connect(ID_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirstEnterDlg::OnOkBtn));
}

void FirstEnterDlg::OnOkBtn(wxCommandEvent& event) {
	if (this->GetFirstPsw() == wxT("") || this->GetConfirmPsw() == wxT("")) {
		wxMessageBox(wxT("��� ���� ������ ���� ���������"), wxT("������ ��� ����� ������"));
	}
	else {
		if (this->GetFirstPsw() != this->GetConfirmPsw()) {
			wxMessageBox(wxT("�������� ����� �� ���������"), wxT("������ ��� ����� ������"));
		}
		else {
			MainFrame* p_wnd = (MainFrame*)GetParent();
			auto it = find_if(p_wnd->start_users.begin(), p_wnd->start_users.end(), [this](User& u) { return u.name == userNameForSearch; });
			if (userNameForSearch == wxT("ADMIN")) {
				it->psw = this->GetFirstPsw();		// ������ ������� ����� ���� �����
				Destroy();
			}
			else {
				if (it->is_limit) {		// ��������� ������ ���������� ����������
					//wxRegEx re("^(([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*[1-9]+[^.,!?:;�-�]*)+|([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*)|([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*))$");
					wxRegEx re("^(([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*[1-9]+[^.,!?:;�-�]*)*(([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*)|([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*))?)$");
					if (re.IsValid()) {
						//wxMessageBox(wxT("Compiled!"));
					}
					if (re.Matches(this->GetFirstPsw())) {
						//wxMessageBox(wxT("Matched"));
						it->psw = this->GetFirstPsw();
						it->pswds_list.push_back(this->GetFirstPsw());
						wxMessageBox(wxT("�� ������� ���������� ������"));
						Destroy();
					}
					else {
						//wxMessageBox(wxT("Not matched!"));
						wxMessageBox(wxT("��� ������ �� ������������� ����������� �����������!\n(����������� ������ ����������, �������� ���������, ����)"));
					}
				}
				else {
					it->psw = this->GetFirstPsw();
					it->pswds_list.push_back(this->GetFirstPsw());
					wxMessageBox(wxT("�� ������� ���������� ������"));
					Destroy();
				}
			}
		}
	}
}

SetMinMaxDlg::SetMinMaxDlg() : wxDialog(NULL, wxID_ANY, wxT("��������� ������� �������� ������"), wxPoint(-1, -1), wxSize(300, 200)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_min = new wxStaticText(pnl, wxID_ANY, wxT("���"));
	wxStaticText* t_max = new wxStaticText(pnl, wxID_ANY, wxT("����"));

	h_box1->Add(t_min, 1, wxEXPAND | wxRIGHT, 10);
	min = new wxTextCtrl(pnl, wxID_ANY);
	h_box1->Add(min, 1, wxEXPAND | wxRIGHT, 10);
	h_box1->Add(t_max, 1, wxEXPAND | wxRIGHT, 10);
	max = new wxTextCtrl(pnl, wxID_ANY);
	h_box1->Add(max, 1, wxEXPAND);

	v_box->Add(h_box1, 0, wxEXPAND | wxBOTTOM, 10);

	wxButton* ok_btn = new wxButton(pnl, wxID_OK, wxT("��"));
	wxButton* cnl_btn = new wxButton(pnl, wxID_CANCEL, wxT("������"));

	h_box2->Add(ok_btn, 0, wxEXPAND | wxRIGHT, 10);
	h_box2->Add(cnl_btn, 0, wxEXPAND);

	v_box->Add(h_box2, 0, wxALIGN_RIGHT);

	around_box->Add(v_box, 0, wxEXPAND | wxALL, 10);

	pnl->SetSizer(around_box);
}