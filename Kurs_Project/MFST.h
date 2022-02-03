#pragma once

#include <stack>
#include "Greibach.h"
#include "LT.h"
#include <iomanip>

//template <class Type>
//class allocator;
//template <class Type, class Allocator = allocator<Type>>
//class deque;
//template <class _Ty, class _Container = deque<_Ty>>
//class My_Stack : public std::stack<_Ty, _Container>
//{
//public:
//	using std::stack<_Ty, _Container>::c;
//};

class my_stack_SHORT :public std::stack<short> {      //создаем класс, наследующий свойства и методы класса stack, чтобы внутри мы могли обьявить сам стек
public:
	using std::stack<short>::c;                       // стек автомата
};


#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];



#define MFST_TRACE_START std::cout<< std::setw(4)<<std::left<<"Шаг"<<":"\
								  << std::setw(20)<<std::left<<" Правило"\
								  << std::setw(30)<<std::left<<" Входная лента"\
								  << std::setw(20)<<std::left<<" Стек"\
								  << std::endl;

#define MFST_TRACE1		 std::cout<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<rule.getCRule(rbuf,nrulechain)\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE2		 std::cout<< std::setw(4)<<std::left<<FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE3		 std::cout<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE4(c)		std::cout<<std::setw(4)<<std::left << ++FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
#define MFST_TRACE5(c)		std::cout<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
#define MFST_TRACE6(c,k)	std::cout<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left << c << k <<std::endl;
#define MFST_TRACE7			std::cout<<std::setw(4)<<std::left << state.lenta_position << ": "\
							<<std::setw(20)<< std::left << rule.getCRule(rbuf,state.nrulechain)\
							<<std::endl;


typedef  my_stack_SHORT MFSTSTSTACK;    // тут мы переопределяем, что класс my_stack_SHORT теперь имеет название MFSTSTSTACK
namespace MFST
{
	struct MfstState					//состояние автомата для сохранения
	{
		short lenta_position;			//состояние автомата для сохранения
		short nrule;					//номер текущего правила
		short nrulechain;				//номер текущей цепчки, текущего правила
		MFSTSTSTACK st;					//стек автомата
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);	//(позиция на ленте;стек автомата; номер текущей цепочки текущего правила)
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);//(позиция на ленте;стек автомата; номер текущего правила; номер текущей цепочки текущего правила)

	};

	struct Mfst							//магазинный автомат
	{
		enum RC_STEP {					//код вовзрата функции step
			NS_OK,					//найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,				//не найдено правило грамматики(ошибка в грамматике)
			NS_NORULECHAIN,			//не найдена подходящая цепочка правила(ошибка в исходном коде)
			NS_ERROR,				//неизвествный нетерминальный символ грамматики
			TS_OK,					//тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,					//тек. символ ленты != вершине стека, продвинулась лента, pop стека
			LENTA_END,				//текущая позиция ленты >= lenta_size
			SURPRISE,				//неожиданный код возврата (ошибка в step)
		};
		struct MfstDiagnosis		//диагностика
		{
			short lenta_position;		//позиция на ленте
			RC_STEP rc_step;			//код завершения шага
			short nrule;				//номер правила
			short nrule_chain;			//номер цепочки правила
			MfstDiagnosis();			//конструктор без параметров
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain); //конструктор с параметрами

		}diagnosis[MFST_DIAGN_NUMBER]; 		// последние самые глубокие сообщения

		class my_stack_MfstState :public std::stack<MfstState> {
		public:
			using std::stack<MfstState>::c;
		};

		GRBALPHABET* lenta;					//перекодированныя (TN/NS) лента (из LEX)
		short lenta_position;				//текущая позиция на ленте
		short nrule;						//номер текущего правила
		short nrulechain;					//номер текущей цепочки,текущего правила
		short lenta_size;					//размер ленты
		GRB::Greibach grebach;				//создание элемента структуры Greibach, сюда присваивается параметр в дальнейшем
		LT::LexTable lex;					//результат работы лексического анализатора, сюда присваивается параметр в дальнейшем
		MFSTSTSTACK st;						//стек автомата
		my_stack_MfstState storestate;		//стек для хранения состояний
		Mfst();
		Mfst(LT::LexTable& plex, GRB::Greibach pgrebach); //конструктор с параметрами *таблица лексем* и *структура правил*
		char* getCSt(char* buf);			//получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);	//лента: n символов с pos
		char* getDiagnosis(short n, char* buf);					//получить n-ю строку диагностики или 0х00
		bool savestate();					//сохранить состояние автомата
		bool resetstate();					//восстановить состояние автомата
		bool push_chain(GRB::Rule::Chain chain);		//поместить цепочку правила в стек
		RC_STEP step();						//выполнить шаг автомата
		bool start();						//запустить автомат
		bool savediagnosis(RC_STEP pprc_step);			//код завершения шага
		void printrules();					//вывести послдеовательность правил

		struct Deducation //обьявление структуры вывода ( дочерняя от Mfst)
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation() //конструктор без параметров, внутри инициализация в 0
			{
				size = 0;
				nrules = 0;
				nrulechains = 0;
			};
		}deducation;

		bool savededucation();
	};
}


