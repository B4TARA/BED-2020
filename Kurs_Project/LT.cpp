#include "LT.h"
#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
using namespace Log;
using namespace std;

namespace LT
{
	LexTable Create(int size)                                                // ������� �������� ������� ������ � ���������� ������������ ������
	{
		LexTable* Table = new LexTable;                                      //�������� ������� ��������� ������� ������
		if (size > LT_MAXSIZE) throw ERROR_THROW(120);                       //�������� �� ������������ ������� � ������ ���� ������ ������ ��� ���������
		Table->maxsize = size;                                               //�������� � ������������ ������ ��������� ������������ ������
		Table->size = 0;                                                     //������� ������ ����� 0
		Table->table = new Entry[size];                                      //������ ������ � �������� ������������ ������
		return *Table;                                                       //���������� ������ ������� ������ ��� ������������������
	}

	void Add(LexTable& lextable, Entry entry)                                //���������� ������� ������ lextable � ���� ������� entry
	{
		if (lextable.size + 1 > lextable.maxsize)                            //���� ������ �� ������������� 
			throw ERROR_THROW(121);                                          //������� ������
		lextable.table[lextable.size] = entry;                               //�������� ���� ������� � ������ ������
		lextable.size += 1;                                                  //����������� ������ �������
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}

	Entry writeEntry(Entry& entry, char lexema, int indx, int line) //���������� ������ ��������� ��� ���������� � �����������, �������(����� ��� ����������� �����), ������, ����� ������ 
	{
		entry.lexema = lexema;
		entry.idxTI = indx;
		entry.sn = line;
		return entry;                                               //���������� � ����������� ��������� 
	}

	void showTable(LexTable lextable, Parm::PARM parm)		// ����� ������� ������
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