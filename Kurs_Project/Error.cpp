#include "Error.h"
namespace Error
{
	// серии ошибок: 0-99 - системные ошибки
    //				 100-109 - ошибки параметров
    //				 110-119 - ошибки открытия и чтения файлов
    //				 120-129 - лексические ошибки
	ERROR error[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0,	"Недопустимый код ошибки"),
		ERROR_ENTRY(1,	"Системный сбой"),
		ERROR_ENTRY(2, "У вас нет main"),
		ERROR_ENTRY(3, "У вас несколько main"),
		ERROR_ENTRY(4, "У вас нет return"),ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6),ERROR_ENTRY_NODEF(7),ERROR_ENTRY_NODEF(8),ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),ERROR_ENTRY_NODEF10(20),ERROR_ENTRY_NODEF10(30),ERROR_ENTRY_NODEF10(40),ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60),ERROR_ENTRY_NODEF10(70),ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100,	"Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101),ERROR_ENTRY_NODEF(102),ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104,	"Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105),ERROR_ENTRY_NODEF(106),ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),ERROR_ENTRY_NODEF(109),

		ERROR_ENTRY(110,	"Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111,	"Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112,	"Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(113,	"Ошибка при создании файла выхода (-out)"),
		ERROR_ENTRY_NODEF(114),
		ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117),
		ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),

		ERROR_ENTRY(120,	"Таблица лексем переполнена"),
		ERROR_ENTRY(121,	"Слишком большое имя переменной или недопустимое имя переменной"),
		ERROR_ENTRY(122,	"Таблица идентификаторов переполнена."),
		ERROR_ENTRY(123,	"Нераспознанная лексема"),
		ERROR_ENTRY(124,	"Нарушен формат строкового литерала"),
		ERROR_ENTRY(125,	"Перезапись идентификатора"),
		ERROR_ENTRY(126,	"Неизвестная переменная"),
		ERROR_ENTRY(127,	"Превышена длина строкового литерала"),
		ERROR_ENTRY(128,	"Не удалось создать файл с лексемами или идентификаторами"),
		ERROR_ENTRY(129,	"Повторное объявление main"),

		ERROR_ENTRY(130,	"Отсутствие ключевого слова declare при объявлении переменной"),
		ERROR_ENTRY(131,    "Такая строка уже есть"),
		ERROR_ENTRY(132, "Имя идентификатора не может быть ключевым словом"),
		ERROR_ENTRY(133, "Найдено ключевое слово call без соответствующих функций"),
		ERROR_ENTRY(134, "Не найдено ключевого слова call при вызове функций даты/времени"),ERROR_ENTRY_NODEF(135),
		ERROR_ENTRY(136, "Нельзя присваивать значение функции"),
		ERROR_ENTRY(137, "Неверное количество параметров data/time"),
		ERROR_ENTRY(138, "Неверное количество параметров вызываемой функции или неверные параметры"),
		ERROR_ENTRY(139, "Тип функции и тип возвращаемого значения отличаются"),
		ERROR_ENTRY(140, "Несоответствие присваемого типа типу переменной"),
		ERROR_ENTRY(141, "Функция не возвращает значение"),
		ERROR_ENTRY(142, "Нарушена структура программного блока"),
		ERROR_ENTRY(146, "Неверный диапазон значений int"),
		ERROR_ENTRY(147, "Неверный диапазон значений unsigned int"),
		ERROR_ENTRY(149, "Нарушение формы логического выражения"),
		ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160),ERROR_ENTRY_NODEF10(170),
		ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),

		ERROR_ENTRY_NODEF100(200),
		ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),
		ERROR_ENTRY_NODEF100(500),

		ERROR_ENTRY(600,"Неверная структура программы"),
		ERROR_ENTRY(601,"Ошибочный оператор"),
		ERROR_ENTRY(602,"Ошибка в выражении"),
		ERROR_ENTRY(603,"Ошибка в параметрах функции"),
		ERROR_ENTRY(604,"Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY_NODEF(605),
		ERROR_ENTRY_NODEF(606),
		ERROR_ENTRY_NODEF(607),
		ERROR_ENTRY_NODEF(608),
		ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610),ERROR_ENTRY_NODEF10(620),ERROR_ENTRY_NODEF10(630),ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660),ERROR_ENTRY_NODEF10(670),ERROR_ENTRY_NODEF10(680),
		ERROR_ENTRY_NODEF10(690),

		ERROR_ENTRY_NODEF100(700),
		ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900)
	};
	ERROR geterror(int id)
	{
		ERROR er;
		if (id < 0 || id > ERROR_MAX_ENTRY)
			er = error[0];
		else
			er = error[id];
		return er;
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		ERROR er;
		if (id < 0 || id > ERROR_MAX_ENTRY)
			er = error[0];
		else
		{
			er = error[id];
			er.inext.line = line;
			er.inext.col = col;
		}
		return er;
	}
};