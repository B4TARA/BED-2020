#include "MFST.h"
#include <iomanip>
#include <iostream>


namespace MFST
{
#pragma region CONSTRUCTORS
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis() //реализация конструктора без параметров для структуры диагностика(изначальное ее состояние), которая является дочерней структуры Mfst
	{
		lenta_position = -1;             //изначальная позиция на ленте = -1
		rc_step = SURPRISE;              //переменная возврата функции step и завершения шага равна неожиданному коду возврата(ошибка step)
		nrule = -1;                      //изначально номер правила = -1
		nrule_chain = -1;                //изначально номер цепочки правила равен -1 
	};
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	};
	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; }; //конструктор без параметров 
	Mfst::Mfst(LT::LexTable& lextable, GRB::Greibach pgrebach)    //конструктор с параметрами куда передается таблица лексем и указанный автомат правил
	{
		grebach = pgrebach; //создание переменной и инициализация автоматом
		lex = lextable;     //создание переменной и инициализация таблицей лексем
		lenta = new short[lenta_size = lex.size];  //выделение памяти для динамического массива lenta с размером равным размеру массива всех лексем
		for (int k = 0; k < lenta_size; k++)       //цикл для прохода по массиву lenta
			lenta[k] = GRB::Rule::Chain::T(lex.table[k].lexema); //создаем массив номеров лексем на основе таблицы лексем 
		lenta_position = 0;                                      //изначальная позиция будет равна 0
		st.push(grebach.stbottomT);                              //с помощью стандартной команды push кладем в наш стэк символ, являющийся дном стека
		st.push(grebach.startN);                                 //с помощью стандартной команды push кладем в наш стэк символ, являющийся началом стека поверх дна
		nrulechain = -1;                                         //номер текущей цепочки,текущего правила инициализируем в -1 
	}
#pragma endregion
	Mfst::RC_STEP Mfst::step()                         //функция проверки на какую-либо из ошибок и ее возврат
	{
		RC_STEP rc = SURPRISE;                         //переменная перечисления с изначальной инициализацией в неожиданный код возврата (ошибка в step)
		if (lenta_position < lenta_size)               //проверка на выход за пределы ленты(размер таблицы лексем)
		{
			if (GRB::Rule::Chain::isN(st.top()))       //берем вершину стека и проверяем, нетерминал ?
			{
				GRB::Rule rule;                        //создание переменной структуры Rule
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0) //номер текущего правила = номеру подходящего правилу, а если такого нет то выдаем ошибку в else 
				{
					GRB::Rule::Chain chain;                         //создание цепочки ( правой части правила)
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) //если номер текущей цепочки текущего правила больше либо равно 0
					{
						MFST_TRACE1                                 //выводим первую строку
							savestate(); st.pop(); push_chain(chain); rc = NS_OK;
						MFST_TRACE2
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")
							savediagnosis(NS_NORULECHAIN); rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;
					};
				}
				else rc = NS_ERROR;
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				lenta_position++; st.pop(); nrulechain = -1; rc = TS_OK;
				MFST_TRACE3
			}
			else { MFST_TRACE4(TS_NOK / NS_NORULECHAIN) rc = resetstate() ? TS_NOK : NS_NORULECHAIN; };
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4(LENTA_END);
		};
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]);
		return true;
	};

	bool Mfst::savestate()
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain)); //добавляем в стек для хранения состояний само состояние(структуру состояния)
		MFST_TRACE6("SAVESTATE:", storestate.size());
		return true;
	};

	bool Mfst::resetstate()
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESSTATE")
				MFST_TRACE2
		};
		return rc;
	};

	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
			k++;

		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
				diagnosis[i].lenta_position = -1;
		}

		return rc;
	};

	bool Mfst::start()          //булевая функция проверки на удовлетворение правилу
	{
		bool rc = false;                //флаг и инициализация в 0
		RC_STEP rc_step = SURPRISE;     //переменная перечисления с изначальной инициализацией в неожиданный код возврата (ошибка в step)(расчет на то что потом изменится)
		char buf[MFST_DIAGN_MAXSIZE]{}; //создаем буфер с максимальным размером диагностического сообщения
		rc_step = step();               //наша переменная перечисления равна тому, что вернет функция step()
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step();

		switch (rc_step)
		{
		case LENTA_END:
		{
			MFST_TRACE4("------>LENTA_END")
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lex.table[lex.size - 1].sn);
			std::cout << std::setw(4) << std::left << 0 << "всего строк " << lex.table[lex.size - 1].sn << ", синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;
		}

		case NS_NORULE:
		{
			MFST_TRACE4("------>NS_NORULE")
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			std::cout << getDiagnosis(0, buf) << std::endl;
			std::cout << getDiagnosis(1, buf) << std::endl;
			std::cout << getDiagnosis(2, buf) << std::endl;
			break;
		}

		case NS_NORULECHAIN:	MFST_TRACE4("------>NS_NORULECHAIN") break;
		case NS_ERROR:			MFST_TRACE4("------>NS_ERROR") break;
		case SURPRISE:			MFST_TRACE4("------>NS_SURPRISE") break;


		}
		return rc;
	};

	char* Mfst::getCSt(char* buf) //получить содержимое стека и передается сам буфер
	{
		short p;
		for (int k = (signed)st.size() - 1; k >= 0; --k) //проходим по стеку
		{
			p = st.c[k];                                 //с - специальный метод получения элемента под номером
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p); //присваиваем в буфер под соответствующим номером лексемы (обратно переводим ее из номера в лексему)
		}
		buf[st.size()] = '\0';  //конец массива символов это \0
		return buf;             //возвращаем получившуюся цепочку
	}
	char* Mfst::getCLenta(char* buf, short pos, short n) //лента: n символов с pos с параметрами, тут создается буфер самой строки(то что в out.txt)
	{
		short i = 0, k = (pos + n < lenta_size) ? pos + n : lenta_size;

		for (int i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);

		buf[i - pos] = '\0';
		return buf;
	}
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char* rc = new char[200]{};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lex.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}
	void Mfst::printrules()
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short i = 0; i < storestate.size(); i++) //цикл по стеку хранения состояний
		{
			state = storestate.c[i]; //i-тый элемент стека состояний
			rule = grebach.getRule(state.nrule); //получаем правило с помощью номера
			MFST_TRACE7
		};
	};

	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];

		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}
		return true;
	}
}
