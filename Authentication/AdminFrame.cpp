#include "AdminFrame.h"
#include "wx/spinctrl.h"

AdminFrame::AdminFrame(wxWindow* parent,
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
	admin->Append(ID_CHANGE, wxT("Смена пароля...\tCtrl+n"));
	admin->Append(ID_AUDIT_OPERATIONS, wxT("Аудит изменений в файле учетных записей"));
	admin->Append(ID_AUDIT, wxT("Аудит удачных и неудачных попыток входа"));
	admin->Append(ID_LOGOUT, wxT("Выход из учетной записи"));
	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT, wxT("О программе...\tCtrl+h"));
	menu_bar->Append(file, wxT("Файл"));
	menu_bar->Append(admin, wxT("Админ"));
	menu_bar->Append(help, wxT("Справка"));

	SetMenuBar(menu_bar);

	wxPanel* pnl = new wxPanel(this, -1);
	wxBoxSizer* h_box = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);

	list = new wxListBox(pnl, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1)/*, arr*/);
	h_box->Add(list, 1, wxEXPAND | wxRIGHT | wxLEFT, 15);

	wxButton* add_new = new wxButton(pnl, wxID_ADD, wxT("Добавить пользователя"));
	v_box->Add(add_new, 0, wxEXPAND | wxBOTTOM, 15);
	block = new wxCheckBox(pnl, wxID_ANY, wxT("блокировка"));
	v_box->Add(block, 0, wxEXPAND | wxBOTTOM, 15);
	block->Enable(false);
	limit = new wxCheckBox(pnl, wxID_ANY, wxT("ограничение \nна пароль"));
	v_box->Add(limit, 0, wxEXPAND | wxBOTTOM, 15);
	limit->Enable(false);
	wxButton* set_minmax = new wxButton(pnl, ID_MINMAX, wxT("Установить\nмин, макс\nсрок действия пароля"));
	v_box->Add(set_minmax, 0, wxEXPAND | wxBOTTOM, 15);

	wxStaticText* length_psws = new wxStaticText(pnl, wxID_ANY, wxT("Длина списка использованных паролей:"));
	spin = new wxSpinCtrl(pnl, wxID_ANY);
	spin->SetRange(1, 1000);
	wxBoxSizer* helph_box = new wxBoxSizer(wxHORIZONTAL);
	helph_box->Add(spin, 0, wxALL, 5);
	spin_btn = new wxButton(pnl, wxID_ANY, wxT("Установить"));
	helph_box->Add(spin_btn, 0, wxALL, 5);
	v_box->Add(length_psws, 0, /*wxEXPAND | */wxBOTTOM, 5);
	v_box->Add(helph_box, 0, /*wxEXPAND | */wxBOTTOM, 15);

	wxStaticText* length_audit = new wxStaticText(pnl, wxID_ANY, wxT("Максимальное кол-во записей в файлах аудита:"));
	spin_length_audit = new wxSpinCtrl(pnl, wxID_ANY);
	spin_length_audit->SetRange(1, 100000);
	wxBoxSizer* helph_box2 = new wxBoxSizer(wxHORIZONTAL);
	helph_box2->Add(spin_length_audit, 0, wxALL, 5);
	spin_length_audit_btn = new wxButton(pnl, wxID_ANY, wxT("Установить"));
	helph_box2->Add(spin_length_audit_btn, 0, wxALL, 5);
	v_box->Add(length_audit, 0, /*wxEXPAND | */wxBOTTOM, 5);
	v_box->Add(helph_box2, 0, /*wxEXPAND | */wxBOTTOM, 15);
	
	//
	h_box->Add(v_box, 1, wxEXPAND | wxRIGHT | wxLEFT, 15);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	around_box->Add(h_box, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	pnl->SetSizer(around_box);
	Centre();

	Connect(wxID_ABOUT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnMenuAbout));
	Connect(wxID_EXIT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnMenuExit));
	Connect(ID_CHANGE, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnChangePsw));
	Connect(ID_AUDIT_OPERATIONS, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnAuditOperations));
	Connect(ID_AUDIT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnAudit));
	Connect(ID_LOGOUT, wxEVT_MENU, wxMenuEventHandler(AdminFrame::OnLogOut));
	Connect(wxEVT_CLOSE_WINDOW, wxCommandEventHandler(AdminFrame::OnClose));
	Connect(wxID_ADD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AdminFrame::OnAddNew));
	Connect(block->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(AdminFrame::OnChange_block));
	Connect(limit->GetId(), wxEVT_CHECKBOX, wxCommandEventHandler(AdminFrame::OnChange_limit));
	Connect(spin_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AdminFrame::OnChange_spin));
	Connect(spin_length_audit_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AdminFrame::OnChange_auditspin));
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
	string s_num_of_pswds;
	string s_last_changepsw;

	string s_filename_1;
	string s_filename_2;

	string s_num_of_audit_records;

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

			User user = User(s_name, s_psw, wxAtoi(s1), wxAtoi(s2), wxAtoi(s3), wxAtoi(s4), vec, wxAtoi(s5), wxAtoi(s6), s_filename_1, s_filename_2, wxAtoi(s7));
			users.push_back(user);
		}
		fin.close();
	}

	for (int i = 0; i < users.size(); ++i) {
		list->Append(wxString(users[i].name));
	}

	spin->SetValue(users[0].num_of_pswds);
	spin_length_audit->SetValue(users[0].num_of_audit_records);
}

void AdminFrame::OnMenuExit(wxMenuEvent& event) {		
	ExitFromSystem(AdminName);
	Destroy();
}

void AdminFrame::OnClose(wxCommandEvent& event) {		
	ExitFromSystem(AdminName);
	event.Skip();
}

void AdminFrame::OnMenuAbout(wxMenuEvent& event) {
	wxMessageBox(wxT("Щучкин Н.Ю. \nГруппа А-13а-19 \nКурсовая работа"), wxT("О программе"));
}

void AdminFrame::OnChangePsw(wxMenuEvent& event) {
	change_dlg = new ChangePswDlg(this);
	change_dlg->ShowModal();
}

void AdminFrame::OnAuditOperations(wxMenuEvent& event) {
	auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит изменений в файле учетных записей"), audfile_2);
	auditop_dlg->ShowModal();
}

void AdminFrame::OnAudit(wxMenuEvent& event) {
	auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит удачных и неудачных попыток входа"), audfile_1, true);
	auditop_dlg->ShowModal();
}

void AdminFrame::CnangeUserData(wxString operation, wxString username) {
	
	std::ofstream fin_aud(audfile_2, std::ios_base::app);
	time_t now = time(0);
	char* dt = ctime(&now);
	fin_aud << operation << "\n" << username << "\n" << dt;
}

void AdminFrame::ExitFromSystem(wxString login) {
	std::ofstream fin_aud(audfile_1, std::ios_base::app);
	time_t now = time(0);
	char* dt = ctime(&now);

	fin_aud << "Успешный выход" << "\n";
	fin_aud << login << "\n";
	fin_aud << dt;
}

void AdminFrame::OnLogOut(wxMenuEvent& event) {
	ExitFromSystem(AdminName);

	MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, wxT("Вход"), wxPoint(-1, -1), wxSize(370, 225));
	mainFrame->start_users = users;
	Destroy();
	mainFrame->Show(true);
}

void AdminFrame::OnAddNew(wxCommandEvent& event) {
	addnew_dlg = new AddNewUserDlg();
	if (addnew_dlg->ShowModal() == wxID_OK) {
		if (addnew_dlg->new_username->GetValue() == "") {
			wxMessageBox(wxT("Поле с именем пользователя дожно быть заполнено!"), wxT("Ошибка при добавлении нового пользователя"));
			addnew_dlg->Destroy();
		}
		else {
			string str = wxString_to_lowercase(addnew_dlg->new_username->GetValue());
			auto it = find_if(users.begin(), users.end(), [str](User& u) {
				string uname = wxString_to_lowercase(u.name);
				return uname == str; });
			if (it != users.end()) {	// Если пользователь с таким именем уже есть
				wxMessageBox(wxT("Пользователь с таким именем уже существует в системе!"));
			}
			else {	// Если такого пользователя еще нет, то добавляем данные по новому пользователю в вектор и ListBox
				// Если файл аудита переполнен(кол-во строк >= 3*n + 1, n - макс кол-во записей, которое задает админ), 
				// Админ принудительно очищает файл аудита
				int number_of_lines = 0;
				std::string line;
				std::ifstream myfile(audfile_2);
				while (std::getline(myfile, line)) {
					++number_of_lines;
				}
				if (number_of_lines >= 3 * users[0].num_of_audit_records + 1) {
					wxMessageBox(wxT("Файл аудита изменений переполнен.\nОчистите его перед продолжением работы.\nПеред очисткой можно сохранить файл аудита."), wxT("Файл аудита переполнен"));
					auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит изменений в файле учетных записей"), audfile_2);
					auditop_dlg->ShowModal();

					//
					addnew_dlg->Destroy();
					this->Destroy();
				}
				else {
					User u(addnew_dlg->new_username->GetValue(), wxT(""), addnew_dlg->block->GetValue(), addnew_dlg->limit->GetValue(), users[0].min_pswtime, users[0].max_pswtime, {}, users[0].num_of_pswds, 0, "", "", 5000);
					users.push_back(u);

					CnangeUserData(wxT("Добавление нового пользователя"), addnew_dlg->new_username->GetValue());
					if (addnew_dlg->block->GetValue()) {
						CnangeUserData(wxT("Установка блокировки"), addnew_dlg->new_username->GetValue());
					}
					else {
						CnangeUserData(wxT("Снятие блокировки"), addnew_dlg->new_username->GetValue());
					}
					if (addnew_dlg->limit->GetValue()) {
						CnangeUserData(wxT("Установка ограничений на пароль"), addnew_dlg->new_username->GetValue());
					}
					else {
						CnangeUserData(wxT("Снятие ограничений на пароль"), addnew_dlg->new_username->GetValue());
					}

					list->Append(wxString(users[users.size() - 1].name));

					addnew_dlg->Destroy();
				}
			}
		}
	}
	else {
		addnew_dlg->Destroy();
	}
}

void AdminFrame::OnChange_block(wxCommandEvent& event) {
	// Если файл аудита переполнен(кол-во строк >= 3*n + 1, n - макс кол-во записей, которое задает админ), 
	// Админ принудительно очищает файл аудита
	int number_of_lines = 0;
	std::string line;
	std::ifstream myfile(audfile_2);
	while (std::getline(myfile, line)) {
		++number_of_lines;
	}
	if (number_of_lines >= 3 * users[0].num_of_audit_records + 1) {
		wxMessageBox(wxT("Файл аудита изменений переполнен.\nОчистите его перед продолжением работы.\nПеред очисткой можно сохранить файл аудита."), wxT("Файл аудита переполнен"));
		auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит изменений в файле учетных записей"), audfile_2);
		auditop_dlg->ShowModal();
		block->SetValue(1 - block->GetValue());
	}
	else {
		bool par = block->GetValue();

		wxString str_name = list->GetStringSelection();

		auto it = find_if(users.begin(), users.end(), [str_name](User& u) { return u.name == str_name; });
		if (it != users.end()) {
			it->is_block = par;

			if (par) {
				CnangeUserData(wxT("Установка блокировки"), str_name);
			}
			else {
				CnangeUserData(wxT("Снятие блокировки"), str_name);
			}
		}
	}
}

void AdminFrame::OnChange_limit(wxCommandEvent& event) {
	// Если файл аудита переполнен(кол-во строк >= 3*n + 1, n - макс кол-во записей, которое задает админ), 
	// Админ принудительно очищает файл аудита
	int number_of_lines = 0;
	std::string line;
	std::ifstream myfile(audfile_2);
	while (std::getline(myfile, line)) {
		++number_of_lines;
	}
	if (number_of_lines >= 3 * users[0].num_of_audit_records + 1) {
		wxMessageBox(wxT("Файл аудита изменений переполнен.\nОчистите его перед продолжением работы.\nПеред очисткой можно сохранить файл аудита."), wxT("Файл аудита переполнен"));
		auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит изменений в файле учетных записей"), audfile_2);
		auditop_dlg->ShowModal();
		limit->SetValue(1 - limit->GetValue());
	}
	else {
		bool par = limit->GetValue();

		wxString str_name = list->GetStringSelection();

		auto it = find_if(users.begin(), users.end(), [str_name](User& u) { return u.name == str_name; });
		if (it != users.end()) {
			it->is_limit = par;

			if (par) {
				CnangeUserData(wxT("Установка ограничений на пароль"), str_name);
			}
			else {
				CnangeUserData(wxT("Снятие ограничений на пароль"), str_name);
			}
		}
	}
}

void AdminFrame::OnMinMax(wxCommandEvent& event) {
	setminmax_dlg = new SetMinMaxDlg();
	setminmax_dlg->min->SetValue((users[0].min_pswtime) / (24 * 60 * 60));
	setminmax_dlg->max->SetValue((users[0].max_pswtime) / (24 * 60 * 60));

	if (setminmax_dlg->ShowModal() == wxID_OK) {
		int min = (setminmax_dlg->min->GetValue()) * 24 * 60 * 60;		//
		int max = (setminmax_dlg->max->GetValue()) * 24 * 60 * 60;		//
		
		if (min > max) {
			wxMessageBox(wxT("Минимальное время не может быть больше, чем максимальное"), wxT("Ошибка при вводе"));
		}
		else {
			for (int i = 0; i < users.size(); ++i) {
				users[i].min_pswtime = min;
			}

			for (int i = 0; i < users.size(); ++i) {
				users[i].max_pswtime = max;
			}
		}
	}
	setminmax_dlg->Destroy();
}

// Когда меняем максимальную длину использованных паролей
void AdminFrame::OnChange_spin(wxCommandEvent& event) {
	int num = spin->GetValue();
	if (num > 0) {
		for (int i = 0; i < users.size(); ++i) {
			users[i].num_of_pswds = spin->GetValue();
			// Если размер меньше, чем было паролей, то обрезаем список паролей
			if (users[i].pswds_list.size() > num) {
				while (users[i].pswds_list.size() != num) {
					users[i].pswds_list.erase(users[i].pswds_list.begin());
				}
			}
		}
	}
	wxMessageBox("Операция выполнена успешно");
}

// Когда меняем максимальное количество записей в файлах аудита
void AdminFrame::OnChange_auditspin(wxCommandEvent& event) {
	int num = spin_length_audit ->GetValue();
	if (num > 0) {
		users[0].num_of_audit_records = spin_length_audit->GetValue();
	}
	wxMessageBox("Операция выполнена успешно");
}

void AdminFrame::OnSelect(wxCommandEvent& event) {
	// Когда выбираем пользователя в списке становяться доступны чекбоксы
	// В эти чекбоксы устанавливаем значение, которые соответствуют выбранному пользователю
	wxString str = event.GetString();

	if (str == AdminName) {
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

}

AdminFrame::~AdminFrame() {		// При уничтожении перезаписываем файл с информацией о пользователях
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
		fout << users[i].num_of_pswds << "\n";
		fout << users[i].last_changepsw << "\n";
		fout << users[i].filename_1 << "\n";
		fout << users[i].filename_2 << "\n";
		fout << users[i].num_of_audit_records << "\n";

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
	fout << users[users.size() - 1].max_pswtime << "\n";
	fout << users[users.size() - 1].num_of_pswds << "\n";
	fout << users[users.size() - 1].last_changepsw << "\n";
	fout << users[users.size() - 1].filename_1 << "\n";
	fout << users[users.size() - 1].filename_2 << "\n";
	fout << users[users.size() - 1].num_of_audit_records;

	fout.close();
}

ChangePswDlg::ChangePswDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("Смена пароля"), wxPoint(-1, -1), wxSize(400, 225))
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
	wxButton* btn_ok = new wxButton(pnl, ID_CHAGE_PSW, wxT("Ок"));
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////

string ChangePswDlg::EncriptionPsw(wxString str, char b) {
	// Первый этап шифрования
	string s = str.ToStdString();
	int sum = 0;    // сумма кодов четных символов
	for (int i = 0; i < s.size(); i += 2) {
		sum += (int)s[i];
	}
	srand(sum); // рандомизация генератора случайных чисел

	// ключ будет состоять из 5 значений
	string secret_key;
	for (int i = 0; i < 5; ++i) {
		secret_key.push_back(rand() % 256);
	}
	string res;
	// Если длина ключа меньше длины пароля, то дополняем оставшиеся позиции повторением ключа
	if (secret_key.size() < s.size()) {
		for (int i = 0; i < s.size(); ++i) {
			res.push_back(s[i] ^ secret_key[i % 5]);
		}
	}
	else {
		for (int i = 0; i < s.size(); ++i) {
			res.push_back(s[i] ^ secret_key[i]);
		}
	}

	// Второй этап шифрования
	string second_secret_key;
	second_secret_key.push_back(123);       // a = 123 - взаимно простое число с 65536 (условие для корректной расшифровки)
	second_secret_key.push_back(b);       // b = код первого символа в логине 

	string res_2;
	for (int i = 0; i < res.size(); ++i) {
		res_2.push_back((res[i] * second_secret_key[0] + second_secret_key[1]) % 65536);	// m = 65536 - длина алфавита
	}

	return res_2;
}


string ChangePswDlg::Decode(wxString str1, wxString str2, char b) {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////


void ChangePswDlg::OnOkBtn(wxCommandEvent& event) {
	if (this->GetOldPsw() == wxT("") || this->GetNewPsw() == wxT("") || this->GetConfirmPsw() == wxT("")) {
		wxMessageBox(wxT("Все поля должны быть заполнены"), wxT("Ошибка при вводе пароля"));
	}
	else {
		wxString oldStr = GetOldPsw();
		AdminFrame* p_wnd = (AdminFrame*)GetParent();
		auto it = find_if(p_wnd->users.begin(), p_wnd->users.end(), [p_wnd](User& u) { 
			return u.name == p_wnd->AdminName; });
		if (it != p_wnd->users.end()) {
			string s = Decode(oldStr, it->psw, wxString_to_lowercase(it->name)[0]);
			if (oldStr != s) {
			//if (it->psw != oldStr) {
				wxMessageBox(wxT("Неправильно введен старый пароль"), wxT("Ошибка при вводе пароля"));
			}
			else {
				if (this->GetNewPsw() != this->GetConfirmPsw()) {
					wxMessageBox(wxT("Пароли не совпадают"), wxT("Ошибка при вводе пароля"));
				}
				else {
					wxMessageBox(wxT("Пароль успешно изменен"), wxT("Смена пароля"));
					it->psw = wxString(EncriptionPsw(this->GetNewPsw(), wxString_to_lowercase(it->name)[0]));		// Сохраняем шифр, а не сам пароль
					//it->psw = this->GetNewPsw();
					Destroy();
				}
			}
		}
	}
}

AuditOperationsDlg::AuditOperationsDlg(wxWindow* parent, wxString f_name, string file_name, bool isEnterAaudit) : wxDialog(parent, wxID_ANY, f_name, wxPoint(-1, -1), wxSize(1200, 500))
{
	_file_name = file_name;

	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* query_box = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* btn_box = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* t_operation = new wxStaticText(pnl, wxID_ANY, wxT("Операция:"));
	tc_operation = new wxTextCtrl(pnl, wxID_ANY);
	wxStaticText* t_name = new wxStaticText(pnl, wxID_ANY, wxT("Имя:"));
	tc_name = new wxTextCtrl(pnl, wxID_ANY);
	wxStaticText* t_start = new wxStaticText(pnl, wxID_ANY, wxT("С:"));
	tc_datetime_start = new wxTextCtrl(pnl, wxID_ANY);
	wxStaticText* t_end = new wxStaticText(pnl, wxID_ANY, wxT("До:"));
	tc_datetime_end = new wxTextCtrl(pnl, wxID_ANY);

	query_box->Add(t_operation, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
	query_box->Add(tc_operation, 1, wxRIGHT, 10);
	query_box->Add(t_name, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
	query_box->Add(tc_name, 1, wxRIGHT, 10);
	query_box->Add(t_start, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
	query_box->Add(tc_datetime_start, 1, wxRIGHT, 10);
	query_box->Add(t_end, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
	query_box->Add(tc_datetime_end, 1, wxRIGHT, 10);
	btn_query = new wxButton(pnl, wxID_ANY, wxT("Найти"));
	query_box->Add(btn_query, 0, wxRIGHT, 5);
	btn_reset = new wxButton(pnl, wxID_ANY, wxT("Сбросить"));
	query_box->Add(btn_reset, 0, wxRIGHT);

	v_box->Add(query_box, 0, wxEXPAND | wxBOTTOM, 10);

	grid = new wxGrid(pnl, wxID_ANY);
	grid->CreateGrid(0, 4);

	string first_str;
	string operation;
	string name;
	string date_time;

	int count = -1;

	std::ifstream fin(file_name);

	if (fin.is_open())
	{
		getline(fin, first_str);
		while (!fin.eof()) 
		{
			getline(fin, operation);
			if (operation == wxT("")) {
				break;
			}
			getline(fin, name);
			getline(fin, date_time);
			count++;

			wxString s1(operation);
			wxString s2(name);
			wxString s3(date_time);

			audits.push_back(Audit(s1, s2, s3));

			grid->AppendRows();
			grid->SetCellValue(count, 0, wxString::Format(wxT("%i"), count + 1));
			grid->SetCellValue(count, 1, s1);
			grid->SetCellValue(count, 2, s2);
			grid->SetCellValue(count, 3, s3);

			grid->SetCellAlignment(count, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
			grid->SetCellAlignment(count, 1, wxALIGN_LEFT, wxALIGN_CENTRE);
			grid->SetCellAlignment(count, 2, wxALIGN_LEFT, wxALIGN_CENTRE);
			grid->SetCellAlignment(count, 3, wxALIGN_LEFT, wxALIGN_CENTRE);

		}
		fin.close();
	}

	grid->HideRowLabels();
	int width_0 = (this->GetSize().x) / 8;
	int width = (this->GetSize().x - width_0) / 3;
	int delta = 10;
	grid->SetColSize(0, width_0);
	grid->SetColSize(1, width - delta);
	grid->SetColSize(2, width - delta);
	grid->SetColSize(3, width - delta);

	grid->SetUseNativeColLabels(true);
	grid->SetColLabelSize(40);
	grid->SetSortingColumn(1);

	wxString* arr_str = new wxString[4]{ wxT("Номер записи"), wxT("Операция"), wxT("Имя пользователя"), wxT("Дата и время")};
	
	for (int i = 0; i < 4; ++i) {
		grid->SetColLabelValue(i, arr_str[i]);
		grid->DisableColResize(i);
	}	

	delete[] arr_str;

	for (int i = 0; i < grid->GetNumberRows(); ++i) {
		for (int j = 0; j < grid->GetNumberCols(); ++j) {
			grid->SetReadOnly(i, j);
		}
	}

	Connect(wxEVT_GRID_COL_SORT, wxGridEventHandler(AuditOperationsDlg::OnSortCol));
	Connect(btn_query->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AuditOperationsDlg::OnQuery));
	Connect(btn_reset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AuditOperationsDlg::OnReset));

	v_box->Add(grid, 1, wxEXPAND | wxBOTTOM, 10);
	wxButton* btn_ok = new wxButton(pnl, wxID_OK, wxT("Выход"));
	save_file = new wxButton(pnl, wxID_SAVE, wxT("Сохранить как"));
	clear_file = new wxButton(pnl, ID_CLEAR, wxT("Очистить файл аудита"));
	btn_box->Add(clear_file, 0, wxRight | wxLEFT, 5);
	btn_box->Add(save_file, 0, wxRight | wxLEFT, 5);
	btn_box->Add(btn_ok, 0, wxRight | wxLEFT, 5);
	
	v_box->Add(btn_box, 0, wxALIGN_RIGHT);
	around_box->Add(v_box, 1, wxEXPAND | wxALL, 7);

	pnl->SetSizer(around_box);

	Connect(wxID_SAVE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AuditOperationsDlg::OnSaveFile));
	Connect(ID_CLEAR, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AuditOperationsDlg::OnClearFile));


	Centre();
}

bool IsEnterAuditEx(wxString f_name) {
	wxString rev_ex;
	int i = f_name.size() - 1;
	char c = f_name[i];
	while (c != '.') {
		rev_ex.append(c);
		i--;
		c = f_name[i];
	}

	return rev_ex == wxT("duae");
}

void AuditOperationsDlg::OnSaveFile(wxCommandEvent& event) {
	if (IsEnterAuditEx(_file_name)) {
		wxFileDialog saveFileDialog(this, _("Сохранение файла аудита входа"), "", "", "Audit files (*.seaud)|*.seaud", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_OK) {
			wxString f_path = saveFileDialog.GetPath();

			std::ifstream fin(f_path.ToStdString());
			if (fin.is_open())								// Если файл уже существовал
			{
				fin.close();
				remove(f_path.ToStdString().c_str());
				std::ofstream fout(f_path.ToStdString());
				for (int i = 0; i < grid->GetNumberRows(); ++i) {
					for (int j = 1; j < grid->GetNumberCols(); ++j) {
						if (grid->GetCellValue(i, j) != "") {
							fout << grid->GetCellValue(i, j) << "\n";
						}
					}
				}
				fout.close();
			}
			else {											// Если пользователь ввел имя нового файла
				fin.close();
				std::ofstream fout(f_path.ToStdString());
				for (int i = 0; i < grid->GetNumberRows(); ++i) {
					for (int j = 1; j < grid->GetNumberCols(); ++j) {
						if (grid->GetCellValue(i, j) != "") {
							fout << grid->GetCellValue(i, j) << "\n";
						}
					}
				}
				fout.close();
			}

			return;
		}
		else {
			return;
		}
	}
	else {
		wxFileDialog saveFileDialog(this, _("Сохранение файла аудита изменений"), "", "", "Audit files (*.scaud)|*.scaud", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_OK) {
			wxString f_path = saveFileDialog.GetPath();

			std::ifstream fin(f_path.ToStdString());
			if (fin.is_open())								// Если файл уже существовал
			{
				fin.close();
				remove(f_path.ToStdString().c_str());
				std::ofstream fout(f_path.ToStdString());
				for (int i = 0; i < grid->GetNumberRows(); ++i) {
					for (int j = 1; j < grid->GetNumberCols(); ++j) {
						if (grid->GetCellValue(i, j) != "") {
							fout << grid->GetCellValue(i, j) << "\n";
						}
					}
				}
				fout.close();
			}
			else {											// Если пользователь ввел имя нового файла
				fin.close();
				std::ofstream fout(f_path.ToStdString());
				for (int i = 0; i < grid->GetNumberRows(); ++i) {
					for (int j = 1; j < grid->GetNumberCols(); ++j) {
						if (grid->GetCellValue(i, j) != "") {
							fout << grid->GetCellValue(i, j) << "\n";
						}
					}
				}
				fout.close();
			}
			
			return;
		}
		else {
			return;
		}
	}
}

void AuditOperationsDlg::OnClearFile(wxCommandEvent& event) {
	if (IsEnterAuditEx(_file_name)) {
		std::ofstream ofs;
		ofs.open(_file_name.ToStdString(), std::ofstream::out | std::ofstream::trunc);
		ofs << "Файл аудита:\n";
		ofs.close();
		
		for (int i = 0; i < grid->GetNumberRows(); ++i) {
			for (int j = 0; j < grid->GetNumberCols(); ++j) {
				grid->SetCellValue(i, j, wxT(""));
			}
		}
	}
	else {
		std::ofstream ofs;
		ofs.open(_file_name.ToStdString(), std::ofstream::out | std::ofstream::trunc);
		ofs << "Файл аудита:\n";
		ofs.close();

		for (int i = 0; i < grid->GetNumberRows(); ++i) {
			for (int j = 0; j < grid->GetNumberCols(); ++j) {
				grid->SetCellValue(i, j, wxT(""));
			}
		}
	}
}


void AuditOperationsDlg::OnSortCol(wxGridEvent& event) {
	int col_num = event.GetCol();
	if (col_num == 0) 
		return;

	if (col_num == 1) 
	{
		sort(audits.begin(), audits.end(), [](Audit& a1, Audit& a2) {return a1.adminOperation < a2.adminOperation; });
	}
	else if (col_num == 2)
	{
		sort(audits.begin(), audits.end(), [](Audit& a1, Audit& a2) {return a1.userName < a2.userName; });
	} 
	else
	{
		sort(audits.begin(), audits.end(), [](Audit& a1, Audit& a2) {return a1.dateTime < a2.dateTime; });
	}

	for (int i = 0; i < grid->GetNumberRows(); ++i) 
	{
		grid->SetCellValue(i, 0, wxString::Format(wxT("%i"), i + 1));
		grid->SetCellValue(i, 1, audits[i].adminOperation);
		grid->SetCellValue(i, 2, audits[i].userName);
		grid->SetCellValue(i, 3, audits[i].dateTime);
	}
}

bool cmp_time(const wxString& time1, const wxString& time2, const char& cmp_op, const wxString& time3 = wxT("")) {
	struct std::tm tm;
	wxString temp = time1;
	std::istringstream ss(temp.ToStdString());
	ss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");
	std::time_t a_time = mktime(&tm);

	struct std::tm tm2;
	temp = time2;
	std::istringstream ss2(temp.ToStdString());
	ss2 >> std::get_time(&tm2, "%a %b %d %H:%M:%S %Y");
	std::time_t time_first = mktime(&tm2);

	if (time3 != wxT("")) {
		struct std::tm tm3;
		temp = time3;
		std::istringstream ss3(temp.ToStdString());
		ss3 >> std::get_time(&tm3, "%a %b %d %H:%M:%S %Y");
		std::time_t time_second = mktime(&tm3);

		return (a_time >= time_first && a_time <= time_second);
	}
	
	if (cmp_op == '<') {
		return (a_time <= time_first);
	}
	if (cmp_op == '>') {
		return (a_time >= time_first);
	}
	
}

void AuditOperationsDlg::OnQuery(wxCommandEvent& event) {
	vector<Audit> res;

	wxString s_op = tc_operation->GetValue();
	wxString s_name = tc_name->GetValue();
	wxString s_dtstart = tc_datetime_start->GetValue();
	wxString s_dtend = tc_datetime_end->GetValue();

	struct std::tm tm;
	wxString temp = s_dtstart;
	std::istringstream ss(temp.ToStdString());
	ss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");
	std::time_t start_time = mktime(&tm);

	struct std::tm tm2;
	temp = s_dtend;
	std::istringstream ss2(temp.ToStdString());
	ss2 >> std::get_time(&tm2, "%a %b %d %H:%M:%S %Y");
	std::time_t end_time= mktime(&tm2);

	if (s_dtstart != wxT("") && s_dtend != wxT("")) {
		if (end_time < start_time) {
			wxMessageBox(wxT("Время, с которого производится поиск больше времени, до которого производится поиск"), wxT("Ошибка во время запроса"));
			return;
		}
	}
	
	auto it = find_if(audits.begin(), audits.end(), [s_op, s_name, s_dtstart, s_dtend](Audit& a) {
		
		return ((s_op.IsEmpty() || a.adminOperation == s_op) && (s_name.IsEmpty() || a.userName == s_name) &&
				(s_dtstart.IsEmpty() && s_dtend.IsEmpty() 
					|| s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtend, '<')
					|| !s_dtstart.IsEmpty() && s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, '>')
					|| !s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, ' ', s_dtend)
				));	
	});
	while (it != audits.end()) {
		res.push_back(*it);
		it = find_if(it + 1, audits.end(), [s_op, s_name, s_dtstart, s_dtend](Audit& a) {

			return ((s_op.IsEmpty() || a.adminOperation == s_op) && (s_name.IsEmpty() || a.userName == s_name) &&
				(s_dtstart.IsEmpty() && s_dtend.IsEmpty()
					|| s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtend, '<')
					|| !s_dtstart.IsEmpty() && s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, '>')
					|| !s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, ' ', s_dtend)
					));
		});
	}

	for (int i = 0; i < grid->GetNumberRows(); ++i) {
		for (int j = 0; j < grid->GetNumberCols(); ++j) {
			grid->SetCellValue(i, j, wxT(""));
		}
	}

	if (!res.size()) {
		wxMessageBox(wxT("Поиск не дал ни одного результата"));
		return;
	}

	for (int i = 0; i < res.size(); ++i) {
		grid->SetCellValue(i, 0, wxString::Format(wxT("%i"), i + 1));
		grid->SetCellValue(i, 1, res[i].adminOperation);
		grid->SetCellValue(i, 2, res[i].userName);
		grid->SetCellValue(i, 3, res[i].dateTime);
	}
}

void AuditOperationsDlg::OnReset(wxCommandEvent& event) {

	string first_str;
	string operation;
	string name;
	string date_time;

	int count = -1;

	std::ifstream fin(_file_name.ToStdString());

	if (fin.is_open())
	{
		getline(fin, first_str);
		while (!fin.eof())
		{
			getline(fin, operation);
			if (operation == wxT("")) {
				break;
			}
			getline(fin, name);
			getline(fin, date_time);
			count++;

			wxString s1(operation);
			wxString s2(name);
			wxString s3(date_time);

			grid->SetCellValue(count, 0, wxString::Format(wxT("%i"), count + 1));
			grid->SetCellValue(count, 1, s1);
			grid->SetCellValue(count, 2, s2);
			grid->SetCellValue(count, 3, s3);
		}
		fin.close();
	}

	tc_operation->SetValue("");
	tc_name->SetValue("");
	tc_datetime_start->SetValue("");
	tc_datetime_end->SetValue("");

}

AddNewUserDlg::AddNewUserDlg() : wxDialog(NULL, wxID_ANY, wxT("Новый пользователь"), wxPoint(-1, -1), wxSize(300, 250))
{
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* t_name = new wxStaticText(pnl, wxID_ANY, wxT("Имя пользователя:"));
	new_username = new wxTextCtrl(pnl, wxID_ANY);
	v_box->Add(t_name, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);
	v_box->Add(new_username, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	block = new wxCheckBox(pnl, wxID_ANY, wxT("блокировка"));
	limit = new wxCheckBox(pnl, wxID_ANY, wxT("ограничение \nна пароль"));

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

/////////////////////////////////////////////////////////////////

FirstAdminEnterDlg::FirstAdminEnterDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("Первый вход в систему"), wxPoint(-1, -1), wxSize(480, 420)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box5 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* info = new wxStaticText(pnl, wxID_ANY, wxT("Вы впервые вошли в систему.\nПридумайте себе пароль.\nТакже установите имена для файлов аудита"));
	v_box->Add(info, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_psw1 = new wxStaticText(pnl, wxID_ANY, wxT("Придумайте пароль:"));
	first_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box1->Add(t_psw1, 1, wxEXPAND | wxRIGHT, 15);
	h_box1->Add(first_psw, 1, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box1, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_psw2 = new wxStaticText(pnl, wxID_ANY, wxT("Повторите пароль:"));
	confirm_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box2->Add(t_psw2, 1, wxEXPAND | wxRIGHT, 15);
	h_box2->Add(confirm_psw, 1, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box2, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_1 = new wxStaticText(pnl, wxID_ANY, wxT("Файл аудита входа:"));
	enter_audit = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1));
	h_box4->Add(t_1, 1, wxEXPAND | wxRIGHT, 15);
	h_box4->Add(enter_audit, 1, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box4, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_2 = new wxStaticText(pnl, wxID_ANY, wxT("Файл аудита изменений:"));
	changes_audit = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1));
	h_box5->Add(t_2, 1, wxEXPAND | wxRIGHT, 15);
	h_box5->Add(changes_audit, 1, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box5, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxButton* ok_btn = new wxButton(pnl, ID_BTN2, wxT("Ок"));
	wxButton* cnl_btn = new wxButton(pnl, wxID_CANCEL, wxT("Отмена"));
	h_box3->Add(ok_btn, 0, wxEXPAND | wxRIGHT, 10);
	h_box3->Add(cnl_btn, 0, wxEXPAND);
	v_box->Add(h_box3, 0, wxALIGN_RIGHT);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	around_box->Add(v_box, 1, wxLEFT | wxALL, 15);

	pnl->SetSizer(around_box);

	Centre();

	Connect(ID_BTN2, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirstAdminEnterDlg::OnOkBtn));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string FirstAdminEnterDlg::EncriptionPsw(wxString str, char b) {
	// Первый этап шифрования
	string s = str.ToStdString();
	int sum = 0;    // сумма кодов четных символов
	for (int i = 0; i < s.size(); i += 2) {
		sum += (int)s[i];
	}
	srand(sum); // рандомизация генератора случайных чисел

	// ключ будет состоять из 5 значений
	string secret_key;
	for (int i = 0; i < 5; ++i) {
		secret_key.push_back(rand() % 256);
	}
	string res;
	// Если длина ключа меньше длины пароля, то дополняем оставшиеся позиции повторением ключа
	if (secret_key.size() < s.size()) {
		for (int i = 0; i < s.size(); ++i) {
			res.push_back(s[i] ^ secret_key[i % 5]);
		}
	}
	else {
		for (int i = 0; i < s.size(); ++i) {
			res.push_back(s[i] ^ secret_key[i]);
		}
	}

	// Второй этап шифрования
	string second_secret_key;
	second_secret_key.push_back(123);       // a = 123 - взаимно простое число с 65536 (условие для корректной расшифровки)
	second_secret_key.push_back(b);       // b = код первого символа в логине 

	string res_2;
	for (int i = 0; i < res.size(); ++i) {
		res_2.push_back((res[i] * second_secret_key[0] + second_secret_key[1]) % 65536);	// m = 65536 - длина алфавита
	}

	return res_2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FirstAdminEnterDlg::OnOkBtn(wxCommandEvent& event) {
	if (this->GetFirstPsw() == wxT("") || this->GetConfirmPsw() == wxT("") || enter_audit->GetValue() == wxT("") || changes_audit->GetValue() == wxT("")) {
		wxMessageBox(wxT("Все поля должны быть заполнены!"), wxT("Ошибка при вводе"));
	}
	else {
		if (this->GetFirstPsw() != this->GetConfirmPsw()) {
			wxMessageBox(wxT("Значения полей с паролями не совпадают!"), wxT("Ошибка при вводе пароля"));
		}
		else {
			if (enter_audit->GetValue() == changes_audit->GetValue()) {
				wxMessageBox(wxT("Имена файлов не могут быть одинаковыми!"), wxT("Ошибка при вводе имен файлов"));
			}
			else {
				MainFrame* p_wnd = (MainFrame*)GetParent();
				auto it = find_if(p_wnd->start_users.begin(), p_wnd->start_users.end(), [this](User& u) { return u.name == userNameForSearch; });
				if (wxString_to_lowercase(userNameForSearch) == wxString_to_lowercase(wxString(wxT("ADMIN")))) {
					it->psw = wxString(EncriptionPsw(this->GetFirstPsw(), wxString_to_lowercase(userNameForSearch)[0])); // Сохраняем шифр, а не сам пароль
					//it->psw = this->GetFirstPsw();		// Пароль аддмина может быть любым
					p_wnd->start_users[0].filename_1 = enter_audit->GetValue();
					p_wnd->start_users[0].filename_2 = changes_audit->GetValue();
					p_wnd->main_audfile_1 = enter_audit->GetValue();
					p_wnd->main_audfile_2 = changes_audit->GetValue();
					wxMessageBox(wxT("Вы успешно установили пароль"));
					Destroy();
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////

FirstEnterDlg::FirstEnterDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("Первый вход в систему"), wxPoint(-1, -1), wxSize(400, 270)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box3 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* info = new wxStaticText(pnl, wxID_ANY, wxT("Вы впервые вошли в систему.\nПридумайте себе пароль."));
	v_box->Add(info, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_psw1 = new wxStaticText(pnl, wxID_ANY, wxT("Придумайте пароль:"));
	first_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box1->Add(t_psw1, 1, wxEXPAND | wxRIGHT, 15);
	h_box1->Add(first_psw, 1, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box1, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_psw2 = new wxStaticText(pnl, wxID_ANY, wxT("Повторите пароль:"));
	confirm_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box2->Add(t_psw2, 1, wxEXPAND | wxRIGHT, 15);
	h_box2->Add(confirm_psw, 1, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box2, 1, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxButton* ok_btn = new wxButton(pnl, ID_BTN, wxT("Ок"));
	wxButton* cnl_btn = new wxButton(pnl, wxID_CANCEL, wxT("Отмена"));
	h_box3->Add(ok_btn, 0, wxEXPAND | wxRIGHT, 10);
	h_box3->Add(cnl_btn, 0, wxEXPAND | wxRIGHT, 10);
	v_box->Add(h_box3, 0, wxALIGN_RIGHT);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	around_box->Add(v_box, 1, wxLEFT | wxALL, 15);

	pnl->SetSizer(around_box);

	Centre();

	Connect(ID_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirstEnterDlg::OnOkBtn));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

string FirstEnterDlg::EncriptionPsw(wxString str, char b) {
	// Первый этап шифрования
	string s = str.ToStdString();
	int sum = 0;    // сумма кодов четных символов
	for (int i = 0; i < s.size(); i += 2) {
		sum += (int)s[i];
	}
	srand(sum); // рандомизация генератора случайных чисел

	// ключ будет состоять из 5 значений
	string secret_key;
	for (int i = 0; i < 5; ++i) {
		secret_key.push_back(rand() % 256);
	}
	string res;
	// Если длина ключа меньше длины пароля, то дополняем оставшиеся позиции повторением ключа
	if (secret_key.size() < s.size()) {
		for (int i = 0; i < s.size(); ++i) {
			res.push_back(s[i] ^ secret_key[i % 5]);
		}
	}
	else {
		for (int i = 0; i < s.size(); ++i) {
			res.push_back(s[i] ^ secret_key[i]);
		}
	}

	// Второй этап шифрования
	string second_secret_key;
	second_secret_key.push_back(123);       // a = 123 - взаимно простое число с 65536 (условие для корректной расшифровки)
	second_secret_key.push_back(b);       // b = код первого символа в логине 

	string res_2;
	for (int i = 0; i < res.size(); ++i) {
		res_2.push_back((res[i] * second_secret_key[0] + second_secret_key[1]) % 65536);	// m = 65536 - длина алфавита
	}

	return res_2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////



void FirstEnterDlg::OnOkBtn(wxCommandEvent& event) {
	if (this->GetFirstPsw() == wxT("") || this->GetConfirmPsw() == wxT("")) {
		wxMessageBox(wxT("Все поля должны быть заполнены"), wxT("Ошибка при вводе пароля"));
	}
	else {
		if (this->GetFirstPsw() != this->GetConfirmPsw()) {
			wxMessageBox(wxT("Значения полей не совпадают"), wxT("Ошибка при вводе пароля"));
		}
		else {
			MainFrame* p_wnd = (MainFrame*)GetParent();
			auto it = find_if(p_wnd->start_users.begin(), p_wnd->start_users.end(), [this](User& u) { return u.name == userNameForSearch; });
			if (wxString_to_lowercase(userNameForSearch) != wxString_to_lowercase(wxString(wxT("ADMIN")))) {
				if (it->is_limit) {		// Проверяем пароль регулярным выражением
					//wxRegEx re("^(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*[1-9]+[^.,!?:;А-я]*)*(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*)|([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*))?)$");
					wxRegEx re("^(([^.,!?:;A-z1-9]*[.,!?:;]+[^A-z1-9]*[A-z]+[^.,!?:;1-9]*[1-9]+[^.,!?:;A-z]*)*(([^.,!?:;A-z1-9]*[.,!?:;]+[^A-z1-9]*[A-z]+[^.,!?:;1-9]*)|([^.,!?:;A-z1-9]*[.,!?:;]+[^A-z1-9]*))?)$");
					if (re.IsValid()) {
						//wxMessageBox(wxT("Compiled!"));
					}
					if (re.Matches(this->GetFirstPsw())) {
						//wxMessageBox(wxT("Matched"));
						it->psw = wxString(EncriptionPsw(this->GetFirstPsw(), wxString_to_lowercase(userNameForSearch)[0]));					// Сохраняем шифр, а не сам пароль
						//it->psw = this->GetFirstPsw();
						it->pswds_list.push_back(wxString(EncriptionPsw(this->GetFirstPsw(), wxString_to_lowercase(userNameForSearch)[0])));	// Сохраняем шифр, а не сам пароль
						//it->pswds_list.push_back(this->GetFirstPsw());
						it->last_changepsw = time(0);
						wxMessageBox(wxT("Вы успешно установили пароль"));
						Destroy();
					}
					else {
						//wxMessageBox(wxT("Not matched!"));
						//wxMessageBox(wxT("Ваш пароль не соответствует минимальным требованиям!\n(чередование знаков препинания, символов кириллицы, цифр)"));
						wxMessageBox(wxT("Ваш пароль не соответствует минимальным требованиям!\n(чередование знаков препинания, символов латиницы, цифр)"));
					}
				}
				else {
					it->psw = wxString(EncriptionPsw(this->GetFirstPsw(), wxString_to_lowercase(userNameForSearch)[0]));					// Сохраняем шифр, а не сам пароль
					//it->psw = this->GetFirstPsw();			
					it->pswds_list.push_back(wxString(EncriptionPsw(this->GetFirstPsw(), wxString_to_lowercase(userNameForSearch)[0])));	// Сохраняем шифр, а не сам пароль
					//it->pswds_list.push_back(this->GetFirstPsw());
					it->last_changepsw = time(0);
					wxMessageBox(wxT("Вы успешно установили пароль"));
					Destroy();
				}
			}
		}
	}
}

AuditsFilesNames::AuditsFilesNames(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("Установка имён для файлов аудита"), wxPoint(-1, -1), wxSize(400, 270)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box3 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* info = new wxStaticText(pnl, wxID_ANY, wxT("Вы впервые вошли в систему.\nУстановите имена для файлов аудита."));
	v_box->Add(info, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_1 = new wxStaticText(pnl, wxID_ANY, wxT("Файл аудита входа:"));
	enter_audit = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1));
	h_box1->Add(t_1, 0, wxEXPAND | wxRIGHT, 15);
	h_box1->Add(enter_audit, 0, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box1, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_2 = new wxStaticText(pnl, wxID_ANY, wxT("Файл аудита изменений учетных записей:"));
	changes_audit = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1));
	h_box2->Add(t_2, 0, wxEXPAND | wxRIGHT, 15);
	h_box2->Add(changes_audit, 0, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box2, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxButton* ok_btn = new wxButton(pnl, wxID_OK, wxT("Ок"));
	wxButton* cnl_btn = new wxButton(pnl, wxID_CANCEL, wxT("Отмена"));
	h_box3->Add(ok_btn, 0, wxEXPAND | wxRIGHT, 10);
	h_box3->Add(cnl_btn, 0, wxEXPAND | wxRIGHT, 10);
	v_box->Add(h_box3, 0, wxALIGN_RIGHT);

	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);
	around_box->Add(v_box, 1, wxLEFT | wxALL, 15);

	pnl->SetSizer(around_box);

	Centre();

}


SetMinMaxDlg::SetMinMaxDlg() : wxDialog(NULL, wxID_ANY, wxT("Установка времени действия пароля"), wxPoint(-1, -1), wxSize(400, 125)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_min = new wxStaticText(pnl, wxID_ANY, wxT("Минимум (дней)"));		//	дней
	wxStaticText* t_max = new wxStaticText(pnl, wxID_ANY, wxT("Максимум (дней)"));		//	дней

	h_box1->Add(t_min, 0, wxEXPAND | wxRIGHT, 5);
	min = new wxSpinCtrl(pnl, wxID_ANY);
	min->SetRange(0, 1000);
	min->SetValue(0);
	h_box1->Add(min, 1, wxEXPAND | wxRIGHT, 5);
	h_box1->Add(t_max, 0, wxEXPAND | wxRIGHT, 5);
	max = new wxSpinCtrl(pnl, wxID_ANY);
	max->SetRange(0, 1000);
	max->SetValue(0);
	h_box1->Add(max, 1, wxEXPAND);

	v_box->Add(h_box1, 0, wxEXPAND | wxBOTTOM, 5);

	wxButton* ok_btn = new wxButton(pnl, wxID_OK, wxT("Ок"));
	wxButton* cnl_btn = new wxButton(pnl, wxID_CANCEL, wxT("Отмена"));

	h_box2->Add(ok_btn, 0, wxEXPAND | wxRIGHT, 5);
	h_box2->Add(cnl_btn, 0, wxEXPAND);

	v_box->Add(h_box2, 0, wxALIGN_RIGHT);

	around_box->Add(v_box, 0, wxEXPAND | wxALL, 5);

	pnl->SetSizer(around_box);
}