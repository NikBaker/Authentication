#include "UserFrame.h"

UserFrame::UserFrame(wxWindow* parent,
	wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) : wxFrame(parent, id, title, pos, size, style, name)
{
	// Меню
	wxMenuBar* menu_bar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT, wxT("Выход...\tCtrl+e"));
	wxMenu* admin = new wxMenu;
	admin->Append(ID_CHANGE_US, wxT("Смена пароля...\tCtrl+n"));
	admin->Append(ID_US_LOGOUT, wxT("Выход из учетной записи"));
	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT, wxT("О программе...\tCtrl+h"));
	menu_bar->Append(file, wxT("Файл"));
	menu_bar->Append(admin, wxT("Пользователь"));
	menu_bar->Append(help, wxT("Справка"));

	SetMenuBar(menu_bar);

	UserName = wxT("");
	wxPanel* pnl = new wxPanel(this, -1);
	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	hello = new wxStaticText(pnl, wxID_ANY, wxString::Format(wxT("Hello, %s!"), UserName));
	v_box->Add(hello, 1, wxEXPAND | wxALL, 15);

	pnl->SetSizer(v_box);
	Centre();

	Connect(wxID_ABOUT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnMenuAbout));
	Connect(wxID_EXIT, wxEVT_MENU, wxMenuEventHandler(UserFrame::OnMenuExit));
	Connect(ID_CHANGE_US, wxEVT_MENU, wxMenuEventHandler(UserFrame::OnChangePsw));
	Connect(ID_US_LOGOUT, wxEVT_MENU, wxMenuEventHandler(UserFrame::OnLogOut));
	Connect(wxEVT_CLOSE_WINDOW, wxCommandEventHandler(UserFrame::OnClose));
}

void UserFrame::OnChangePsw(wxMenuEvent& event) {
	change_dlg = new ChangeUsPswDlg(this);
	change_dlg->user_vec = user_users;
	change_dlg->from_maxtime = false;
	change_dlg->changepsw_name = UserName;
	change_dlg->ShowModal();
}

void UserFrame::ExitFromSystem(wxString login) {
	std::ofstream fin_aud(user_audfile_1, std::ios_base::app);
	//std::ofstream fin_aud("audit.eaud", std::ios_base::app);
	time_t now = time(0);
	char* dt = ctime(&now);

	fin_aud << "Успешный выход" << "\n";
	fin_aud << login << "\n";
	fin_aud << dt;
}

void UserFrame::OnLogOut(wxMenuEvent& event) {
	ExitFromSystem(UserName);

	MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, wxT("Вход"), wxPoint(-1, -1), wxSize(370, 225));
	mainFrame->start_users = user_users;
	Destroy();
	mainFrame->Show(true);
}

void UserFrame::OnMenuExit(wxMenuEvent& event) {	// При закрытии программы разлогиниваем пользователя?
	ExitFromSystem(UserName);
	Destroy();
}

void UserFrame::OnClose(wxCommandEvent& event) {	// При закрытии программы разлогиниваем пользователя?
	ExitFromSystem(UserName);
	event.Skip();
	//Close();
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
		fout << user_users[i].num_of_pswds << "\n";
		fout << user_users[i].last_changepsw << "\n";
		fout << user_users[i].filename_1 << "\n";
		fout << user_users[i].filename_2 << "\n";
		fout << user_users[i].num_of_audit_records << "\n";

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
	fout << user_users[user_users.size() - 1].max_pswtime << "\n";
	fout << user_users[user_users.size() - 1].num_of_pswds << "\n";
	fout << user_users[user_users.size() - 1].last_changepsw << "\n";
	fout << user_users[user_users.size() - 1].filename_1 << "\n";
	fout << user_users[user_users.size() - 1].filename_2 << "\n";
	fout << user_users[user_users.size() - 1].num_of_audit_records;

	fout.close();
}

ChangeUsPswDlg::ChangeUsPswDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("Смена пароля"), wxPoint(-1, -1), wxSize(400, 225))
{
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* h_box = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* v_box1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* v_box2 = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_old = new wxStaticText(pnl, wxID_ANY, wxT("Старый пароль:"));
	wxStaticText* t_new = new wxStaticText(pnl, wxID_ANY, wxT("Новый пароль:"));
	wxStaticText* t_apply = new wxStaticText(pnl, wxID_ANY, wxT("Подтвердить пароль:"));
	v_box1->Add(t_old, 0, wxALIGN_LEFT | wxBOTTOM, 25);
	v_box1->Add(t_new, 0, wxALIGN_LEFT | wxBOTTOM, 25);
	v_box1->Add(t_apply, 0, wxALIGN_LEFT | wxBOTTOM, 18);
	wxButton* btn_ok = new wxButton(pnl, ID_CHANGE_US_PSW, wxT("Ок"));
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
		wxMessageBox(wxT("Все поля должны быть заполнены"), wxT("Ошибка при вводе пароля"));
	}
	else {
		wxString oldStr = GetOldPsw();
		//UserFrame* p_wnd = (UserFrame*)GetParent();
		//auto it = find_if(p_wnd->user_users.begin(), p_wnd->user_users.end(), [p_wnd](User& u) { return u.name == p_wnd->UserName; });
		//if (it != p_wnd->user_users.end()) {
		auto it = find_if(user_vec.begin(), user_vec.end(), [this](User& u) { return u.name == changepsw_name; });
		if (it != user_vec.end()) {
			if (it->psw != oldStr) {
				wxMessageBox(wxT("Неправильно введен старый пароль"), wxT("Ошибка при вводе пароля"));
			}
			else {
				if (this->GetNewPsw() != this->GetConfirmPsw()) {
					wxMessageBox(wxT("Пароли не совпадают"), wxT("Ошибка при вводе пароля"));
				}
				else {
					if (it->is_limit) {		// Проверяем новый пароль регулярным выражением
						wxRegEx re("^(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*[1-9]+[^.,!?:;А-я]*)*(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*)|([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*))?)$");
						if (re.IsValid()) {
							//wxMessageBox(wxT("Compiled!"));
						}
						if (re.Matches(this->GetNewPsw())) {
							//wxMessageBox(wxT("Matched"));

							if (find(it->pswds_list.begin(), it->pswds_list.end(), this->GetNewPsw()) != it->pswds_list.end()) {
								wxMessageBox(wxT("Вы уже использовали такой пароль,\nпопробуйте другой"));
							}
							else {
								if (it->min_pswtime == 0) {
									if (it->pswds_list.size() == it->num_of_pswds) {		// it->num_of_pswds - максимальное значение, которое устанавливается админом
										it->pswds_list.erase(it->pswds_list.begin());
										it->pswds_list.push_back(this->GetNewPsw());
									}
									else {
										it->pswds_list.push_back(this->GetNewPsw());
									}

									it->psw = this->GetNewPsw();
									wxMessageBox(wxT("Вы успешно изменили пароль"));
									it->last_changepsw = time(0);

									if (from_maxtime) {
										MainFrame* p_wnd = (MainFrame*)GetParent();
										p_wnd->start_users = user_vec;
									}
									else {
										UserFrame* p_wnd = (UserFrame*)GetParent();
										p_wnd->user_users = user_vec;
									}
								}
								else {
									if (time(0) - it->last_changepsw < it->min_pswtime) {
										wxMessageBox(wxT("Ещё не прошло достаточно времени с момента прошлой смены пароля,\nпопробуйте позднее"), wxT("Ошибка при вводе пароля"));
									}
									else {
										if (it->pswds_list.size() == it->num_of_pswds) {		// it->num_of_pswds - максимальное значение, которое устанавливается админом
											it->pswds_list.erase(it->pswds_list.begin());
											it->pswds_list.push_back(this->GetNewPsw());
										}
										else {
											it->pswds_list.push_back(this->GetNewPsw());
										}

										it->psw = this->GetNewPsw();
										wxMessageBox(wxT("Вы успешно изменили пароль"));
										it->last_changepsw = time(0);

										if (from_maxtime) {
											MainFrame* p_wnd = (MainFrame*)GetParent();
											p_wnd->start_users = user_vec;
										}
										else {
											UserFrame* p_wnd = (UserFrame*)GetParent();
											p_wnd->user_users = user_vec;
										}
									}
								}
								
								Destroy();
							}

						}
						else {
							//wxMessageBox(wxT("Not matched!"));
							wxMessageBox(wxT("Ваш пароль не соответствует минимальным требованиям!\n(чередование знаков препинания, символов кириллицы, цифр)"));
						}
					}
					else {
						if (find(it->pswds_list.begin(), it->pswds_list.end(), this->GetNewPsw()) != it->pswds_list.end()) {
							wxMessageBox(wxT("Вы уже использовали такой пароль,\nпопробуйте другой"));
							//wxMessageBox(wxT("Вы уже недавно изменяли пароль,\nпопробуйте позже"));	//
							//wxMessageBox(wxT("Пришло время сменить пароль,\nВы будете перенаправлены на страницу смены пароля"));	//
						}
						else {
							if (it->min_pswtime == 0) {
								if (it->pswds_list.size() == it->num_of_pswds) {		// it->num_of_pswds - максимальное значение, которое устанавливается админом
									it->pswds_list.erase(it->pswds_list.begin());
									it->pswds_list.push_back(this->GetNewPsw());
								}
								else {
									it->pswds_list.push_back(this->GetNewPsw());
								}

								it->psw = this->GetNewPsw();
								wxMessageBox(wxT("Вы успешно изменили пароль"));
								it->last_changepsw = time(0);

								if (from_maxtime) {
									MainFrame* p_wnd = (MainFrame*)GetParent();
									p_wnd->start_users = user_vec;
									int temp = 0;				///
								}
								else {
									UserFrame* p_wnd = (UserFrame*)GetParent();
									p_wnd->user_users = user_vec;
								}
							}
							else {
								if (time(0) - it->last_changepsw < it->min_pswtime) {
									wxMessageBox(wxT("Ещё не прошло достаточно времени с момента прошлой смены пароля,\nпопробуйте позднее"), wxT("Ошибка при вводе пароля"));
								}
								else {
									if (it->pswds_list.size() == it->num_of_pswds) {		// it->num_of_pswds - максимальное значение, которое устанавливается админом
										it->pswds_list.erase(it->pswds_list.begin());
										it->pswds_list.push_back(this->GetNewPsw());
									}
									else {
										it->pswds_list.push_back(this->GetNewPsw());
									}

									it->psw = this->GetNewPsw();
									wxMessageBox(wxT("Вы успешно изменили пароль"));
									it->last_changepsw = time(0);

									if (from_maxtime) {
										MainFrame* p_wnd = (MainFrame*)GetParent();
										p_wnd->start_users = user_vec;
										
									}
									else {
										UserFrame* p_wnd = (UserFrame*)GetParent();
										p_wnd->user_users = user_vec;
									}
								}
							}
							
							Destroy();
						}
					}
				}
			}
		}
	}
}