#pragma once
#include "wx/wx.h"
#include <vector>

// Структура пользователя
struct User {
	User(wxString _name, wxString _psw, bool _is_block, bool _is_limit, int _min_pswtime, int _max_pswtime, std::vector<wxString> _list_of_psw) {
		name = _name;
		psw = _psw;
		is_block = _is_block;
		is_limit = _is_limit;
		min_pswtime = _min_pswtime;
		max_pswtime = _max_pswtime;
		pswds_list = _list_of_psw;
	}

	wxString name;		// Имя
	wxString psw;		// Пароль
	bool is_block;		// флаг блокировки
	bool is_limit;		// флаг ограничения на пароль

	int min_pswtime;	// Минимальный срок действия пароля (пока в секундах)
	int max_pswtime;	// Максимальный срок действия пароля (пока в секундах)

	//int last_changepsw;	// Время последней смены пароля пользователем

	std::vector<wxString> pswds_list;	// Список уже использованных паролей
};