#pragma once
#include "wx/wx.h"
#include <vector>

// ��������� ������������
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

	wxString name;		// ���
	wxString psw;		// ������
	bool is_block;		// ���� ����������
	bool is_limit;		// ���� ����������� �� ������

	int min_pswtime;	// ����������� ���� �������� ������ (���� � ��������)
	int max_pswtime;	// ������������ ���� �������� ������ (���� � ��������)

	//int last_changepsw;	// ����� ��������� ����� ������ �������������

	std::vector<wxString> pswds_list;	// ������ ��� �������������� �������
};