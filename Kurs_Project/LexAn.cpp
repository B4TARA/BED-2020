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
	LEX lexAnaliz(Log::LOG log, In::IN in, Parm::PARM parm)                             //������� ����������� ����������
	{
		LEX lex;                                                                        //������ ��������� �������
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);                                 //������� ���������� ������������������ ��������� ������ � ��� ������������� ������ ������� ��������� ������� ������
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);                                   //������� ���������� ������������������ ��������� ��������������� � ��� ������������� ������ ������� ��������� ������� ��������������

		OUT out = getout(parm.out);                                                     //������� ���������� ������������������ ��������� OUT � ��� ������������� ������ ������� ���� ���������
		out_in_file(in.text, in.size, out);                                             //������� ������ � ���� � ���������������� ��������� ������
		Close(out);                                                                     //��������� �����

		OUT out_2 = getout_2(parm.out);                                                 //������� ���������� ������������������ ��������� OUT � ��� ������������� ������ ������� ���� ���������
		out_of_file(out_2);                                                             //������� ������ �� ����� ��� � ��� ����������������� ����� � ����� ������� ������ ������
		Close_out(out_2);                                                               //��������� �����

		int indxLex = 0;		// ������ �������
		int indexID = 0;		// ������ �����.
		int clit = 1;			// ������� ���������
		int line = 1;			// ����� ������
		int position = 0;	    // ����� ������� � �������� �����
		int sn = 0;

		bool findMain = false;
		bool findReturn = false;
		bool findLibryary = false;
		int countMain = 0;
		int braces[2] = { 0, 0 };


		char emptystr[] = "";	// ������ ������
		char* prefix = new  char[10]{ "" };	// ������� �������
		char* bufprefix = new  char[10]{ "" };	// ����� ��� ��������
		char* ifprefix = new  char[3]{ "if" };
		char* mainprefix = new char[5]{ "main" };
		char* oldprefix = new  char[10]{ "" };	// ���������� �������
		char* L = new  char[2]{ "L" };
		char* bufL = new  char[TI_STR_MAXSIZE];
		char* nameLiteral = new  char[10]{ "" };
		char* charclit = new char[10]{ "" };		// ��� ��������� ������ ��������

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
			FST::FST fstTypeString(out_2.word[i], FST_STRING);                             //���� ������
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
				int idx = IT::IsId(idtable, out_2.word[i]);	                              // ���� ����� �� �������������
				if (idx != TI_NULLIDX)		                                              // ���� ����� ������������� ��� ����
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
            #pragma endregion    //���������

            #pragma region FST_TIME
			FST::FST fstTime(out_2.word[i], FST_TIME);
			if (FST::execute(fstTime))
			{
				findCall = false;
				findLibryary = true;
				int idx = IT::IsId(idtable, out_2.word[i]);	                              // ���� ����� �� �������������
				if (idx != TI_NULLIDX)		                                              // ���� ����� ������������� ��� ����
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
            #pragma endregion    //���������

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
				entryIT.idxfirstLE = indxLex;                                                          //����� ������� � ������
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
            #pragma endregion    //���������

            #pragma region FST_ID
			FST::FST fstIdentif(out_2.word[i], FST_ID);                                      //�������� ������� �� ��������     
			if (FST::execute(fstIdentif))                                                    //������ 
			{
				int counter_size = 0;
				for (int counter_siz = 0; out_2.word[i][counter_siz] != '\0'; counter_siz++)
					counter_size++;
				if (counter_size > 10)
					out_2.word[i][10] = '\0';
				if (findFunc)	                                                             // ���� ��� ������ ���� �������
				{
					int idx = IT::IsId(idtable, out_2.word[i]);	                             // ���� ����� �� �������������
					if (idx != TI_NULLIDX)		                                             // ���� ����� ������������� ��� ����
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);          //�������� ������� ��������� entryLT � ��� ����������
						entryLT.position = position;
						LT::Add(lextable, entryLT);                                          //��������� � ������� ������
						findFunc = false;                                                    //�������� ������ ���������� �������
						position += strlen(out_2.word[i]);
						continue;
					}
				}


				else                                                                          //���� ���� ������� �� ��� ������ 
				{
					int idx = IT::IsId(idtable, out_2.word[i]);	                              // ���� ����� �� �������������
					if (idx != TI_NULLIDX)		                                              // ���� ����� ������������� ��� ����
					{


						if (findDeclare == true) throw ERROR_THROW_IN(125, line, position);
						findDeclare = false;


						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);           //�������� ������� ��������� entryLT � ��� ����������
						entryLT.position = position;
						LT::Add(lextable, entryLT);                                           //��������� � ������� ������ 
						findFunc = false;                                                     //�������� ������ ���������� �������
						position += strlen(out_2.word[i]);
						continue;                                                             //����������
					}
					if (findIf && findDeclare)
					{
						out_2.word[i] = strcat(ifprefix, out_2.word[i]);
					}
					else
					{
						strcpy(bufprefix, prefix);                                                //���� �������������� ��� �� �������� ������� � �����
						out_2.word[i] = strcat(bufprefix, out_2.word[i]);                         //������ ���� ������� ��� ����������� ��������(��������) � ��������������� ��� ����������
					}
					idx = IT::IsId(idtable, out_2.word[i]);                                   //���� � ���������
					if (idx != TI_NULLIDX)		                                              // ���� ����� ������������� ��� ����
					{
						if (findDeclare == true) throw ERROR_THROW_IN(125, line, position);
						findDeclare = false;
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);           //��������� ��� � ������� ������
						entryLT.position = position;
						LT::Add(lextable, entryLT);
						position += strlen(out_2.word[i]);
						continue;
					}
				}

				//� ��� ������ ���� ������� � �� �� ����� ������ �� ��������������
				if(findDeclare == false && findFunc == false && findParm == false) throw ERROR_THROW_IN(126, line, position);
				findDeclare = false;
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);             //��������� � ������� ������
				LT::Add(lextable, entryLT);
				if (findParm) {		                                                          // ���� ��������
					entryIT.idtype = IT::P;                                                   //��������� ��� �������������� - ��������
					strcpy(entryIT.visibility, prefix);                                       //�������� �������(��������) � ���� ���������(������ ��� �������)
					if (entryIT.iddatatype == IT::INT)                                        //���� ��� ���������� int 
						entryIT.value.vint = TI_INT_DEFAULT;                                  //��������� �������� �� ��������� ��� �������� ����������
					if (entryIT.iddatatype == IT::UNS)                                        //���� ��� ���������� int 
						entryIT.value.vuint = TI_UNS_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else if (!findFunc) {	                                                      // ���� ����������
					entryIT.idtype = IT::V;                                                   //��������� ��� �������������� - ����������
					strcpy(entryIT.visibility, prefix);                                       //�������� �������
					if (entryIT.iddatatype == IT::INT)                                        //���� ��� ���������� int 
						entryIT.value.vint = TI_INT_DEFAULT;                                  //��������� �������� �� ��������� ��� �������� ����������
					if (entryIT.iddatatype == IT::UNS)                                        //���� ��� ���������� int 
						entryIT.value.vuint = TI_UNS_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else {		                                                                  // ���� �������
					strcpy(oldprefix, prefix);                                                //�������� �������
					strcpy(prefix, out_2.word[i]);                                            //� ������� �������� �������� �������
					strcpy(entryIT.visibility, emptystr);                                     //�������� ������ ������
				}

				entryIT.idxfirstLE = indxLex;                                                  //������ ������ ������ � ������� ������ 
				strcpy(entryIT.id, out_2.word[i]);                                             //�������� �������� ������� � ���� id 
				IT::Add(idtable, entryIT);                                                     //��������� ������ ��������� ��������������� � ������� 
				findFunc = false;                                                              //�������� ���� �������
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_INTLIT
			FST::FST fstLiteralInt(out_2.word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt))
			{
				int value = atoi((char*)out_2.word[i]);	// �������� ��������
		
				
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.value.vuint = value;
				entryIT.idxfirstLE = indxLex;
				// ������������ ����� ��������
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// ����������� �������� �������� � ������(charclit)
				strcpy(bufL, L);	// �������� � ����� "L"
				out_2.word[i] = strcat(bufL, (char*)charclit);	// ��������� ��� ��� ��������
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
				// ������ ��������
				int length = strlen(out_2.word[i]);
				for (int k = 0; k < length; k++)	// ���������� �������, ������� �������
					out_2.word[i][k] = out_2.word[i][k + 1];
				out_2.word[i][length - 2] = 0;
				
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				entryLT.position = position;
				LT::Add(lextable, entryLT);
				strcpy(entryIT.value.vstr.str, out_2.word[i]);	// ������ �������� ���������� ��������
				entryIT.value.vstr.len = length - 2;		// ������ ����� ���������� ��������
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = indxLex;
				// ������������ ����� ��������
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// ����������� �������� �������� � ������(charclit)
				strcpy(bufL, L);	// �������� � ����� "L"
				nameLiteral = strcat(bufL, (char*)charclit);	// ��������� ��� ��� �������� (L + charclit)
				strcpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_OPERATOR
			FST::FST fstOperator(out_2.word[i], FST_OPERATOR);                                         //���� ������� �������� 
			if (FST::execute(fstOperator))                                                             //��������
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
				entryLT.position = position;
				switch (out_2.word[i][0])                                                              //�������� ����� �����
				{
				case PLUS: 
					entryLT.priority = 2;
					entryIT.value.operation = '+';
					break;
				case MINUS:                                                                 //���� ��� ����� ��������� 2
					entryLT.priority = 2;
					entryIT.value.operation = '-';
					break;
				case DIRSLASH: 
					entryLT.priority = 3;
					entryIT.value.operation = '/';
					break;
				case STAR:                                                              //������ ��� �������� ��������� ���� - 3
					entryLT.priority = 3;
					entryIT.value.operation = '*';
					break;
				}
				LT::Add(lextable, entryLT);                                                            //��������� � ������� ������
				strcpy(entryIT.id, out_2.word[i]);                                                     //�������� � ���� ��������
				entryIT.idxfirstLE = indxLex;                                                          //����� ������� � ������
				entryIT.idtype = IT::OP;                                                               //��� ��������������
				IT::Add(idtable, entryIT);                                                             //��������� ��� � ������� ���������������
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_SAME
			FST::FST fstSame(out_2.word[i], FST_SAME);                                       // ���� ==
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
			FST::FST fstNotSame(out_2.word[i], FST_NOTSAME);                                       // ���� !=
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
			FST::FST fstLess(out_2.word[i], FST_LESS);                                       // ���� <
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
			FST::FST fstMore(out_2.word[i], FST_MORE);                                       // ���� >
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
			FST::FST fstSameMore(out_2.word[i], FST_SAMEMORE);                                       // ���� >=
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
			FST::FST fstSameLess(out_2.word[i], FST_SAMELESS);                                       // ���� <=
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
			FST::FST fstSemicolon(out_2.word[i], FST_SEMICOLON);                                       // ���� ;
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
			FST::FST fstComma(out_2.word[i], FST_COMMA);                                               //���� �������
			if (FST::execute(fstComma))                                                                //��������
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);               //�������� � �����������������
				entryLT.priority = 1;                                                                  //��������� 1
				entryLT.position = position;
				LT::Add(lextable, entryLT);                                                            //���������� � ������� ������
				position += strlen(out_2.word[i]);
				continue;
			}
            #pragma endregion

            #pragma region FST_LEFTBRACE
			FST::FST fstLeftBrace(out_2.word[i], FST_LEFTBRACE);                                       //���� {
			if (FST::execute(fstLeftBrace))                                                            //��������
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);           //�������� � ������������� ������� ���������
				entryLT.position = position;
				LT::Add(lextable, entryLT);        
				position += strlen(out_2.word[i]);//���������� ����� ������� � ������� ������
				continue;
			}
            #pragma endregion

            #pragma region FST_BRACELET
			FST::FST fstRightBrace(out_2.word[i], FST_BRACELET);                                       //���� }
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
			FST::FST fstLeftThesis(out_2.word[i], FST_LEFTTHESIS);                                   // ���� (
			if (FST::execute(fstLeftThesis))                                                         // ��������
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);        //�������� � ������������� � ������� ������� 
				entryLT.position = position;
				entryLT.priority = 0;                                                                //���������
				LT::Add(lextable, entryLT);                                                          //��������� ��������� � ������� ������
				if (indexID != 0)
				if (idtable.table[indexID - 1].idtype == IT::F || idtable.table[indexID].idtype == IT::F) //���� � ������� ��������������� ����� ������� ��������� ������� �� 
					findParm = true;    
				position += strlen(out_2.word[i]);//�� ����� ���������
				continue;
			}
            #pragma endregion

            #pragma region FST_RIGHTTHESIS
			FST::FST fstRightThesis(out_2.word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);        //���� )
				entryLT.position = position;
				entryLT.priority = 0;                                                                 //��������� 0
				if (findParm && out_2.word[i + 1][0] != LEX_LEFTBRACE && out_2.word[i + 2][0] != LEX_LEFTBRACE && out_2.word[i-1][0] != LEX_LEFTTHESIS && !findLibryary)		// ���� ����� ������� ��� {
					strcpy(prefix, oldprefix);		// ���������� ���������� ���. ���������
				findParm = false;                   //��������� �� ��� ������������
				LT::Add(lextable, entryLT);         //��������� ) � ������� ������
				position += strlen(out_2.word[i]);
				findLibryary = false;
				continue;
			}
            #pragma endregion

            #pragma region FST_EQUAL
			FST::FST fstEqual(out_2.word[i], FST_EQUAL);                                              //���� =
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