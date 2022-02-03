#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "out.h"
#include "Parm.h"
#include "LexAn.h"
#include "FST.h"
#include "Graphs.h"
#include "Error.h"
#define SAME '=='
#define NOTSAME '!='
#define MORE '>'
#define LESS '<'
#define SAMEMORE '>='
#define SAMELESS '<='
#define DIV '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='

using namespace std;
namespace Lex
{
	LEX lexAnaliz(Log::LOG log, In::IN in, Parm::PARM parm)                             //функция лексический анализатор
	{
		LEX lex;                                                                        //обьект структуры лексема
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);                                 //функция возвращает инициализированную структуру лексем и она присваивается новому обьекту структуры таблица лексем
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);                                   //функция возвращает инициализированную структуру идентификаторов и она присваивается новому обьекту структуры таблица идентификатров

		OUT out = getout(parm.out);                                                     //функция возвращает инициализированную структуру OUT и она присваивается новому обьекту этой структуры
		out_in_file(in.text, in.size, out);                                             //функция записи в файл с корректированием исходного текста
		Close(out);                                                                     //закрываем поток

		OUT out_2 = getout_2(parm.out);                                                 //функция возвращает инициализированную структуру OUT и она присваивается новому обьекту этой структуры
		out_of_file(out_2);                                                             //функция чтения из файла где у нас отредактированный текст и сразу создаем массив лексем
		Close_out(out_2);                                                               //закрываем поток

		int indxLex = 0;		// индекс лексемы
		int indexID = 0;		// индекс идент.
		int clit = 1;			// счетчик литералов
		int line = 1;			// номер строки
		int position = 0;	    // номер позиции в исходном файле
		int sn = 0;

		bool findMain = false;
		bool findReturn = false;
		bool findLibryary = false;
		int countMain = 0;
		int braces[2] = { 0, 0 };


		char emptystr[] = "";	// пустая строка
		char* prefix = new  char[10]{ "" };	// текущий префикс
		char* bufprefix = new  char[10]{ "" };	// буфер для префикса
		char* ifprefix = new  char[3]{ "if" };
		char* mainprefix = new char[5]{ "main" };
		char* oldprefix = new  char[10]{ "" };	// предыдущий префикс
		char* L = new  char[2]{ "L" };
		char* bufL = new  char[TI_STR_MAXSIZE];
		char* nameLiteral = new  char[10]{ "" };
		char* charclit = new char[10]{ "" };		// для строковой записи счетчика

		bool findFunc = false;
		bool findParm = false;
		cout << out_2.word[166];


		IT::Entry entryIT;
		bool findDeclare = false;
		bool findCall = false;
		bool findIf = false;
		int i = 0;
		for (i; out_2.word[i][0] != '\0'; i++, indxLex++)
		{

			bool findSameID = false;

            #pragma region FST_CALL
			FST::FST fstCall(out_2.word[i], FST_CALL);
			if (FST::execute(fstCall))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_CALL, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				findCall = true;
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion
			
            #pragma region FST_DECLARE
			FST::FST fstDeclare(out_2.word[i], FST_DECLARE);
			if (FST::execute(fstDeclare))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_DECLARE, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				findDeclare = true;
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_UNSIGNED
			FST::FST fstTypeUnsigned(out_2.word[i], FST_UNSIGNED);
			if (FST::execute(fstTypeUnsigned))
			{
				if (strcmp(out_2.word[i + 1], "integer") == 0)
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_UNSIGNED, LT_TI_NULLIDX, line);
					entryLT.position = position;
					LT::Add(lextable, entryLT);
					entryIT.iddatatype = IT::UNS;
					i++;
					position += strlen(out_2.word[i]);
					continue;
				}
			}
            #pragma endregion

            #pragma region FST_INTEGER
			FST::FST fstTypeInteger(out_2.word[i], FST_INTEGER);
			if (FST::execute(fstTypeInteger))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line); 
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				entryIT.iddatatype = IT::INT;
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_STRING
			FST::FST fstTypeString(out_2.word[i], FST_STRING);                             //если стринг
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::STR;
				strcpy(entryIT.value.vstr.str, emptystr);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_FUNCTION
			FST::FST fstFunction(out_2.word[i], FST_FUNCTION);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::F;
				findFunc = true;
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_RETURN
			FST::FST fstReturn(out_2.word[i], FST_RETURN);
			if (FST::execute(fstReturn))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				if (strcmp(prefix, mainprefix) == 0)
					findReturn = true;
				continue;
			}
            #pragma endregion

            #pragma region FST_OUTPUT
			FST::FST fstOutput(out_2.word[i], FST_OUTPUT);
			if (FST::execute(fstOutput)) 
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OUTPUT, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion 

            #pragma region FST_DATE
			FST::FST fstDate(out_2.word[i], FST_DATE);
			if (FST::execute(fstDate))
			{
				findCall = false;
				findLibryary = true;
				int idx = IT::IsId(idtable, out_2.word[i]);	                              // ищем такой же идентификатор
				if (idx != TI_NULLIDX)		                                              // если такой идентификатор уже есть
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);   
					entryLT.position = position;
					LT::Add(lextable, entryLT);
					position += strlen(out_2.word[i]);
					continue;                                            
				}
				else
				{

					LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
					entryLT.position = position;
					LT::Add(lextable, entryLT);
					
					strcpy(entryIT.id, out_2.word[i]);
					entryIT.idxfirstLE = indxLex;                                                         
					entryIT.idtype = IT::F;
					entryIT.iddatatype = IT::VOI;
					IT::Add(idtable, entryIT);
					position += strlen(out_2.word[i]);
					continue;
				}
			}
            #pragma endregion    //разобрано

            #pragma region FST_TIME
			FST::FST fstTime(out_2.word[i], FST_TIME);
			if (FST::execute(fstTime))
			{
				findCall = false;
				findLibryary = true;
				int idx = IT::IsId(idtable, out_2.word[i]);	                              // ищем такой же идентификатор
				if (idx != TI_NULLIDX)		                                              // если такой идентификатор уже есть
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);  
					entryLT.position = position;
					LT::Add(lextable, entryLT);
					position += strlen(out_2.word[i]);
					continue;
				}
				else
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
					entryLT.position = position;
					LT::Add(lextable, entryLT);

					strcpy(entryIT.id, out_2.word[i]);
					entryIT.idxfirstLE = indxLex;
					entryIT.idtype = IT::F;
					entryIT.iddatatype = IT::VOI;
					IT::Add(idtable, entryIT);
					position += strlen(out_2.word[i]);
					continue;
				}
			}
            #pragma endregion    //разобрано

            #pragma region FST_IF
			FST::FST fstIf(out_2.word[i], FST_IF);
			if (FST::execute(fstIf))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_IF, LT_TI_NULLIDX, line);
				entryLT.position = position;
				entryLT.idxTI = indexID++;
				LT::Add(lextable, entryLT);
				findIf = true;
				strcpy(entryIT.id, out_2.word[i]);
				entryIT.idxfirstLE = indxLex;                                                          //место начиная с начала
				entryIT.idtype = IT::OP;
				IT::Add(idtable, entryIT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_MAIN
			FST::FST fstMain(out_2.word[i], FST_MAIN);
			if (FST::execute(fstMain))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
				entryLT.position = position;
				entryLT.idxTI = indexID++;
				LT::Add(lextable, entryLT);
				strcpy(oldprefix, prefix);
				strcpy(prefix, out_2.word[i]);
				strcpy(entryIT.visibility, emptystr);
				findMain = true;
				countMain++;
				strcpy(entryIT.id, out_2.word[i]);
				entryIT.iddatatype = IT::INT;
				entryIT.idxfirstLE = indxLex;
				entryIT.idtype = IT::F;
				IT::Add(idtable, entryIT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion    //разобрано

            #pragma region FST_ID
			FST::FST fstIdentif(out_2.word[i], FST_ID);                                      //создание цепочки по автомату     
			if (FST::execute(fstIdentif))                                                    //сверка 
			{
				int counter_size = 0;
				for (int counter_siz = 0; out_2.word[i][counter_siz] != '\0'; counter_siz++)
					counter_size++;
				if (counter_size > 10)
					out_2.word[i][10] = '\0';
				if (findFunc)	                                                             // если был поднят флаг функции
				{
					int idx = IT::IsId(idtable, out_2.word[i]);	                             // ищем такой же идентификатор
					if (idx != TI_NULLIDX)		                                             // если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);          //создание обьекта структуры entryLT и его заполнение
						entryLT.position = position;
						LT::Add(lextable, entryLT);                                          //добавляем в таблицу лексем
						findFunc = false;                                                    //опускаем флажок нахождения функции
						position += strlen(out_2.word[i]);
						continue;
					}
				}


				else                                                                          //если флаг функции не был поднят 
				{
					int idx = IT::IsId(idtable, out_2.word[i]);	                              // ищем такой же идентификатор
					if (idx != TI_NULLIDX)		                                              // если такой идентификатор уже есть
					{


						if (findDeclare == true) throw ERROR_THROW_IN(125, line, position);
						findDeclare = false;


						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);           //создание обьекта структуры entryLT и его заполнение
						entryLT.position = position;
						LT::Add(lextable, entryLT);                                           //добавляем в таблицу лексем 
						findFunc = false;                                                     //опускаем флажок нахождения функции
						position += strlen(out_2.word[i]);
						continue;                                                             //продолжаем
					}
					if (findIf && findDeclare)
					{
						out_2.word[i] = strcat(ifprefix, out_2.word[i]);
					}
					else
					{
						strcpy(bufprefix, prefix);                                                //если идентификатора нет то копируем префикс в буфер
						out_2.word[i] = strcat(bufprefix, out_2.word[i]);                         //теперь наша лексема это обьединение префикса(названия) с идентификатором или переменной
					}
					idx = IT::IsId(idtable, out_2.word[i]);                                   //ищем с префиксом
					if (idx != TI_NULLIDX)		                                              // если такой идентификатор уже есть
					{
						if (findDeclare == true) throw ERROR_THROW_IN(125, line, position);
						findDeclare = false;
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);           //добавляем его в таблицу лексем
						entryLT.position = position;
						LT::Add(lextable, entryLT);
						position += strlen(out_2.word[i]);
						continue;
					}
				}

				//у нас поднят флаг функции и мы не нашли такого же идентификатора
				if(findDeclare == false && findFunc == false && findParm == false) throw ERROR_THROW_IN(126, line, position);
				findDeclare = false;
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);             //добавляем в таблицу лексем
				LT::Add(lextable, entryLT);
				if (findParm) {		                                                          // если параметр
					entryIT.idtype = IT::P;                                                   //указываем тип идентификатора - параметр
					strcpy(entryIT.visibility, prefix);                                       //копируем префикс(название) в поле видимость(только эта функция)
					if (entryIT.iddatatype == IT::INT)                                        //если тип переменной int 
						entryIT.value.vint = TI_INT_DEFAULT;                                  //указываем значение по умолчанию для значения переменной
					if (entryIT.iddatatype == IT::UNS)                                        //если тип переменной int 
						entryIT.value.vuint = TI_UNS_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else if (!findFunc) {	                                                      // если переменная
					entryIT.idtype = IT::V;                                                   //указываем тип идентификатора - переменная
					strcpy(entryIT.visibility, prefix);                                       //копируем префикс
					if (entryIT.iddatatype == IT::INT)                                        //если тип переменной int 
						entryIT.value.vint = TI_INT_DEFAULT;                                  //указываем значение по умолчанию для значения переменной
					if (entryIT.iddatatype == IT::UNS)                                        //если тип переменной int 
						entryIT.value.vuint = TI_UNS_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else {		                                                                  // если функция
					strcpy(oldprefix, prefix);                                                //уопируем префикс
					strcpy(prefix, out_2.word[i]);                                            //в префикс копируем название функции
					strcpy(entryIT.visibility, emptystr);                                     //копируем пустую строку
				}

				entryIT.idxfirstLE = indxLex;                                                  //индекс первой строки в таблице лексем 
				strcpy(entryIT.id, out_2.word[i]);                                             //копируем название функции в поле id 
				IT::Add(idtable, entryIT);                                                     //добавляем обьект структуры идентификаторов в таблицу 
				findFunc = false;                                                              //опускаем флаг функции
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_INTLIT
			FST::FST fstLiteralInt(out_2.word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt))
			{
				int value = atoi((char*)out_2.word[i]);	// значение литерала
		
				
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.value.vuint = value;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				strcpy(bufL, L);	// помещаем в буфер "L"
				out_2.word[i] = strcat(bufL, (char*)charclit);	// формируем имя для литерала
				strcpy(entryIT.id, out_2.word[i]);
				IT::Add(idtable, entryIT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_STRLIT
			FST::FST fstLiteralString(out_2.word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString))
			{
				// запись значения
				int length = strlen(out_2.word[i]);
				for (int k = 0; k < length; k++)	// перезапись массива, убираем кавычки
					out_2.word[i][k] = out_2.word[i][k + 1];
				out_2.word[i][length - 2] = 0;
				
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				strcpy(entryIT.value.vstr.str, out_2.word[i]);	// запись значиния строкового литерала
				entryIT.value.vstr.len = length - 2;		// запись длины строкового литерала
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				strcpy(bufL, L);	// помещаем в буфер "L"
				nameLiteral = strcat(bufL, (char*)charclit);	// формируем имя для литерала (L + charclit)
				strcpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_OPERATOR
			FST::FST fstOperator(out_2.word[i], FST_OPERATOR);                                         //если какойто оператор 
			if (FST::execute(fstOperator))                                                             //проверка
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
				entryLT.position = position;
				switch (out_2.word[i][0])                                                              //проверка через свитч
				{
				case PLUS: 
					entryLT.priority = 2;
					entryIT.value.operation = '+';
					break;
				case MINUS:                                                                 //плюс или минус приоритет 2
					entryLT.priority = 2;
					entryIT.value.operation = '-';
					break;
				case DIRSLASH: 
					entryLT.priority = 3;
					entryIT.value.operation = '/';
					break;
				case STAR:                                                              //делить или умножить приоритет выше - 3
					entryLT.priority = 3;
					entryIT.value.operation = '*';
					break;
				}
				LT::Add(lextable, entryLT);                                                            //добавляем в таблицу лексем
				strcpy(entryIT.id, out_2.word[i]);                                                     //копируем в айди оператор
				entryIT.idxfirstLE = indxLex;                                                          //место начиная с начала
				entryIT.idtype = IT::OP;                                                               //тип идентификатора
				IT::Add(idtable, entryIT);                                                             //добавляем уже в таблицу идентификаторов
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_SAME
			FST::FST fstSame(out_2.word[i], FST_SAME);                                       // если ==
			if (FST::execute(fstSame))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SAME, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				i++;
				continue;
			}
            #pragma endregion

            #pragma region FST_NOTSAME
			FST::FST fstNotSame(out_2.word[i], FST_NOTSAME);                                       // если !=
			if (FST::execute(fstNotSame))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_NOTSAME, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				i++;
				continue;
			}
            #pragma endregion

            #pragma region FST_LESS
			FST::FST fstLess(out_2.word[i], FST_LESS);                                       // если <
			if (FST::execute(fstLess))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LESS, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_MORE
			FST::FST fstMore(out_2.word[i], FST_MORE);                                       // если >
			if (FST::execute(fstMore))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_MORE, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_SAMEMORE
			FST::FST fstSameMore(out_2.word[i], FST_SAMEMORE);                                       // если >=
			if (FST::execute(fstSameMore))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SAMEMORE, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				i++;
				continue;
			}
            #pragma endregion

            #pragma region FST_SAMELESS
			FST::FST fstSameLess(out_2.word[i], FST_SAMELESS);                                       // если <=
			if (FST::execute(fstSameLess))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SAMELESS, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				i++;
				continue;
			}
            #pragma endregion

            #pragma region FST_SEMICOLON
			FST::FST fstSemicolon(out_2.word[i], FST_SEMICOLON);                                       // если ;
			if (FST::execute(fstSemicolon))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_COMMA
			FST::FST fstComma(out_2.word[i], FST_COMMA);                                               //если запятая
			if (FST::execute(fstComma))                                                                //проверка
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);               //создание и инициализирование
				entryLT.priority = 1;                                                                  //приоритет 1
				entryLT.position = position;
				LT::Add(lextable, entryLT);                                                            //добавление в таблицу лексем
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_LEFTBRACE
			FST::FST fstLeftBrace(out_2.word[i], FST_LEFTBRACE);                                       //если {
			if (FST::execute(fstLeftBrace))                                                            //проверка
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);           //создание и инициализация обьекта структуры
				entryLT.position = position;
				LT::Add(lextable, entryLT);        
				position += strlen(out_2.word[i]);//добавление этого обьекта в таблицу лексем
				continue;
			}
            #pragma endregion

            #pragma region FST_BRACELET
			FST::FST fstRightBrace(out_2.word[i], FST_BRACELET);                                       //если }
			if (FST::execute(fstRightBrace))
			{
				findIf = false;
				LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_LEFTTHESIS
			FST::FST fstLeftThesis(out_2.word[i], FST_LEFTTHESIS);                                   // если (
			if (FST::execute(fstLeftThesis))                                                         // проверка
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);        //создание и инициализация с помощью функции 
				entryLT.position = position;
				entryLT.priority = 0;                                                                //приоритет
				LT::Add(lextable, entryLT);                                                          //добавляем структуры в таблицу лексем
				if (indexID != 0)
				if (idtable.table[indexID - 1].idtype == IT::F || idtable.table[indexID].idtype == IT::F) //если в таблице идентификаторов перед текущим находится функция то 
					findParm = true;    
				position += strlen(out_2.word[i]);//мы нашли параметры
				continue;
			}
            #pragma endregion

            #pragma region FST_RIGHTTHESIS
			FST::FST fstRightThesis(out_2.word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);        //если )
				entryLT.position = position;
				entryLT.priority = 0;                                                                 //приоритет 0
				if (findParm && out_2.word[i + 1][0] != LEX_LEFTBRACE && out_2.word[i + 2][0] != LEX_LEFTBRACE && out_2.word[i-1][0] != LEX_LEFTTHESIS && !findLibryary)		// если после функции нет {
					strcpy(prefix, oldprefix);		// возвращаем предыдущую обл. видимости
				findParm = false;                   //параметры мы уже использовали
				LT::Add(lextable, entryLT);         //добавляем ) в таблицу лексем
				position += strlen(out_2.word[i]);
				findLibryary = false;
				continue;
			}
            #pragma endregion

            #pragma region FST_EQUAL
			FST::FST fstEqual(out_2.word[i], FST_EQUAL);                                              //если =
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

			if (out_2.word[i][0] == DIV) 
			{
				line++;
				sn++;
				position = 0;
				indxLex--;
				continue;
			}



			throw ERROR_THROW_IN(123, line, position);
		}
		if(findCall) throw ERROR_THROW(133)
		if(findLibryary && !findCall) throw ERROR_THROW(134)
		if (!findMain) throw ERROR_THROW(2);
		if (countMain > 1) throw ERROR_THROW(3);
		if (!findReturn) throw ERROR_THROW(4);
		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;



	}

}