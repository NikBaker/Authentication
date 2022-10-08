#include "UserFrame.h"

UserFrame::UserFrame(wxWindow* parent,
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
	admin->Append(ID_CHANGE_US, wxT("����� ������...\tCtrl+n"));
	admin->Append(ID_US_LOGOUT, wxT("����� �� ������� ������"));
	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT, wxT("� ���������...\tCtrl+h"));
	menu_bar->Append(file, wxT("����"));
	menu_bar->Append(admin, wxT("������������"));
	menu_bar->Append(help, wxT("�������"));

	SetMenuBar(menu_bar);

	UserName = wxT("");
	wxPanel* pnl = new wxPanel(this, -1);
	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	hello = new wxStaticText(pnl, wxID_ANY, wxString::Format(wxT("Hello, %s!"), UserName));
	v_box->Add(hello, 1, wxEXPAND | wxALL, 15);

	pnl->SetSizer(v_box);
	Centre();

	Connect(wxID_ABOUT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnMenuAbout));
	Connect(wxID_EXIT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnMenuExit));
	Connect(ID_CHANGE_US, wxEVT_MENU, wxMenuEventHandler(UserFrame::OnChangePsw));
	Connect(ID_US_LOGOUT, wxEVT_MENU, wxMenuEventHandler(UserFrame::OnLogOut));

}

void UserFrame::OnChangePsw(wxMenuEvent& event) {
	change_dlg = new ChangeUsPswDlg(this);
	change_dlg->ShowModal();
}

void UserFrame::ExitFromSystem(wxString login) {
	std::ofstream fin_aud("audit.txt", std::ios_base::app);
	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char* dt = ctime(&now);
	fin_aud << "�������� �����:" << "\t" << "��� ������������:" << login << "\t" << "����/�����:" << dt;
}

void UserFrame::OnLogOut(wxMenuEvent& event) {
	ExitFromSystem(UserName);

	MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, wxT("����"), wxPoint(-1, -1), wxSize(370, 225));
	mainFrame->start_users = user_users;
	Destroy();
	mainFrame->Show(true);
}

UserFrame::~UserFrame() {
	std::ofstream fout("users.txt");
	for (int i = 0; i < user_users.size() - 1; ++i) {
		fout << user_users[i].name << "\n";
		fout << user_users[i].psw << "\n";
		for (int j = 0; j < user_users[i].pswds_list.size(); ++j) {
			fout << user_users[i].pswds_list[j].size() << "_";
			fout << user_users[i].pswds_list[j];
		}
		fout << "\n";
		fout << user_users[i].is_block << "\n";
		fout << user_users[i].is_limit << "\n";
		fout << user_users[i].min_pswtime << "\n";
		fout << user_users[i].max_pswtime << "\n";

	}

	fout << user_users[user_users.size() - 1].name << "\n";
	fout << user_users[user_users.size() - 1].psw << "\n";
	for (int j = 0; j < user_users[user_users.size() - 1].pswds_list.size(); ++j) {
		fout << user_users[user_users.size() - 1].pswds_list[j].size() << "_";
		fout << user_users[user_users.size() - 1].pswds_list[j];
	}
	fout << "\n";
	fout << user_users[user_users.size() - 1].is_block << "\n";
	fout << user_users[user_users.size() - 1].is_limit << "\n";
	fout << user_users[user_users.size() - 1].min_pswtime << "\n";
	fout << user_users[user_users.size() - 1].max_pswtime;

	fout.close();
}

ChangeUsPswDlg::ChangeUsPswDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("����� ������"), wxPoint(-1, -1), wxSize(400, 225))
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
	wxButton* btn_ok = new wxButton(pnl, ID_CHANGE_US_PSW, wxT("��"));
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

	Connect(ID_CHANGE_US_PSW, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ChangeUsPswDlg::OnOkBtn));
}

void ChangeUsPswDlg::OnOkBtn(wxCommandEvent& event) {

	if (this->GetOldPsw() == wxT("") || this->GetNewPsw() == wxT("") || this->GetConfirmPsw() == wxT("")) {
		wxMessageBox(wxT("��� ���� ������ ���� ���������"), wxT("������ ��� ����� ������"));
	}
	else {
		wxString oldStr = GetOldPsw();
		UserFrame* p_wnd = (UserFrame*)GetParent();
		auto it = find_if(p_wnd->user_users.begin(), p_wnd->user_users.end(), [p_wnd](User& u) { return u.name == p_wnd->UserName; });
		if (it != p_wnd->user_users.end()) {
			if (it->psw != oldStr) {
				wxMessageBox(wxT("����������� ������ ������ ������"), wxT("������ ��� ����� ������"));
			}
			else {
				if (this->GetNewPsw() != this->GetConfirmPsw()) {
					wxMessageBox(wxT("������ �� ���������"), wxT("������ ��� ����� ������"));
				}
				else {
					if (it->is_limit) {		// ��������� ����� ������ ���������� ����������
						wxRegEx re("^(([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*[1-9]+[^.,!?:;�-�]*)*(([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*)|([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*))?)$");
						if (re.IsValid()) {
							//wxMessageBox(wxT("Compiled!"));
						}
						if (re.Matches(this->GetNewPsw())) {
							//wxMessageBox(wxT("Matched"));

							if (find(it->pswds_list.begin(), it->pswds_list.end(), this->GetNewPsw()) != it->pswds_list.end()) {
								wxMessageBox(wxT("�� ��� ������������ ����� ������,\n���������� ������"));
							}
							else {
								if (it->pswds_list.size() == 10) {		// 10 - ������������ ��������, ������� ��������������� �������
									it->pswds_list.erase(it->pswds_list.begin());
									it->pswds_list.push_back(this->GetNewPsw());
								}
								else {
									it->pswds_list.push_back(this->GetNewPsw());
								}

								it->psw = this->GetNewPsw();
								wxMessageBox(wxT("�� ������� �������� ������"));
								Destroy();
							}

						}
						else {
							//wxMessageBox(wxT("Not matched!"));
							wxMessageBox(wxT("��� ������ �� ������������� ����������� �����������!\n(����������� ������ ����������, �������� ���������, ����)"));
						}
					}
					else {
						if (find(it->pswds_list.begin(), it->pswds_list.end(), this->GetNewPsw()) != it->pswds_list.end()) {
							wxMessageBox(wxT("�� ��� ������������ ����� ������,\n���������� ������"));
						}
						else {
							if (it->pswds_list.size() == 10) {		// 10 - ������������ ��������, ������� ��������������� �������
								it->pswds_list.erase(it->pswds_list.begin());
								it->pswds_list.push_back(this->GetNewPsw());
							}
							else {
								it->pswds_list.push_back(this->GetNewPsw());
							}

							it->psw = this->GetNewPsw();
							wxMessageBox(wxT("�� ������� �������� ������"));
							Destroy();
						}

					}
				}
			}
		}
	}
}