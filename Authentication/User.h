#pragma once
#include "wx/wx.h"
#include <vector>
#include <ctime>
#include <chrono>

// Структура пользователя
struct User {
	User(wxString _name, wxString _psw, bool _is_block, bool _is_limit, int _min_pswtime, int _max_pswtime, std::vector<wxString> _list_of_psw, int _num_of_pswds, time_t _last_changepsw) {
		name = _name;
		psw = _psw;
		is_block = _is_block;
		is_limit = _is_limit;
		min_pswtime = _min_pswtime;
		max_pswtime = _max_pswtime;
		pswds_list = _list_of_psw;
		num_of_pswds = _num_of_pswds;
		last_changepsw = _last_changepsw;
	}

	wxString name;						// Имя
	wxString psw;						// Пароль
	bool is_block;						// флаг блокировки
	bool is_limit;						// флаг ограничения на пароль

	int min_pswtime;					// Минимальный срок действия пароля (в секундах)
	int max_pswtime;					// Максимальный срок действия пароля (в секундах)

	time_t last_changepsw;				// Время последней смены пароля пользователем

	std::vector<wxString> pswds_list;	// Список уже использованных паролей
	int num_of_pswds;					// Максимальная длина списка уже использованных пользователем паролей(задается админом)
};