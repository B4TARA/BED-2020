#pragma once
#include "Error.h"
#include "Parm.h"
#define LEXEMA_FIXSIZE	1			// ������������� ������ �������
#define LT_MAXSIZE		4096		// ������������ ���������� ����� � ������� ������
#define LT_TI_NULLIDX	-1	// ��� �������� ������� ���������������
#define LEX_UNSIGNED	't'	// ������� ��� unsigned integer
#define LEX_INTEGER		't'	// ������� ��� integer
#define LEX_STRING		't'	// ������� ��� string
#define LEX_ID			'i'	// ������� ��� ��������������
#define LEX_LITERAL		'l'	// ������� ��� ��������
#define LEX_FUNCTION	'f'	// ������� ��� function
#define LEX_DECLARE		'd'	// ������� ��� declare
#define LEX_RETURN		'r'	// ������� ��� return
#define LEX_OUTPUT		'o'	// ������� ��� output
#define LEX_MAIN		'm'	// ������� ��� main
#define LEX_IF          '?' // ������� ��� if
#define LEX_CALL        'c' //������� ��� call

#define LEX_SAME		'~'// ������� ��� ==
#define LEX_NOTSAME		'!'// ������� ��� !=
#define LEX_MORE		'>'	// ������� ��� >
#define LEX_LESS		'<'	// ������� ��� <
#define LEX_SAMEMORE	']'// ������� ��� >=
#define LEX_SAMELESS	'['// ������� ��� <=

#define LEX_SEMICOLON	';'	// ������� ��� ;
#define LEX_COMMA		','	// ������� ��� ,
#define LEX_LEFTBRACE	'{'	// ������� ��� {
#define LEX_BRACELET	'}'	// ������� ��� }
#define LEX_LEFTTHESIS	'('	// ������� ��� (
#define LEX_RIGHTTHESIS	')'	// ������� ��� )
#define LEX_PLUS		'v'	// ������� ��� +

#define PLUS		    '+'	// ������� ��� +
#define LEX_MINUS		'v'	// ������� ��� -
#define MINUS	     	'-'	// ������� ��� -
#define LEX_STAR		'v'	// ������� ��� *
#define STAR		    '*'	// ������� ��� *
#define LEX_DIRSLASH	'v'	// ������� ��� /
#define DIRSLASH	    '/'	// ������� ��� /
#define LEX_OPERATOR	'v'	// ������� ��� ����������
#define LEX_EQUAL		'=' // ������� ��� =

namespace LT
{
	struct Entry      //��������� �������
	{
		char lexema;  //�������
		int sn;       //����� ������ � �������� ������
		int idxTI = -1;   //������ � ��
		int priority; 
		int position;
		Entry(const char lex, int str_n, int idxTI = -1)
		{
			this->lexema = lex;
			this->sn = str_n;
			this->idxTI = idxTI;
		}
		Entry()
		{
			this->lexema = '\0';
			this->sn = 0;
			this->idxTI = -1;
			this->priority = 0;
			this->position = 0;
		}
	};
	struct LexTable   //��������� ������� ������
	{
		int maxsize;  //������������ ������
		int size;     //������� ������
		Entry* table; //������� � �������
	};

	LexTable Create(int size); //������� �������� ������� ������ � ���������� ������������ ������
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
	Entry writeEntry(Entry& entry, char lexema, int indx, int line);
	void showTable(LexTable lextable, Parm::PARM parm);
}
