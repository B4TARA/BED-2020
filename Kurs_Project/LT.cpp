#include "LT.h"
#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
using namespace Log;
using namespace std;

namespace LT
{
	LexTable Create(int size)                                                // функци€ создани€ таблицы лексем с параметром ћј —»ћјЋ№Ќџ… –ј«ћ≈–
	{
		LexTable* Table = new LexTable;                                      //создание обьекта структуры таблица лексем
		if (size > LT_MAXSIZE) throw ERROR_THROW(120);                       //проверка на допустимость размера и ошибка если размер больше чем указанный
		Table->maxsize = size;                                               //передача в максимальный размер параметра ћј —»ћјЋ№Ќџ… –ј«ћ≈–
		Table->size = 0;                                                     //текущий размер равен 0
		Table->table = new Entry[size];                                      //массив лексем с размером ћј —»ћјЋ№Ќџ… –ј«ћ≈–
		return *Table;                                                       //возвращаем обьект таблицы лексем уже инициализированный
	}

	void Add(LexTable& lextable, Entry entry)                                //передаетс€ таблица лексем lextable и сама лексема entry
	{
		if (lextable.size + 1 > lextable.maxsize)                            //если размер не соответствует 
			throw ERROR_THROW(121);                                          //выводим ошибку
		lextable.table[lextable.size] = entry;                               //помещаем нашу лексему в массив лексем
		lextable.size += 1;                                                  //увеличиваем размер таблицы
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}

	Entry writeEntry(Entry& entry, char lexema, int indx, int line) //передаетс€ пуста€ структура дл€ заполнени€ и возвращени€, лексема(буква дл€ обозначени€ слова), индекс, номер строки 
	{
		entry.lexema = lexema;
		entry.idxTI = indx;
		entry.sn = line;
		return entry;                                               //заполнение и возвращение структуры 
	}

	void showTable(LexTable lextable, Parm::PARM parm)		// вывод таблицы лексем
	{
		ofstream out(parm.out);
		int buf_sn = 1;
		int i = 0;
		bool endline = true;

		if (lextable.table[0].lexema == 'm')
		{
			out << "1." << "\t" << "m";
			i = 1;
		}
		else out << "1." << "\t";
		for (i; i < lextable.size; i++)
		{

			if (buf_sn < lextable.table[i].sn)
			{
				out << endl << lextable.table[i].sn << "." << "\t" << lextable.table[i].lexema;
			}
			else
			{
				out << lextable.table[i].lexema;
			}
			buf_sn = lextable.table[i].sn;
		}
		out.close();
	}
}