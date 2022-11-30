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

	string s_num_of_pswds;
	string s_last_changepsw;

	string s_filename_1;
	string s_filename_2;

	string s_num_of_audit_records;

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
			getline(fin, s_num_of_pswds);
			getline(fin, s_last_changepsw);
			getline(fin, s_filename_1);
			getline(fin, s_filename_2);
			getline(fin, s_num_of_audit_records);

			wxString s1(s_isblock);
			wxString s2(s_islimit);
			wxString s3(s_mintime);
			wxString s4(s_maxtime);
			wxString s5(s_num_of_pswds);
			wxString s6(s_last_changepsw);
			wxString s7(s_num_of_audit_records);

			// Вектор уже использованных паролей
			vector<wxString> vec;

			// В векторе будут шифрованные пароли:
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

			User user = User(s_name, s_psw, wxAtoi(s1), wxAtoi(s2), wxAtoi(s3), wxAtoi(s4), vec, wxAtoi(s5), wxAtoi(s6), s_filename_1, s_filename_2, wxAtoi(s7));
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
		fout << "0\n";
		fout << "1\n";
		fout << "0\n";
		fout << "\n";
		fout << "\n";
		fout << "5000";

		User user = User("ADMIN", "", 0, 0, 0, 0, {}, 1, 0, "", "", 5000);
		start_users.push_back(user);
		fout.close();
	}

	if (start_users[0].filename_1 != "") {
		
		string temp = "";
		for (int i = start_users[0].filename_1.size() - 1; i >= 0; --i) {
			temp.push_back(start_users[0].filename_1[i]);
		}
		if (temp.substr(0, 5) != "duae.") {
			start_users[0].filename_1 = start_users[0].filename_1 + ".eaud";
		}
		
		// Файл айдита входа
		std::ifstream fin_aud(start_users[0].filename_1);
		if (!fin_aud.is_open())
		{
			std::ofstream fout(start_users[0].filename_1);
			fout << "Файл аудита:\n";
			fout.close();
		}
	}

	if (start_users[0].filename_2 != "") {

		string temp = "";
		for (int i = start_users[0].filename_2.size() - 1; i >= 0; --i) {
			temp.push_back(start_users[0].filename_2[i]);
		}
		if (temp.substr(0, 5) != "duac.") {
			start_users[0].filename_2 = start_users[0].filename_2 + ".caud";
		}

		// Файл айдита изменений
		std::ifstream fin_aud(start_users[0].filename_2);
		if (!fin_aud.is_open())
		{
			std::ofstream fout(start_users[0].filename_2);
			fout << "Файл аудита:\n";
			fout.close();
		}
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
		fout << start_users[i].num_of_pswds << "\n";
		fout << start_users[i].last_changepsw << "\n";
		fout << start_users[i].filename_1 << "\n";
		fout << start_users[i].filename_2 << "\n";
		fout << start_users[i].num_of_audit_records << "\n";
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
	fout << start_users[start_users.size() - 1].max_pswtime << "\n";
	fout << start_users[start_users.size() - 1].num_of_pswds << "\n";
	fout << start_users[start_users.size() - 1].last_changepsw << "\n";
	fout << start_users[start_users.size() - 1].filename_1 << "\n";
	fout << start_users[start_users.size() - 1].filename_2 << "\n";
	fout << start_users[start_users.size() - 1].num_of_audit_records;

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
///////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainFrame::Decode(wxString str1, wxString str2, char b) {
	// Первый этап расшифровки:
	string s1 = str1.ToStdString();
	string s2 = str2.ToStdString();

	string decode_1;

	string second_secret_key;
	second_secret_key.push_back(123);       // a = 123 - взаимно простое число с 65536
	second_secret_key.push_back(b);       // b = код первого символа в логине 

	// Расшифровываем запись с шифром пароля из файла с пользователями
	for (int i = 0; i < s2.size(); ++i) {
		decode_1.push_back((57011 * (s2[i] + 65536 - b)) % 65536);           // 57011 - число обратное к 123 по модулю 65536
	}

	// Второй этап расшифровки:
	int sum = 0;
	// формируем ключ:
	for (int i = 0; i < str1.size(); i += 2) {
		sum += (int)str1[i];
	}
	srand(sum); // рандомизация генератора случайных чисел
	string secret_key2;
	for (int i = 0; i < 5; ++i) {
		secret_key2.push_back(rand() % 256);
	}

	string result_psw;

	// расшифровываем decode_1 при помощи полученного ключа:
	// Если длина ключа меньше длины шифротекста, то дополняем оставшиеся позиции повторением ключа
	if (secret_key2.size() < decode_1.size()) {
		for (int i = 0; i < decode_1.size(); ++i) {
			result_psw.push_back(decode_1[i] ^ secret_key2[i % 5]);
		}
	}
	else {
		for (int i = 0; i < decode_1.size(); ++i) {
			result_psw.push_back(decode_1[i] ^ secret_key2[i]);
		}
	}

	return result_psw;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainFrame::EnterToSystem(bool isSucces, wxString login) {
	std::ofstream fin_aud(start_users[0].filename_1, std::ios_base::app);
	//std::ofstream fin_aud("audit.eaud", std::ios_base::app);
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
	// Открываем файл аудита входа, если он задан

	
	// Для сравнения логинов переведем их в нижний регистр
	string s_username = wxString_to_lowercase(user_name->GetValue());
	string s_adminname = wxString_to_lowercase(wxString(wxT("ADMIN")));

	if (s_username == s_adminname) {	// Вход администратора
		auto it = find_if(start_users.begin(), start_users.end(), [s_adminname](User& u) {
			string u_name = wxString_to_lowercase(u.name);
			return u_name == s_adminname; });
		if (it != start_users.end()) {
			if (it->psw == wxT("")) {	// Первый вход
				first_adm_enter = new FirstAdminEnterDlg(this);
				first_adm_enter->userNameForSearch = it->name;
				first_adm_enter->ShowModal();
				//first_enter = new FirstEnterDlg(this);
				//first_enter->userNameForSearch = it->name;
				//first_enter->ShowModal();

				// После первого входа админа, мы имеем имена файлов
				if (start_users[0].filename_1 != "") {
					start_users[0].filename_1 = start_users[0].filename_1 + ".eaud";
					std::ifstream fin_aud(start_users[0].filename_1);
					if (!fin_aud.is_open())
					{
						std::ofstream fout(start_users[0].filename_1);
						fout << "Файл аудита:\n";
						fout.close();
					}
				}
				if (start_users[0].filename_2 != "") {
					start_users[0].filename_2 = start_users[0].filename_2 + ".caud";
					std::ifstream fin_aud(start_users[0].filename_2);
					if (!fin_aud.is_open())
					{
						std::ofstream fout(start_users[0].filename_2);
						fout << "Файл аудита:\n";
						fout.close();
					}
				}

			}
			else {		// Не первый вход
				// Расшифровываем пароль:
				string s = Decode(password->GetValue(), it->psw, (s_adminname)[0]);
				//string s = Decode(password->GetValue(), it->psw, (user_name->GetValue())[0]);
				
				if (password->GetValue() == s) {		// Проверка с шифром
				//if (password->GetValue() == it->psw) {
					// Если файл аудита переполнен(кол-во строк >= 3*n + 1, n - макс кол-во записей, которое задает админ), 
					// Админ принудительно очищает файл аудита
					int number_of_lines = 0;
					std::string line;
					std::ifstream myfile(start_users[0].filename_1);
					while (std::getline(myfile, line)){
						++number_of_lines;
					}
					if (number_of_lines >= 3 * start_users[0].num_of_audit_records + 1) {
						wxMessageBox(wxT("Файл аудита входа переполнен.\nОчистите его перед продолжением работы.\nПеред очисткой можно сохранить файл аудита."), wxT("Файл аудита переполнен"));

						admin_frame = new AdminFrame(NULL, wxID_ANY, wxT("Раздел администратора"), wxPoint(-1, -1), wxSize(700, 500));
						admin_frame->users = start_users;
						admin_frame->AdminName = it->name;
						admin_frame->audfile_1 = start_users[0].filename_1;
						admin_frame->audfile_2 = start_users[0].filename_2;
						admin_frame->Show(true);
						admin_frame->auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит удачных и неудачных попыток входа"), admin_frame->audfile_1, true);
						admin_frame->auditop_dlg->ShowModal();
						Destroy();
					}
					else {
						EnterToSystem(true, it->name);

						admin_frame = new AdminFrame(NULL, wxID_ANY, wxT("Раздел администратора"), wxPoint(-1, -1), wxSize(700, 500));
						admin_frame->users = start_users;
						admin_frame->AdminName = it->name;
						admin_frame->audfile_1 = start_users[0].filename_1;
						admin_frame->audfile_2 = start_users[0].filename_2;
						Destroy();
						admin_frame->Show(true);
					}
											
				}
				else {
					if (!num_mis) {
						// Если файл аудита переполнен(кол-во строк >= 3*n + 1, n - макс кол-во записей, которое задает админ), 
						// Данная запись не попадет в файл аудита
						int number_of_lines = 0;
						std::string line;
						std::ifstream myfile(start_users[0].filename_1);
						while (std::getline(myfile, line)) {
							++number_of_lines;
						}
						if (number_of_lines >= 3 * start_users[0].num_of_audit_records + 1) {
							wxMessageBox(wxT("Вы исчерпали все попытки"));
							Destroy();
						}
						else {
							EnterToSystem(false, it->name);
							wxMessageBox(wxT("Вы исчерпали все попытки"));
							Destroy();
						}
					}
					else {
						// Если файл аудита переполнен(кол-во строк >= 3*n + 1, n - макс кол-во записей, которое задает админ), 
						// Данная запись не попадет в файл аудита
						int number_of_lines = 0;
						std::string line;
						std::ifstream myfile(start_users[0].filename_1);
						while (std::getline(myfile, line)) {
							++number_of_lines;
						}
						if (number_of_lines >= 3 * start_users[0].num_of_audit_records + 1) {
							wxMessageBox(wxString::Format(wxT("Неправильный пароль, попробуйте ещё раз\n(осталось %d попытки)"), num_mis));
							num_mis--;
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
	else {		// Вход пользователя
		// Если файл аудита переполнен(кол-во строк >= 3*n + 1, n - макс кол-во записей, которое задает админ), 
		// Пользователи не могут войти в систему
		int number_of_lines = 0;
		std::string line;
		std::ifstream myfile(start_users[0].filename_1);
		while (std::getline(myfile, line)) {
			++number_of_lines;
		}
		if (number_of_lines >= 3 * start_users[0].num_of_audit_records + 1) {
			wxMessageBox(wxT("В данный момент вы не можете войти в систему\n(проблемы с файлами аудита)\nОбратитесь к администратору"), wxT("Ошибка при входе"));
		}
		else {
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
					// Расшифровываем пароль:
					string s = Decode(this->GetUserPsw(), it->psw, s_username[0]);
					//string s = Decode(this->GetUserPsw(), it->psw, (this->GetUserName())[0]);
					if (this->GetUserPsw() == s) {		// Проверка с шифром
					//if (this->GetUserPsw() == it->psw) {
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
									if (it->max_pswtime == 0) {
										user_frame = new UserFrame(NULL, wxID_ANY, wxT("Раздел пользователя"));
										user_frame->user_users = start_users;
										user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), it->name));
										user_frame->UserName = it->name;

										user_frame->user_audfile_1 = start_users[0].filename_1;
										user_frame->user_audfile_2 = start_users[0].filename_2;

										EnterToSystem(true, it->name);
										Destroy();
										user_frame->Show(true);
									}
									else {
										if (time(0) - it->last_changepsw > it->max_pswtime) {
											wxMessageBox(wxT("Прошло слишком много времени с момента последей смены пароля,\nпожалуйста, смените пароль."), wxT("Ошибка при входе"));

											ChangeUsPswDlg* change_dlg = new ChangeUsPswDlg(this);
											change_dlg->user_vec = start_users;
											change_dlg->from_maxtime = true;
											change_dlg->changepsw_name = it->name;
											change_dlg->ShowModal();
										}
										else {
											user_frame = new UserFrame(NULL, wxID_ANY, wxT("Раздел пользователя"));
											user_frame->user_users = start_users;
											user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), it->name));
											user_frame->UserName = it->name;
											user_frame->user_audfile_1 = start_users[0].filename_1;
											user_frame->user_audfile_2 = start_users[0].filename_2;

											EnterToSystem(true, it->name);
											Destroy();
											user_frame->Show(true);
										}
									}
								}
							}
							else {
								if (it->max_pswtime == 0) {
									user_frame = new UserFrame(NULL, wxID_ANY, wxT("Раздел пользователя"));
									user_frame->user_users = start_users;
									user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), it->name));
									user_frame->UserName = it->name;
									user_frame->user_audfile_1 = start_users[0].filename_1;
									user_frame->user_audfile_2 = start_users[0].filename_2;

									EnterToSystem(true, it->name);
									Destroy();
									user_frame->Show(true);
								}
								else {
									if (time(0) - it->last_changepsw > it->max_pswtime) {
										wxMessageBox(wxT("Прошло слишком много времени с момента последей смены пароля,\nпожалуйста, смените пароль."), wxT("Ошибка при входе"));

										ChangeUsPswDlg* change_dlg = new ChangeUsPswDlg(this);
										change_dlg->user_vec = start_users;
										change_dlg->from_maxtime = true;
										change_dlg->changepsw_name = it->name;
										change_dlg->ShowModal();
									}
									else {
										user_frame = new UserFrame(NULL, wxID_ANY, wxT("Раздел пользователя"));
										user_frame->user_users = start_users;
										user_frame->hello->SetLabel(wxString::Format(wxT("Hello, %s!"), it->name));
										user_frame->UserName = it->name;
										user_frame->user_audfile_1 = start_users[0].filename_1;
										user_frame->user_audfile_2 = start_users[0].filename_2;

										EnterToSystem(true, it->name);
										Destroy();
										user_frame->Show(true);
									}
								}
							}
						}
						else {
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

	// Закрываем файл аудита входа



}