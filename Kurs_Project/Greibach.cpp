#include "stdafx.h"
#include <cstring>
#include "Rule.h"

namespace GRB
{

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)	// конструктор
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i) nt[i] = (GRBALPHABET)p[i];
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) // конструктор
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...) // конструктор
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};

	Greibach getGreibach() { return greibach; }; //метод структуры Greibach, возвращающий структуру, реализуемую с помощью конструктора с параметрами

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) // функция получить правило, возвращающая номер правила или -1, которая принимает вершину стека и элемент структуры Rule для правила
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn) k++; //цикл по прохождению каждого правила и сопоставления переданной вершины стека с нетерминалом(левой частью правила)
		if (k < size) prule = rules[rc = k];        //если у нас не выходит за пределы множества правил то в нашу переменную структуры Rule присваиваем правило под номером который нашли
		return rc;                                  //возвращаем номер правила
	};

	Rule Greibach::getRule(short n)	// // получить правило по номеру
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)		// полчить правило в виде N->цепочка с параметрами *буфер* и *номер цепочки (правой части) в правиле*
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00;  //создание строки для вывода
		chains[nchain].getCChain(bchain);                                         //берем правило 
		strcat_s(b, sizeof(bchain) + 5, bchain);                                  //соединяем вместе 
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) // получить следующую за j подходящую цепочку, вернуть её номер или -1
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;		// цикл по всем цепочкам в правиле(пока самый первый терминал или нетерминал каждой цепочки не равен t(тип) мы проходим по цепочкам)
		rc = (j < size ? j : -1);                           //если j меньше размера возвращаем j, а если нет возвращаем -1 
		if (rc >= 0) pchain = chains[rc];                   //если j меньше размера наша цепочка инициализируется в цепочку под номером j
		return rc;                                          //возвращаем rc(для проверки больше или меньше 0)
	};

	char* Rule::Chain::getCChain(char* b)	// получить правую сторону правила (цепочку)
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};

}