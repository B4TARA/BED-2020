#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "LT.h"
#include "LexAn.h"
#include "MFST.h"
#include "FST.h"
#include "Greibach.h"
#include "PolishNotation.h"
#include "CodeGeneration.h"
#include "SemAn.h"

//таблица семантические проверки в пояснительной записке
//добавить стринг в семантическом анализе
//добавить operation в лексическом анализе
//нумеровать строки в лог файле
//Выяснить что там если нет return в СИНТАКСИЧЕСКОМ
//Ошибка нет реализации функции
//разобраться с классификацией ошибок 

//в лексическом анализе пофиксить зоны допустимости

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "ru");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getAllText(parm.in);
		Log::WriteIn(log, in);

		Lex::LEX lex = Lex::lexAnaliz(log, in, parm);

		IT::showTable(lex.idtable);
		LT::showTable(lex.lextable, parm);

		MFST_TRACE_START //основа таблицы 
		MFST::Mfst mfst(lex.lextable, GRB::getGreibach()); //функция mfst на вход которой подается таблица лексем с и функция которая возвращает автомат правил 
		mfst.start();                                      //после того как мы создали массив номеров лексем и необходимые элементы на основе таблицы лексем мы можем начинать проверку
		mfst.savededucation();
		mfst.printrules();

        #pragma region ОБРАБОТКА ВХОДНЫХ ВЫРАЖЕНИЙ
		int PNnumberGenerated = 0;
		for (int iter = 0, lt_position; iter < lex.lextable.size; iter++)
		{
			if (lex.lextable.table[iter - 1].lexema == LEX_EQUAL ||
				lex.lextable.table[iter - 1].lexema == LEX_CALL  ||
				lex.lextable.table[iter - 1].lexema == LEX_MORE  ||
				lex.lextable.table[iter - 1].lexema == LEX_LESS  ||
				lex.lextable.table[iter - 1].lexema == LEX_RETURN    ||
				lex.lextable.table[iter - 1].lexema == LEX_OUTPUT ||
				(lex.lextable.table[iter - 3].lexema == LEX_LEFTTHESIS &&
					lex.lextable.table[iter - 2].lexema == LEX_INTEGER &&
					lex.lextable.table[iter - 1].lexema == LEX_RIGHTTHESIS))
			{
				if (PN::PolishNotation(iter, lex.lextable, lex.idtable))
				{
					PNnumberGenerated++;
				}
			}
		}
        #pragma endregion
		LT::showTable(lex.lextable, parm);
		bool sem_ok = SemanticAnalyze(lex.lextable, lex.idtable);
		CG::ConstSegment CnstS(lex.idtable.size);
		CG::DataSegment Data(lex.idtable.size);
		CG::CodeSegment Code;
		CG::add(lex.idtable, CnstS);
		CG::add(lex.idtable, Data);
		CG::CodeSegmentGeneration(Code, lex.lextable, lex.idtable);
		CG::startGeneration(parm.out, CnstS, Data, Code);
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	}
	system("pause");
	return 0;
}



