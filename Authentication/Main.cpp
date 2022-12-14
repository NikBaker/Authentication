#include <wx/wx.h>
#include "AuthApp.h"

/*
* 
1.	Разработка программы парольной аутентификации с дополнительными средствами администрирования
Указание по теме 1: Дополнительные (по сравнению с лабораторной работой 1) средства администрирования:
+•	задание максимального и минимального сроков действия паролей пользователей,
+•	ведение списка уже использованных паролей каждого пользователя задаваемой администратором максимальной длины
	и проверка уникальности нового пароля этого пользователя,
+•	аудит удачных и неудачных попыток входа в программу и выхода из нее 
	с фиксацией времени события, его результата – успех 
	или неудача – и имени учетной записи пользователя в специальном файле,
+•	аудит изменений в файле учетных записей – добавления нового пользователя, 
	установки и снятия блокировки или ограничений на используемые пароли – с фиксацией времени события и его типа.

.......................................................................................................................

	+ ловеркейс,

	+ длина списка паролей (сейчас захардкожено число 10, нужно добавить каждому пользователю это число, которое будет задавать админ),
	+ мин макс время,	(добавить в структуру User поле для времени последней успешной смены пароля)
	
	Возможно добавить пользователя-аудитора, который будет отвечать за аудит

	+Возможность сохранять файл (сохранить как / сохранить) аудита 
	+при помощи стандартного окна (берем информацию из таблицы и заполняем заданный файл данными из таблицы).
	
	+Для администратора задать дополнительные поля (адресс файла аудита входа/выхода, адресс файла аудита изменений)
	+Добавить для администратора возможность очищать файлы аудита (после достижения предела длины файла, администратору обязательно нужно будет
	+очистить файл аудита, предварительно можно будет сохранить имеющийся файл).

	+Открытие файлов аудита должно происходить не через проводник (а например, через меню)

	///////////////////////////////////////////////////////////////////////////////////////
	Остается неясным, когда и по какой команде устанавливаются имена файлов
	аудита и как они просмпьриваются. Если через Проводник, то это не лучшее
	решение. Нкжны разные команды для задания имен файлов аудита и их просмотра.
	///////////////////////////////////////////////////////////////////////////////////////

	+При первом входе админа(аудитора) он задает имена для файлов аудита
	+Затем в меню он просто выбирает пункт "открыть файл аудита".
	+Если захочет очистить, выбирает пункт "очистить файл аудита".
	+Если захочет сохранить, выбирает пункт "сохранить файл аудита".

	+Добавить возможность изменения параметров файла аудита(например, администратор указывает максимальную длину файлов аудита)
	
	+Когда файл аудита переполнятся при следующем входе администратора ему необходимо очистить журнал.
	
	+Если пользователь входит после переполнения файла аудита:
		+1) Не записывать этот вход?
		+2) Выдавать сообщение о том, что на данный момент вход в систему невозможен?

	+шифрование паролей(взять из лабы),

	///////////////////////////////////////////////////////////////////////////////////////
	шифрование файлов с данными(взять из лабы, если понадобится),

	рефакторниг

*/

IMPLEMENT_APP(AuthApp)
