#include "MainFrame.h"

MainFrame::MainFrame(wxWindow* parent,
wxWindowID id,
const wxString& title,
const wxPoint& pos,
const wxSize& size,
long style,
const wxString& name) : wxFrame(parent, id, title, pos, size, style, name)
{
	num_mis = 2;

	// ����
	wxMenuBar* menu_bar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT, wxT("�����\tCtrl+e"));
	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT, wxT("� ���������...\tCtrl+h"));
	menu_bar->Append(file, wxT("����"));
	menu_bar->Append(help, wxT("�������"));

	SetMenuBar(menu_bar);

	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_username = new wxStaticText(pnl, wxID_ANY, wxT("��� ������������:"));
	h_box1->Add(t_username, 1, wxLEFT | wxRIGHT, 5);
	user_name = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1));
	h_box1->Add(user_name, 1, wxLEFT | wxRIGHT, 5);
	v_box->Add(h_box1, 1, wxTOP | wxBOTTOM | wxALIGN_CENTRE_HORIZONTAL, 5);

	wxStaticText* t_password = new wxStaticText(pnl, wxID_ANY, wxT("������:"));
	h_box2->Add(t_password, 1, wxLEFT | wxRIGHT, 5);
	password = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box2->Add(password, 1, wxLEFT | wxRIGHT, 5);
	v_box->Add(h_box2, 1, wxTOP | wxBOTTOM | wxALIGN_CENTRE_HORIZONTAL, 5);

	wxButton* btn_ok = new wxButton(pnl, wxID_OK);
	h_box3->Add(btn_ok, 0, wxLEFT | wxRIGHT, 15);
	btn_ok->SetFocus();
	wxButton* btn_cancel = new wxButton(pnl, wxID_CANCEL, wxT("������"));
	h_box3->Add(btn_cancel, 0, wxLEFT | wxRIGHT, 15);
	v_box->Add(h_box3, 1, wxTOP | wxBOTTOM | wxALIGN_RIGHT, 5);

	around_box->Add(v_box, 1, wxALL, 10);

	pnl->SetSizer(around_box);

	Centre();

	// ������ ������ � ������ ����
	Connect(wxID_EXIT, wxEVT_MENU, wxMenuEventHandler(MainFrame::OnMenuExit));
	Connect(wxID_ABOUT, wxEVT_MENU, wxMenuEventHandler(MainFrame::OnMenuAbout));
	Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnCancelClick));
	Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnOkClick));

	// ��������� ������� ����� � ��������������
	// ���� ����� ���, ������� ����� ������ � ������� ������
	// ���� ���� ����, �� ������� ������ � ������������� � ������ ��������

	string s_name;
	string s_psw;
	string s_isblock;
	string s_islimit;
	string s_mintime;
	string s_maxtime;

	string list_pswds;
	//bool b_isblock;
	//bool b_islimit;
	std::ifstream fin("users.txt");
	if (fin.is_open())
	{
		isFirstStart = false;
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
			wxString s4(s_mintime);

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

			User user = User(s_name, s_psw, wxAtoi(s1), wxAtoi(s2), wxAtoi(s3), wxAtoi(s4), vec);
			start_users.push_back(user);
		}
		fin.close();
	}
	else {	// ���� �� ���� �����, �� ������� ����� � �������
		isFirstStart = true;
		//wxMessageBox(wxT("can't open"));
		std::ofstream fout("users.txt");
		fout << "ADMIN\n";
		fout << "\n";
		fout << "\n";		// ��� ������ �������
		fout << "0\n";
		fout << "0\n";
		fout << "0\n";
		fout << "0";

		User user = User("ADMIN", "", 0, 0, 0, 0, {});
		start_users.push_back(user);
		fout.close();
	}

	// ���� ������
	std::ifstream fin_aud("audit.txt");
	if (!fin_aud.is_open())
	{
		std::ofstream fout("audit.txt");
		fout << "���� ������:\n";
		fout.close();
	}
};


MainFrame::~MainFrame() {
	std::ofstream fout("users.txt");
	for (int i = 0; i < start_users.size() - 1; ++i) {
		fout << start_users[i].name << "\n";
		fout << start_users[i].psw << "\n";
		for (int j = 0; j < start_users[i].pswds_list.size(); ++j) {
			fout << start_users[i].pswds_list[j].size() << "_";
			fout << start_users[i].pswds_list[j];
		}
		fout << "\n";
		fout << start_users[i].is_block << "\n";
		fout << start_users[i].is_limit << "\n";
		fout << start_users[i].min_pswtime << "\n";
		fout << start_users[i].max_pswtime << "\n";
	}

	fout << start_users[start_users.size() - 1].name << "\n";
	fout << start_users[start_users.size() - 1].psw << "\n";
	for (int j = 0; j < start_users[start_users.size() - 1].pswds_list.size(); ++j) {
		fout << start_users[start_users.size() - 1].pswds_list[j].size() << "_";
		fout << start_users[start_users.size() - 1].pswds_list[j];
	}
	fout << "\n";
	fout << start_users[start_users.size() - 1].is_block << "\n";
	fout << start_users[start_users.size() - 1].is_limit << "\n";
	fout << start_users[start_users.size() - 1].min_pswtime << "\n";
	fout << start_users[start_users.size() - 1].max_pswtime;

	fout.close();
}

void MainFrame::OnCancelClick(wxCommandEvent& event) {
	Close();
}

void MainFrame::OnMenuExit(wxMenuEvent& event) {
	Close();
}

void MainFrame::OnMenuAbout(wxMenuEvent& event) {
	wxMessageBox(wxT("������ �.�. \n������ �-13�-19 \n�������� ������"), wxT("� ���������"));
}

void MainFrame::EnterToSystem(bool isSucces, wxString login) {
	std::ofstream fin_aud("audit.txt", std::ios_base::app);
	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char* dt = ctime(&now);
	if (isSucces) {
		fin_aud << "�������� ����" << "\n";
		fin_aud << login << "\n";
		fin_aud << dt;

		//fin_aud << "�������� ����: " << "��� ������������:" << login << " " << "����/�����:" << dt;
	}
	else {
		if (login == wxT("")) {
			fin_aud << "���������� ����" << "\n";
			fin_aud << "�������������� �����" << "\n";
			fin_aud << dt;

			//fin_aud << "���������� ����: " << "��� ������������:" << "�������������� ����� " << "����/�����:" << dt;
		}
		else {
			fin_aud << "���������� ����" << "\n";
			fin_aud << login << "\n";
			fin_aud << dt;

			//fin_aud << "���������� ����: " << "��� ������������:" << login << " " << "����/�����:" << dt;
		}
	}
}

void MainFrame::OnOkClick(wxCommandEvent& event) {
	if (user_name->GetValue() == wxT("ADMIN")) {	// ���� ��������������

		auto it = find_if(start_users.begin(), start_users.end(), [](User& u) { return u.name == wxT("ADMIN"); });
		if (it != start_users.end()) {
			if (it->psw == wxT("")) {	// ������ ����
				first_enter = new FirstEnterDlg(this);
				first_enter->userNameForSearch = wxT("ADMIN");
				first_enter->ShowModal();
			}
			else {		// �� ������ ����
				if (password->GetValue() == it->psw) {

					EnterToSystem(true, wxT("ADMIN"));

					admin_frame = new AdminFrame(NULL, wxID_ANY, wxT("������ ��������������"), wxPoint(-1, -1), wxSize(500, 500));
					admin_frame->users = start_users;
					Destroy();
					admin_frame->Show(true);
				}
				else {
					if (!num_mis) {
						EnterToSystem(false, wxT("ADMIN"));
						wxMessageBox(wxT("�� ��������� ��� �������"));
						Destroy();
					}
					else {
						wxMessageBox(wxString::Format(wxT("������������ ������, ���������� ��� ���\n(�������� %d �������)"), num_mis));

						EnterToSystem(false, wxT("ADMIN"));

						num_mis--;
					}
				}
			}
		}
	}
	else {		// ���� ������������
		auto it = find_if(start_users.begin(), start_users.end(), [this](User& u) { return u.name == this->GetUserName(); });
		if (it == start_users.end()) {
			if (user_name->GetValue() == wxT("")) {
				wxMessageBox(wxT("��������� ���� � ������"), wxT("������ ��� �����"));
			}
			else {
				wxMessageBox(wxT("������������ � ����� ������ �� ����������"), wxT("������ ��� �����"));

				EnterToSystem(false, wxT(""));

			}
		}
		else {
			if (it->psw == wxT("")) {	// ������ ����
				first_enter = new FirstEnterDlg(this);
				first_enter->userNameForSearch = this->GetUserName();
				first_enter->ShowModal();
			}
			else {		// �� ������ ����
				if (this->GetUserPsw() == it->psw) {
					if (!it->is_block) {
						if (it->is_limit) {
							wxRegEx re("^(([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*[1-9]+[^.,!?:;�-�]*)*(([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*[�-�]+[^.,!?:;1-9]*)|([^.,!?:;�-�1-9]*[.,!?:;]+[^�-�1-9]*))?)$");
							if (!re.Matches(this->GetUserPsw())) {
								wxMessageBox(wxT("��� ������ ������ �� ������������� ����������� �����������\n(����������� ������ ����������, �������� ���������, ����)\n���������� ����� ������"));
								first_enter = new FirstEnterDlg(this);
								first_enter->userNameForSearch = this->GetUserName();
								first_enter->ShowModal();
							}
							else {
								user_frame = new UserFrame(NULL, wxID_ANY, wxT("������ ������������"));
								user_frame->user_users = start_users;
								user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), this->GetUserName()));
								user_frame->UserName = this->GetUserName();

								EnterToSystem(true, this->GetUserName());

								Destroy();
								user_frame->Show(true);
							}
						}
						else {
							user_frame = new UserFrame(NULL, wxID_ANY, wxT("������ ������������"));
							user_frame->user_users = start_users;
							user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), this->GetUserName()));
							user_frame->UserName = this->GetUserName();

							EnterToSystem(true, this->GetUserName());

							Destroy();
							user_frame->Show(true);
						}
					}
					else {
						wxMessageBox(wxT("��� ������� ������������, ���������� � ��������������"));
						Destroy();
					}
				}
				else {
					if (!num_mis) {

						EnterToSystem(false, this->GetUserName());

						wxMessageBox(wxT("�� ��������� ��� �������"));
						Destroy();
					}
					else {
						wxMessageBox(wxString::Format(wxT("������������ ������, ���������� ��� ���\n(�������� %d �������)"), num_mis));

						EnterToSystem(false, this->GetUserName());

						num_mis--;
					}
				}
			}
		}
	}
}