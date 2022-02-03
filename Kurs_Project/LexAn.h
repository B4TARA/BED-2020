#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"
#include "In.h"
namespace Lex
{
	struct LEX                                                       //структура лексема
	{
		IT::IdTable idtable;                                         //поле структуры таблица идентификаторов
		LT::LexTable lextable;                                       //поле структуры таблица лексем
	};

	LEX lexAnaliz(Log::LOG log, In::IN in, Parm::PARM parm);         //функция лексический анализтаор
}

