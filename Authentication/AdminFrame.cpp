#include "AdminFrame.h"

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

	wxButton* add_new = new wxButton(pnl, wxID_ADD, wxT("Добавить"));
	v_box->Add(add_new, 0, wxEXPAND | wxBOTTOM, 15);
	block = new wxCheckBox(pnl, wxID_ANY, wxT("блокировка"));
	v_box->Add(block, 0, wxEXPAND | wxBOTTOM, 15);
	block->Enable(false);
	limit = new wxCheckBox(pnl, wxID_ANY, wxT("ограничение \nна пароль"));
	v_box->Add(limit, 0, wxEXPAND | wxBOTTOM, 15);
	limit->Enable(false);
	wxButton* set_minmax = new wxButton(pnl, ID_MINMAX, wxT("Установить\nмин, макс\nсрок действия пароля"));
	v_box->Add(set_minmax, 0, wxEXPAND | wxBOTTOM, 15);
	wxButton* save = new wxButton(pnl, wxID_SAVE, wxT("Сохранить"));
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
	//Connect(wxID_SAVE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AdminFrame::OnSave));	// Пока отключим, будем менять значения разу после изменения checkbox
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

			// Проверить, что в файле присутствовали все нужные поля ???
			User user = User(s_name, s_psw, wxAtoi(s1), wxAtoi(s2), wxAtoi(s3), wxAtoi(s4), vec);
			users.push_back(user);
		}
		fin.close();
	}

	for (int i = 0; i < users.size(); ++i) {
		list->Append(wxString(users[i].name));
	}

	// Файл для аудита изменений в файле учетных записей
	std::ifstream fin_aud("changes_audit.txt");
	if (!fin_aud.is_open())
	{
		std::ofstream fout("changes_audit.txt");
		fout << "Файл аудита изменений в файле учетных записей:\n";
		fout.close();
	}
}

void AdminFrame::OnMenuExit(wxMenuEvent& event) {
	Close();
}

void AdminFrame::OnMenuAbout(wxMenuEvent& event) {
	wxMessageBox(wxT("Щучкин Н.Ю. \nГруппа А-13а-19 \nКурсовая работа"), wxT("О программе"));
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
	fin_aud << "Операция:" << operation << "\t" << "Имя пользователя:" << username << "\t" << "Дата/время:" << dt;
}

void AdminFrame::ExitFromSystem(wxString login) {
	std::ofstream fin_aud("audit.txt", std::ios_base::app);
	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	char* dt = ctime(&now);
	fin_aud << "Успешный выход:" << "\t" << "Имя пользователя:" << login << "\t" << "Дата/время:" << dt;
}

void AdminFrame::OnLogOut(wxMenuEvent& event) {
	ExitFromSystem(wxT("ADMIN"));

	MainFrame* mainFrame = new MainFrame(NULL, wxID_ANY, wxT("Вход"), wxPoint(-1, -1), wxSize(370, 225));
	mainFrame->start_users = users;
	Destroy();
	mainFrame->Show(true);
}

void AdminFrame::OnAddNew(wxCommandEvent& event) {
	addnew_dlg = new AddNewUserDlg();
	if (addnew_dlg->ShowModal() == wxID_OK) {
		wxString str = addnew_dlg->new_username->GetValue();
		auto it = find_if(users.begin(), users.end(), [str](User& u) { return u.name == str; });
		if (it != users.end()) {	// Если пользователь с таким именем уже есть
			wxMessageBox(wxT("Пользователь с таким именем уже существует в системе!"));
		}
		else {	// Если такого пользователя еще нет, то добавляем данные по новому пользователю в вектор и ListBox
			User u(addnew_dlg->new_username->GetValue(), wxT(""), addnew_dlg->block->GetValue(), addnew_dlg->limit->GetValue(), 0, 0, {});	// !!! вместо двух предпоследних параметров установливается мин и макс время, если есть
			users.push_back(u);

			CnangeUserData(wxT("Добавление нового пользователя"), str);
			if (addnew_dlg->block->GetValue()) {
				CnangeUserData(wxT("Установка блокировки"), str);
			}
			else {
				CnangeUserData(wxT("Снятие блокировки"), str);
			}
			if (addnew_dlg->limit->GetValue()) {
				CnangeUserData(wxT("Установка ограничений на пароль"), str);
			}
			else {
				CnangeUserData(wxT("Снятие ограничений на пароль"), str);
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
			CnangeUserData(wxT("Установка блокировки"), str_name);
		}
		else {
			CnangeUserData(wxT("Снятие блокировки"), str_name);
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
			CnangeUserData(wxT("Установка ограничений на пароль"), str_name);
		}
		else {
			CnangeUserData(wxT("Снятие ограничений на пароль"), str_name);
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
	// Когда выбираем пользователя в списке становяться доступны чекбоксы
	// В эти чекбоксы устанавливаем значение, которые соответствуют выбранному пользователю
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

	wxMessageBox(list_pswds, wxT("Список использованных паролей"));
	
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

void ChangePswDlg::OnOkBtn(wxCommandEvent& event) {
	if (this->GetOldPsw() == wxT("") || this->GetNewPsw() == wxT("") || this->GetConfirmPsw() == wxT("")) {
		wxMessageBox(wxT("Все поля должны быть заполнены"), wxT("Ошибка при вводе пароля"));
	}
	else {
		wxString oldStr = GetOldPsw();
		AdminFrame* p_wnd = (AdminFrame*)GetParent();
		auto it = find_if(p_wnd->users.begin(), p_wnd->users.end(), [](User& u) { return u.name == wxT("ADMIN"); });
		if (it != p_wnd->users.end()) {
			if (it->psw != oldStr) {
				wxMessageBox(wxT("Неправильно введен старый пароль"), wxT("Ошибка при вводе пароля"));
			}
			else {
				if (this->GetNewPsw() != this->GetConfirmPsw()) {
					wxMessageBox(wxT("Пароли не совпадают"), wxT("Ошибка при вводе пароля"));
				}
				else {
					wxMessageBox(wxT("Пароль успешно изменен"), wxT("Смена пароля"));
					it->psw = this->GetNewPsw();
					Destroy();
				}
			}
		}
	}
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
	h_box1->Add(t_psw1, 0, wxEXPAND | wxRIGHT, 15);
	h_box1->Add(first_psw, 0, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box1, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	wxStaticText* t_psw2 = new wxStaticText(pnl, wxID_ANY, wxT("Повторите пароль:"));
	confirm_psw = new wxTextCtrl(pnl, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(200, -1), wxTE_PASSWORD);
	h_box2->Add(t_psw2, 0, wxEXPAND | wxRIGHT, 15);
	h_box2->Add(confirm_psw, 0, wxEXPAND | wxLEFT, 15);
	v_box->Add(h_box2, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

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
			if (userNameForSearch == wxT("ADMIN")) {
				it->psw = this->GetFirstPsw();		// Пароль аддмина может быть любым
				Destroy();
			}
			else {
				if (it->is_limit) {		// Проверяем пароль регулярным выражением
					//wxRegEx re("^(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*[1-9]+[^.,!?:;А-я]*)+|([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*)|([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*))$");
					wxRegEx re("^(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*[1-9]+[^.,!?:;А-я]*)*(([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*[А-я]+[^.,!?:;1-9]*)|([^.,!?:;А-я1-9]*[.,!?:;]+[^А-я1-9]*))?)$");
					if (re.IsValid()) {
						//wxMessageBox(wxT("Compiled!"));
					}
					if (re.Matches(this->GetFirstPsw())) {
						//wxMessageBox(wxT("Matched"));
						it->psw = this->GetFirstPsw();
						it->pswds_list.push_back(this->GetFirstPsw());
						wxMessageBox(wxT("Вы успешно установили пароль"));
						Destroy();
					}
					else {
						//wxMessageBox(wxT("Not matched!"));
						wxMessageBox(wxT("Ваш пароль не соответствует минимальным требованиям!\n(чередование знаков препинания, символов кириллицы, цифр)"));
					}
				}
				else {
					it->psw = this->GetFirstPsw();
					it->pswds_list.push_back(this->GetFirstPsw());
					wxMessageBox(wxT("Вы успешно установили пароль"));
					Destroy();
				}
			}
		}
	}
}

SetMinMaxDlg::SetMinMaxDlg() : wxDialog(NULL, wxID_ANY, wxT("Установка времени действия пароля"), wxPoint(-1, -1), wxSize(300, 200)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_min = new wxStaticText(pnl, wxID_ANY, wxT("Мин"));
	wxStaticText* t_max = new wxStaticText(pnl, wxID_ANY, wxT("Макс"));

	h_box1->Add(t_min, 1, wxEXPAND | wxRIGHT, 10);
	min = new wxTextCtrl(pnl, wxID_ANY);
	h_box1->Add(min, 1, wxEXPAND | wxRIGHT, 10);
	h_box1->Add(t_max, 1, wxEXPAND | wxRIGHT, 10);
	max = new wxTextCtrl(pnl, wxID_ANY);
	h_box1->Add(max, 1, wxEXPAND);

	v_box->Add(h_box1, 0, wxEXPAND | wxBOTTOM, 10);

	wxButton* ok_btn = new wxButton(pnl, wxID_OK, wxT("Ок"));
	wxButton* cnl_btn = new wxButton(pnl, wxID_CANCEL, wxT("Отмена"));

	h_box2->Add(ok_btn, 0, wxEXPAND | wxRIGHT, 10);
	h_box2->Add(cnl_btn, 0, wxEXPAND);

	v_box->Add(h_box2, 0, wxALIGN_RIGHT);

	around_box->Add(v_box, 0, wxEXPAND | wxALL, 10);

	pnl->SetSizer(around_box);
}