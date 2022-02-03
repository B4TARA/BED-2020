#pragma once
#include "Error.h"
#include "Parm.h"
#define LEXEMA_FIXSIZE	1			// фиксированный размер лексемы
#define LT_MAXSIZE		4096		// максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX	-1	// нет элемента таблицы идентификаторов
#define LEX_UNSIGNED	't'	// лексема для unsigned integer
#define LEX_INTEGER		't'	// лексема для integer
#define LEX_STRING		't'	// лексема для string
#define LEX_ID			'i'	// лексема для идентификатора
#define LEX_LITERAL		'l'	// лексема для литерала
#define LEX_FUNCTION	'f'	// лексема для function
#define LEX_DECLARE		'd'	// лексема для declare
#define LEX_RETURN		'r'	// лексема для return
#define LEX_OUTPUT		'o'	// лексема для output
#define LEX_MAIN		'm'	// лексема для main
#define LEX_IF          '?' // лексема для if
#define LEX_CALL        'c' //лексема для call

#define LEX_SAME		'~'// лексема для ==
#define LEX_NOTSAME		'!'// лексема для !=
#define LEX_MORE		'>'	// лексема для >
#define LEX_LESS		'<'	// лексема для <
#define LEX_SAMEMORE	']'// лексема для >=
#define LEX_SAMELESS	'['// лексема для <=

#define LEX_SEMICOLON	';'	// лексема для ;
#define LEX_COMMA		','	// лексема для ,
#define LEX_LEFTBRACE	'{'	// лексема для {
#define LEX_BRACELET	'}'	// лексема для }
#define LEX_LEFTTHESIS	'('	// лексема для (
#define LEX_RIGHTTHESIS	')'	// лексема для )
#define LEX_PLUS		'v'	// лексема для +

#define PLUS		    '+'	// лексема для +
#define LEX_MINUS		'v'	// лексема для -
#define MINUS	     	'-'	// лексема для -
#define LEX_STAR		'v'	// лексема для *
#define STAR		    '*'	// лексема для *
#define LEX_DIRSLASH	'v'	// лексема для /
#define DIRSLASH	    '/'	// лексема для /
#define LEX_OPERATOR	'v'	// лексема для операторов
#define LEX_EQUAL		'=' // лексема для =

namespace LT
{
	struct Entry      //структура лексемы
	{
		char lexema;  //лексема
		int sn;       //номер строки в исходном тексте
		int idxTI = -1;   //индекс в ТИ
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
	struct LexTable   //структура таблицы лексем
	{
		int maxsize;  //максимальный размер
		int size;     //текущий размер
		Entry* table; //лексема в таблице
	};

	LexTable Create(int size); //функция создания таблицы лексем с параметром МАКСИМАЛЬНЫЙ РАЗМЕР
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
	Entry writeEntry(Entry& entry, char lexema, int indx, int line);
	void showTable(LexTable lextable, Parm::PARM parm);
}
