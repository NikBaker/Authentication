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
	//
	wxStaticText* length_psws = new wxStaticText(pnl, wxID_ANY, wxT("Длина списка использованных паролей:"));
	wxSpinCtrl* spin = new wxSpinCtrl(pnl, wxID_ANY);
	v_box->Add(length_psws, 0, /*wxEXPAND | */wxBOTTOM, 5);
	v_box->Add(spin, 0, /*wxEXPAND | */wxBOTTOM, 15);
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

		fin_aud.close();	//
	}
}

void AdminFrame::OnMenuExit(wxMenuEvent& event) {		// При закрытии разлогиниваемся?
	ExitFromSystem(AdminName);
	Destroy();
}

void AdminFrame::OnClose(wxCommandEvent& event) {		// При закрытии разлогиниваемся?
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
	auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит изменений в файле учетных записей"));
	auditop_dlg->ShowModal();
}

void AdminFrame::OnAudit(wxMenuEvent& event) {
	auditop_dlg = new AuditOperationsDlg(this, wxT("Аудит удачных и неудачных попыток входа"), true);
	auditop_dlg->ShowModal();
}

void AdminFrame::CnangeUserData(wxString operation, wxString username) {
	std::ofstream fin_aud("changes_audit.txt", std::ios_base::app);
	time_t now = time(0);
	char* dt = ctime(&now);
	fin_aud << operation << "\n" << username << "\n" << dt;
}

void AdminFrame::ExitFromSystem(wxString login) {
	std::ofstream fin_aud("audit.txt", std::ios_base::app);
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
		string str = wxString_to_lowercase(addnew_dlg->new_username->GetValue());
		auto it = find_if(users.begin(), users.end(), [str](User& u) {
			string uname = wxString_to_lowercase(u.name);
			return uname == str; });
		if (it != users.end()) {	// Если пользователь с таким именем уже есть
			wxMessageBox(wxT("Пользователь с таким именем уже существует в системе!"));
		}
		else {	// Если такого пользователя еще нет, то добавляем данные по новому пользователю в вектор и ListBox
			User u(addnew_dlg->new_username->GetValue(), wxT(""), addnew_dlg->block->GetValue(), addnew_dlg->limit->GetValue(), 0, 0, {});	// !!! вместо двух предпоследних параметров установливается мин и макс время, если есть
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

		//setminmax_dlg->Destroy();
	}
	setminmax_dlg->Destroy();
	/*else {
		setminmax_dlg->Destroy();
	}*/
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
	/*wxString name = event.GetString();
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
	wxMessageBox(list_pswds, wxT("Список использованных паролей"));*/
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
		auto it = find_if(p_wnd->users.begin(), p_wnd->users.end(), [p_wnd](User& u) { 
			return u.name == p_wnd->AdminName; });
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

AuditOperationsDlg::AuditOperationsDlg(wxWindow* parent, wxString f_name, bool isEnterAaudit) : wxDialog(parent, wxID_ANY, f_name, wxPoint(-1, -1), wxSize(1200, 500))
{
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
	query_box->Add(btn_query, 0, wxRIGHT);

	v_box->Add(query_box, 0, wxEXPAND | wxBOTTOM, 10);

	grid = new wxGrid(pnl, wxID_ANY);
	grid->CreateGrid(0, 4);

	string first_str;
	string operation;
	string name;
	string date_time;

	int count = -1;

	std::ifstream fin;

	if (isEnterAaudit) {
		fin.open("audit.txt");
	}
	else {
		fin.open("changes_audit.txt");
	}

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

	v_box->Add(grid, 1, wxEXPAND | wxBOTTOM, 10);
	wxButton* btn_ok = new wxButton(pnl, wxID_OK);
	btn_box->Add(btn_ok, 0, wxRight, 5);
	v_box->Add(btn_box, 0, wxALIGN_RIGHT);
	around_box->Add(v_box, 1, wxEXPAND | wxALL, 7);

	pnl->SetSizer(around_box);

	Centre();
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
		///////////////////////////////////////
		
		return ((s_op.IsEmpty() || a.adminOperation == s_op) && (s_name.IsEmpty() || a.userName == s_name) &&
				(s_dtstart.IsEmpty() && s_dtend.IsEmpty() 
					|| s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtend, '<')
					|| !s_dtstart.IsEmpty() && s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, '>')
					|| !s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, ' ', s_dtend)
				));
		
		// Попробовать через Enum:
		/*
		enum class {true, first, second, pair}
		sl.IsEmpty() + 2*sr.Empty() - вычисляем побитово одно из значений: 00, 01, 10, 11
		Затем передаем получившееся значение в cmp_time()
		*/
		///////////////////////////////////////
		
	});
	while (it != audits.end()) {
		res.push_back(*it);
		it = find_if(it + 1, audits.end(), [s_op, s_name, s_dtstart, s_dtend](Audit& a) {
			///////////////////////////////////

			return ((s_op.IsEmpty() || a.adminOperation == s_op) && (s_name.IsEmpty() || a.userName == s_name) &&
				(s_dtstart.IsEmpty() && s_dtend.IsEmpty()
					|| s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtend, '<')
					|| !s_dtstart.IsEmpty() && s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, '>')
					|| !s_dtstart.IsEmpty() && !s_dtend.IsEmpty() && cmp_time(a.dateTime, s_dtstart, ' ', s_dtend)
					));

			// Попробовать через Enum:
			/*
			enum class {true, first, second, pair}
			sl.IsEmpty() + 2*sr.Empty() - вычисляем побитово одно из значений: 00, 01, 10, 11
			Затем передаем получившееся значение в cmp_time()
			*/

			////////////////////////////////////
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
			if (wxString_to_lowercase(userNameForSearch) == wxString_to_lowercase(wxString(wxT("ADMIN")))) {
			//if (userNameForSearch == wxT("ADMIN")) {
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

SetMinMaxDlg::SetMinMaxDlg() : wxDialog(NULL, wxID_ANY, wxT("Установка времени действия пароля"), wxPoint(-1, -1), wxSize(400, 125)) {
	wxPanel* pnl = new wxPanel(this, -1);

	wxBoxSizer* v_box = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* h_box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* h_box2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* around_box = new wxBoxSizer(wxVERTICAL);

	wxStaticText* t_min = new wxStaticText(pnl, wxID_ANY, wxT("Минимум (дней)"));
	wxStaticText* t_max = new wxStaticText(pnl, wxID_ANY, wxT("Максимум (дней)"));

	h_box1->Add(t_min, 0, wxEXPAND | wxRIGHT, 5);
	min = new wxTextCtrl(pnl, wxID_ANY);
	h_box1->Add(min, 1, wxEXPAND | wxRIGHT, 5);
	h_box1->Add(t_max, 0, wxEXPAND | wxRIGHT, 5);
	max = new wxTextCtrl(pnl, wxID_ANY);
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