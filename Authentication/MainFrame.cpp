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

	// Меню
	wxMenuBar* menu_bar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT, wxT("Выход\tCtrl+e"));
	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT, wxT("О программе...\tCtrl+h"));
	menu_bar->Append(file, wxT("Файл"));
	menu_bar->Append(help, wxT("Справка"));

	SetMenuBar(menu_bar);

	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_username = new wxStaticText(pnl, wxID_ANY, wxT("Имя пользователя:"));
	h_box1->Add(t_username, 1, wxLEFT | wxRIGHT, 5);
	user_name = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1));
	h_box1->Add(user_name, 1, wxLEFT | wxRIGHT, 5);
	v_box->Add(h_box1, 1, wxTOP | wxBOTTOM | wxALIGN_CENTRE_HORIZONTAL, 5);

	wxStaticText* t_password = new wxStaticText(pnl, wxID_ANY, wxT("Пароль:"));
	h_box2->Add(t_password, 1, wxLEFT | wxRIGHT, 5);
	password = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box2->Add(password, 1, wxLEFT | wxRIGHT, 5);
	v_box->Add(h_box2, 1, wxTOP | wxBOTTOM | wxALIGN_CENTRE_HORIZONTAL, 5);

	wxButton* btn_ok = new wxButton(pnl, wxID_OK);
	h_box3->Add(btn_ok, 0, wxLEFT | wxRIGHT, 15);
	btn_ok->SetFocus();
	wxButton* btn_cancel = new wxButton(pnl, wxID_CANCEL, wxT("Отмена"));
	h_box3->Add(btn_cancel, 0, wxLEFT | wxRIGHT, 15);
	v_box->Add(h_box3, 1, wxTOP | wxBOTTOM | wxALIGN_RIGHT, 5);

	around_box->Add(v_box, 1, wxALL, 10);

	pnl->SetSizer(around_box);

	Centre();

	// Биндим кнопки и пункты меню
	Connect(wxID_EXIT, wxEVT_MENU, wxMenuEventHandler(MainFrame::OnMenuExit));
	Connect(wxID_ABOUT, wxEVT_MENU, wxMenuEventHandler(MainFrame::OnMenuAbout));
	Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnCancelClick));
	Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::OnOkClick));

	// Проверяем наличие файла с пользователями
	// Если файла нет, создаем новый только с данными Админа
	// Если файл есть, то заносим данные о пользователях в вектор структур

	string s_name;
	string s_psw;
	string s_isblock;
	string s_islimit;
	string s_mintime;
	string s_maxtime;

	string list_pswds;

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

			// Вектор уже использованных паролей
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
	else {	// Если не было файла, то создаем новый с АДМИНОМ
		isFirstStart = true;
		std::ofstream fout("users.txt");
		fout << "ADMIN\n";
		fout << "\n";
		fout << "\n";
		fout << "0\n";
		fout << "0\n";
		fout << "0\n";
		fout << "0";

		User user = User("ADMIN", "", 0, 0, 0, 0, {});
		start_users.push_back(user);
		fout.close();
	}

	// Файл айдита
	std::ifstream fin_aud("audit.txt");
	if (!fin_aud.is_open())
	{
		std::ofstream fout("audit.txt");
		fout << "Файл аудита:\n";
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
	wxMessageBox(wxT("Щучкин Н.Ю. \nГруппа А-13а-19 \nКурсовая работа"), wxT("О программе"));
}

void MainFrame::EnterToSystem(bool isSucces, wxString login) {
	std::ofstream fin_aud("audit.txt", std::ios_base::app);
	time_t now = time(0);
	char* dt = ctime(&now);
	if (isSucces) {
		fin_aud << "Успешный вход" << "\n";
		fin_aud << login << "\n";
		fin_aud << dt;
	}
	else {
		if (login == wxT("")) {
			fin_aud << "Неуспешный вход" << "\n";
			fin_aud << "Несуществующий логин" << "\n";
			fin_aud << dt;
		}
		else {
			fin_aud << "Неуспешный вход" << "\n";
			fin_aud << login << "\n";
			fin_aud << dt;
		}
	}
}

string wxString_to_lowercase(const wxString &str) {
	string std_str = str.ToStdString();
	std::transform(std_str.begin(), std_str.end(), std_str.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return std_str;
}

void MainFrame::OnOkClick(wxCommandEvent& event) {
	// Для сравнения логинов переведем их в нижний регистр
	string s_username = wxString_to_lowercase(user_name->GetValue());
	string s_adminname = wxString_to_lowercase(wxString(wxT("ADMIN")));

	if (s_username == s_adminname) {	// Вход администратора
		auto it = find_if(start_users.begin(), start_users.end(), [s_adminname](User& u) {
			string u_name = wxString_to_lowercase(u.name);
			return u_name == s_adminname; });
		if (it != start_users.end()) {
			if (it->psw == wxT("")) {	// Первый вход
				first_enter = new FirstEnterDlg(this);
				first_enter->userNameForSearch = it->name;
				first_enter->ShowModal();
			}
			else {		// Не первый вход
				if (password->GetValue() == it->psw) {

					EnterToSystem(true, it->name);

					admin_frame = new AdminFrame(NULL, wxID_ANY, wxT("Раздел администратора"), wxPoint(-1, -1), wxSize(500, 500));
					admin_frame->users = start_users;
					admin_frame->AdminName = it->name;
					Destroy();
					admin_frame->Show(true);
				}
				else {
					if (!num_mis) {
						EnterToSystem(false, it->name);
						wxMessageBox(wxT("Вы исчерпали все попытки"));
						Destroy();
					}
					else {
						wxMessageBox(wxString::Format(wxT("Неправильный пароль, попробуйте ещё раз\n(осталось %d попытки)"), num_mis));

						EnterToSystem(false, it->name);

						num_mis--;
					}
				}
			}
		}
	}
	else {		// Вход пользователя
		auto it = find_if(start_users.begin(), start_users.end(), [this, s_username](User& u) { 
			string u_name = wxString_to_lowercase(u.name);
			return u_name == s_username; });
		if (it == start_users.end()) {
			if (user_name->GetValue() == wxT("")) {
				wxMessageBox(wxT("Заполните поле с именем"), wxT("Ошибка при входе"));
			}
			else {
				wxMessageBox(wxT("Пользователя с таким именем не существует"), wxT("Ошибка при входе"));
				EnterToSystem(false, wxT(""));
			}
		}
		else {
			if (it->psw == wxT("")) {	// Первый вход
				first_enter = new FirstEnterDlg(this);
				first_enter->userNameForSearch = it->name;
				first_enter->ShowModal();
			}
			else {		// Не первый вход
				if (this->GetUserPsw() == it->psw) {
					if (!it->is_block) {
						if (it->is_limit) {
							wxRegEx re("^(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*[1-9]+[^.,!?:;А-я]*)*(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*)|([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*))?)$");
							if (!re.Matches(this->GetUserPsw())) {
								wxMessageBox(wxT("Ваш пароль больше не соответствует минимальным требованиям\n(чередование знаков препинания, символов кириллицы, цифр)\nУстановите новый пароль"));
								first_enter = new FirstEnterDlg(this);
								first_enter->userNameForSearch = it->name;
								first_enter->ShowModal();
							}
							else {
								user_frame = new UserFrame(NULL, wxID_ANY, wxT("Раздел пользователя"));
								user_frame->user_users = start_users;
								user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), it->name));
								user_frame->UserName = it->name;

								EnterToSystem(true, it->name);

								Destroy();
								user_frame->Show(true);
							}
						}
						else {
							user_frame = new UserFrame(NULL, wxID_ANY, wxT("Раздел пользователя"));
							user_frame->user_users = start_users;
							user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), it->name));
							user_frame->UserName = it->name;

							EnterToSystem(true, it->name);

							Destroy();
							user_frame->Show(true);
						}
					}
					else {
						// Нужно ли учитывать такие попытки входа в аудите???
						wxMessageBox(wxT("Ваш аккаунт заблокирован, обратитесь к администратору"));		
						Destroy();
					}
				}
				else {
					if (!num_mis) {

						EnterToSystem(false, it->name);

						wxMessageBox(wxT("Вы исчерпали все попытки"));
						Destroy();
					}
					else {
						wxMessageBox(wxString::Format(wxT("Неправильный пароль, попробуйте ещё раз\n(осталось %d попытки)"), num_mis));

						EnterToSystem(false, it->name);

						num_mis--;
					}
				}
			}
		}
	}
}