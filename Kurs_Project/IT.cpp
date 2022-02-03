#include "stdafx.h"
#include "IT.h"
#include "Error.h"
#include<iomanip>

namespace IT
{
	IdTable Create(int size)                                 //функция создания таблицы идентификаторов с параметром МАКСИМАЛЬНЫЙ РАЗМЕР таблицы
	{
		IdTable* tabl = new IdTable;                         //создание обьекта структуры таблицы идентификаторов
		if (size > TI_MAXSIZE) throw ERROR_THROW(160);       //проверка на допустимость размера и ошибка если размер больше указанного
		tabl->maxsize = size;                                //записываем в поле максимального размера параметр МАКСИМАЛЬНЫЙ РАЗМЕР
		tabl->size = 0;                                      //текущий размер таблицы
		tabl->table = new Entry[size];                       //создание массива идентификаторов с размером МАКСИМАЛЬНЫЙ РАЗМЕР
		return*tabl;                                         //возвращаем обьект структуры таблицы идентификаторов
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size + 1 > idtable.maxsize)
			throw ERROR_THROW(161);

		idtable.table[idtable.size] = entry;
		idtable.size += 1;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE])         //функция поиска такого идентификатора
	{
		for (int i = 0; i < idtable.size; i++)              //проходим по таблице идентификаторов
		{
			if (strcmp(idtable.table[i].id, id) == 0) return i;//если нашли такой же(метод strcmp вернул 0) то возвращаем место где находится тот же
		}

		return TI_NULLIDX;                                  //если не нашли то возвращаем нулевой индекс
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}
	void showTable(IdTable& idtable)
	{
		int i, numberOP = 0;
		std::cout << std::setfill('-') << std::setw(87) << '-' << std::endl;
		std::cout << "   №" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение    " << std::endl;
		std::cout << std::setw(87) << '-' << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			std::cout << std::setfill('0') << std::setw(4) << std::right << i << " | ";
			std::cout << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | ";
			switch (idtable.table[i].iddatatype)
			{
			case INT: std::cout << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP || strcmp(idtable.table[i].id, "main") == 0)
				{
					std::cout << "-" << " | ";
				}
				else std::cout << "integer" << " | "; break;
			case STR: std::cout << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else std::cout << "string" << " | "; break;
			case UNS: std::cout << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else std::cout << "unsigned" << " | "; break;
			case VOI: std::cout << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP) std::cout << "-" << " | ";
				else std::cout << "void" << " | "; break;
			default: std::cout << std::setw(10) << std::left << "unknown" << " | "; break;
			}
			switch (idtable.table[i].idtype)
			{
			case V: std::cout << std::setw(18) << std::left << "переменная" << " | "; break;
			case F: std::cout << std::setw(18) << std::left << "функция" << " | "; break;
			case P: std::cout << std::setw(18) << std::left << "параметр" << " | "; break;
			case L: std::cout << std::setw(18) << std::left << "литерал" << " | "; break;
			case OP: std::cout << std::setw(18) << std::left << "оператор" << " | ";
				numberOP++;
				break;
			default: std::cout << std::setw(18) << std::left << "unknown" << " | "; break;
			}
			std::cout << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | ";
			if (idtable.table[i].iddatatype == INT && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == STR && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else
				std::cout << "-";
			std::cout << std::endl;
		}
		std::cout << std::setfill('-') << std::setw(87) << '-' << std::endl;
		std::cout << "Количество идентификаторов: " << i - numberOP << std::endl;
		std::cout << std::setw(87) << '-' << std::endl; 
		std::setfill(' ');
	}
};