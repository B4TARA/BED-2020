#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <vector>
#include <string>
#include "CodeGeneration.h"
#include "In.h"
#include "PolishNotation.h"
#include "Log.h"
#include <string> 
#include <sstream>
#include <fstream>
#pragma once

using namespace std;
namespace CG
{
	void add(IT::IdTable& idTable, ConstSegment& cnst)
	{
		string str;
		for (int i = 0; i < idTable.size; i++)
		{
			if (idTable.table[i].idtype == IT::L && idTable.table[i].iddatatype == IT::STR)
			{
				str += "\n\t";
				str += idTable.table[i].visibility;
				str += idTable.table[i].id; str += " BYTE ";
				str += "\"";
				str += idTable.table[i].value.vstr.str;
				str += "\"";
				str += ", 0";
				strcpy(cnst.Data[cnst.size++], str.c_str());
				str.clear();
			}
			else if (idTable.table[i].idtype == IT::L && idTable.table[i].iddatatype == IT::INT)
			{
				str += "\n\t";
				str += idTable.table[i].visibility;
				str += idTable.table[i].id; str += " SDWORD ";

				str += to_string(idTable.table[i].value.vint);
				strcpy(cnst.Data[cnst.size++], str.c_str());
				str.clear();
			}
			else if (idTable.table[i].idtype == IT::L && idTable.table[i].iddatatype == IT::UNS)
			{
				str += "\n\t";
				str += idTable.table[i].visibility;
				str += idTable.table[i].id;
				str += " DWORD ";
				str += to_string(idTable.table[i].value.vuint);
				strcpy(cnst.Data[cnst.size++], str.c_str());
				str.clear();
			}

		}
	}
	void add(IT::IdTable& idTable, DataSegment& dataseg)
	{
		for (int i = 0; i < idTable.size; i++)
		{
			if (idTable.table[i].idtype == IT::V || idTable.table[i].idtype == IT::VF)
			{
				switch (idTable.table[i].iddatatype)
				{
				case IT::INT:
					strcpy_s(dataseg.Data[dataseg.size], TI_STR_MAXSIZE, "\n\t");

					strcat(dataseg.Data[dataseg.size], idTable.table[i].visibility);
					strcat(dataseg.Data[dataseg.size], idTable.table[i].id);
					strcat(dataseg.Data[dataseg.size++], " SDWORD 0");
					break;
				case IT::STR:
					strcpy_s(dataseg.Data[dataseg.size], TI_STR_MAXSIZE, "\n\t");

					strcat(dataseg.Data[dataseg.size], idTable.table[i].visibility);
					strcat(dataseg.Data[dataseg.size], idTable.table[i].id);
					strcat(dataseg.Data[dataseg.size++], " DWORD ?");
					break;
				case IT::UNS:
					strcpy_s(dataseg.Data[dataseg.size], TI_STR_MAXSIZE, "\n\t");

					strcat(dataseg.Data[dataseg.size], idTable.table[i].visibility);
					strcat(dataseg.Data[dataseg.size], idTable.table[i].id);
					strcat(dataseg.Data[dataseg.size++], " DWORD 0");
					break;
				}
			}

		}
	}
	void CodeSegment::add(string str)
	{
		strcpy_s(Data[size++], TI_STR_MAXSIZE, str.c_str());
	}

	void	CodeSegmentGeneration(CodeSegment& CS, LT::LexTable lexTable, IT::IdTable& idTable)
	{
		vector<LT::Entry> ñondition_part1;
		vector<vector<LT::Entry>> ñondition_part1_vectors;
		vector<LT::Entry> ñondition_part2;
		vector<vector<LT::Entry>> ñondition_part2_vectors;
		string metka[255];
		vector<LT::Entry> compare_operator;
		vector<LT::Entry> counter;
		vector<LT::Entry> counter_change;
		vector<vector<LT::Entry>> counter_change_vectors;
		bool ifmain = false;
		int if_if = 0;
		In::IN myinf;
		string str;
		vector<LT::Entry> str_for;
		vector<LT::Entry> tempEntries;
		string func_name;
		int idxTI;
		for (int i = 0; i < lexTable.size; i++)
		{
			str.clear();
			while (myinf.code[lexTable.table[i].lexema] != myinf.D &&
				lexTable.table[i].lexema != '\0' &&
				lexTable.table[i].lexema != '=' &&
				lexTable.table[i].lexema != LEX_MORE &&
				lexTable.table[i].lexema != LEX_LESS &&
				lexTable.table[i].lexema != LEX_SAME &&
				lexTable.table[i].lexema != LEX_NOTSAME)
			{
				str += lexTable.table[i++].lexema;
				if (lexTable.table[i - 1].lexema == 'i' || lexTable.table[i - 1].lexema == 'l')
				{
					idxTI = lexTable.table[i - 1].idxTI;
				}
				else if (lexTable.table[i - 1].lexema == 'o' || lexTable.table[i - 1].lexema == 'r')
					break;
			}

			if (str == "c@0#")
			{
				string other_name;
				if (strcmp(idTable.table[lexTable.table[i - 3].idxTI].id, "date") == 0)
					other_name = "Date";
				if (strcmp(idTable.table[lexTable.table[i - 3].idxTI].id, "time") == 0)
					other_name = "Time";
				CS.add(generateInstructions(CALL, lexTable.table[i - 3].idxTI, idTable, idTable.table[lexTable.table[i - 3].idxTI].iddatatype, other_name));
				continue;
			}

			if (str == "dtfi")
			{
				func_name = idTable.table[idxTI].id;
				func_name += "_proc";
				CS.add(generateInstructions(FUNC, idxTI, idTable, idTable.table[idxTI].iddatatype, func_name));
				continue;
			}

			if (str == "ti" && idTable.table[idxTI].idtype == IT::P)
			{
				CS.add(generateInstructions(PARM, idxTI, idTable, idTable.table[idxTI].iddatatype));
				continue;
			}

			if ((str == "dti" || str == "i") && lexTable.table[i].lexema == '=')
			{
				i++;
				while (lexTable.table[i].lexema != LEX_SEMICOLON)
				{
					tempEntries.push_back(lexTable.table[i]);
					i++;
				}
				string pop_result;
				if ((idTable.table[idxTI].iddatatype == IT::IDDATATYPE::STR) && tempEntries.size() == 1)
				{

					pop_result = "\n\t mov "; pop_result += idTable.table[idxTI].visibility; pop_result += idTable.table[idxTI].id; pop_result += ",offset ";
					pop_result += idTable.table[tempEntries[0].idxTI].visibility; pop_result += idTable.table[tempEntries[0].idxTI].id;
				}
				else if ((idTable.table[idxTI].iddatatype == IT::IDDATATYPE::STR) && tempEntries.size() != 1)
				{
					generateExpression(tempEntries, CS, idxTI, idTable, idTable.table[idxTI].iddatatype);
					pop_result = "\n\tpop "; pop_result += idTable.table[idxTI].visibility; pop_result += idTable.table[idxTI].id;
				}
				else
				{
					generateExpression(tempEntries, CS, idxTI, idTable, idTable.table[idxTI].iddatatype);
					pop_result = "\n\tpop ";
					pop_result += idTable.table[idxTI].visibility;
					pop_result += idTable.table[idxTI].id;
				}
				CS.add(pop_result);
				tempEntries.clear();
				continue;
			}

			if (str == "m")
			{
				ifmain = true;
				func_name = "main";
				CS.add(generateInstructions(FUNC, lexTable.table[i].idxTI, idTable, IT::INT, func_name));
				continue;
			}


			//////////////////////////////////////////////////////////////////////////////////////
			else if (str == "?")
			{
				i++;
				for (; lexTable.table[i].lexema != '{'; i++)
				{
					str_for.push_back(lexTable.table[i]);
				}
				int strnumber = str_for[0].sn;
				int iter = 0;
				iter++;
				while (str_for[iter].lexema != '<' && str_for[iter].lexema != '>' && str_for[iter].lexema != '~' && str_for[iter].lexema != '!' && str_for[iter].lexema != ']' && str_for[iter].lexema != '[')
				{
					ñondition_part1.push_back(str_for[iter++]);
				}
				ñondition_part1_vectors.push_back(ñondition_part1);
				compare_operator.push_back(str_for[iter]);
				iter++;
				while (str_for[iter].lexema != ';')
				{
					ñondition_part2.push_back(str_for[iter++]);
				}
				ñondition_part2_vectors.push_back(ñondition_part2);
				iter++;
				vector<vector<LT::Entry>> buf;
				buf.push_back(ñondition_part1_vectors[if_if]);
				generateExpression(buf[0], CS, -1, idTable, idTable.table[idxTI].iddatatype);
				CS.add("\n\tpop ebx");
				buf.clear();
				buf.push_back(ñondition_part2_vectors[if_if]);
				generateExpression(buf[0], CS, -1, idTable, idTable.table[idxTI].iddatatype);
				buf.clear();
				CS.add("\n\tpop eax");
				iter++;
				metka[if_if] = " metka1";
				metka[if_if] += std::to_string(strnumber);
				switch (compare_operator[if_if].lexema)
				{
				case '<':
				{
					CS.add("\n\t cmp ebx, eax");
					CS.add("\n\tjb " + metka[if_if]);
					break;
				}
				case '>':
				{
					CS.add("\n\t cmp ebx, eax");
					CS.add("\n\tja " + metka[if_if]);
					break;
				}
				case '~':
				{
					CS.add("\n\t cmp ebx, eax");
					CS.add("\n\tje " + metka[if_if]);
					break;
				}
				case '!':
				{
					CS.add("\n\t cmp ebx, eax");
					CS.add("\n\tjne " + metka[if_if]);
					break;
				}
				case ']':
				{
					CS.add("\n\t cmp ebx, eax");
					CS.add("\n\tjae " + metka[if_if]);
					break;
				}
				case '[':
				{
					CS.add("\n\t cmp ebx, eax");
					CS.add("\n\tjbe " + metka[if_if]);
					break;
				}
				}
				if_if++;
				metka[if_if] = " metka2";
				metka[if_if] += std::to_string(strnumber);
				CS.add("\n\tjmp ");
				CS.add(metka[if_if]);
				CS.add("\n\t");
				CS.add(metka[if_if - 1]);
				CS.add(":");

				str_for.clear();

				continue;
			}
			////////////////////////////////////////////////////////////////////////////////////////////

			if (str == "o")
			{
				int l = i;
				while (lexTable.table[l].lexema != LEX_SEMICOLON)
				{
					tempEntries.push_back(lexTable.table[l]);
					l++;
				}
				generateExpression(tempEntries, CS, -1, idTable, idTable.table[lexTable.table[i].idxTI].iddatatype);
				CS.add(generateInstructions(PRNT, -1, idTable, idTable.table[lexTable.table[i].idxTI].iddatatype, func_name));
				continue;
			}

			if (lexTable.table[i].lexema == '}' && if_if > 0)
			{
				CS.add("\n\t");
				CS.add(metka[if_if]);
				CS.add(":");
				if_if -= 2;
				if (if_if == 0)
				{
					ñondition_part1.clear();
					ñondition_part1_vectors.clear();
					ñondition_part2.clear();
					ñondition_part2_vectors.clear();
					compare_operator.clear();
					counter.clear();
					counter_change.clear();
					counter_change_vectors.clear();
				}
				continue;
			}
			if (lexTable.table[i].lexema == '}')
			{
				
				if (ifmain)
				{
					CS.add(generateInstructions(RET, 1, idTable));
					CS.add(generateInstructions(ENDP, NULL, idTable, idTable.table[idxTI].iddatatype, func_name));
					break;
				}
				CS.add(generateInstructions(ENDP, NULL, idTable, idTable.table[idxTI].iddatatype, func_name));
				continue;
			}
			if (str == "r")
			{
				int l = i;
				if (ifmain)
				{
					endOfFucntion(CS);
					continue;
				}
				while (lexTable.table[l].lexema != LEX_SEMICOLON)
				{
					tempEntries.push_back(lexTable.table[l]);
					l++;
				}
				generateExpression(tempEntries, CS, NULL, idTable, idTable.table[idxTI].iddatatype);
				endOfFucntion(CS);
				CS.add(generateInstructions(RET, NULL, idTable));
				continue;
			}
		}
	}



	void endOfFucntion(CodeSegment& CS)
	{
		CS.add("\n\n\tjmp EXIT\n\tEXIT_DIV_ON_NULL:\n\tpush offset null_division\n\tcall outputstr\n\tpush - 1\n\tcall ExitProcess");
		CS.add("\n\n\tEXIT_OVERFLOW:\n\tpush offset overflow\n\tcall outputstr\n\tpush - 2\n\tcall ExitProcess\n\n\tEXIT:");
	}



	void generateExpression(vector<LT::Entry>& tempEntries, CodeSegment& CS, int idxTI, IT::IdTable idTable, IT::IDDATATYPE type)
	{
		bool isboolop = 0;
		bool isconvert = false;
		string out;
		vector<LT::Entry> typeconvertion;
		int begin = 0; int end = 0; int count = 0;

		for (size_t i = 0; i < tempEntries.size(); i++)
		{
			switch (tempEntries[i].lexema)
			{

			case LEX_LITERAL:
			case LEX_ID:
			{
				CS.add(generateInstructions(PUSH, tempEntries[i].idxTI, idTable));	break;
			}
			case LEX_SEMICOLON: CS.add(generateInstructions(SEM, idxTI, idTable));	break;
			case '@':			CS.add(generateInstructions(CALL, tempEntries[i].idxTI, idTable));	break;
			case '<':
			{
				std::ostringstream oss;
				oss << tempEntries[0].sn;
				string number_str = oss.str();
				out = "\n\tpop ebx \n\tpop eax \n\t cmp eax,ebx";
				out += "\n\tjl istrue";
				out += number_str;
				out += "\n\tpush 0";
				out += "\n\tjmp exit"; out += number_str;
				out += "\n\tistrue";
				out += number_str;
				out += ":\n\tpush 1";
				out += "\n\texit"; out += number_str; out += ":";
				isboolop = true;
				break;
			}
			case '>':
			{
				std::ostringstream oss;
				oss << tempEntries[0].sn;
				string number_str = oss.str();
				out = "\n\tpop ebx \n\tpop eax \n\t cmp eax,ebx";
				out += "\n\tjg istrue";
				out += number_str;
				out += "\n\tpush 0";
				out += "\n\tjmp exit"; out += number_str;
				out += "\n\tistrue";
				out += number_str;
				out += ":\n\tpush 1";
				out += "\n\texit"; out += number_str; out += ":";
				isboolop = true;
				break;
			}
			case '~':
			{
				std::ostringstream oss;
				oss << tempEntries[0].sn;
				string number_str = oss.str();
				out = "\n\tpop ebx \n\tpop eax \n\t cmp eax,ebx";
				out += "\n\tje istrue";
				out += number_str;
				out += "\n\tpush 0";
				out += "\n\tjmp exit"; out += number_str;
				out += "\n\tistrue";
				out += number_str;
				out += ":\n\tpush 1 ";
				out += "\n\texit"; out += number_str; out += ":";
				isboolop = true;
				break;
			}
			case '!':
			{
				std::ostringstream oss;
				oss << tempEntries[0].sn;
				string number_str = oss.str();
				out = "\n\tpop ebx \n\tpop eax \n\t cmp eax,ebx";
				out += "\n\tjne istrue";
				out += number_str;
				out += "\n\tpush 0";
				out += "\n\tjmp exit"; out += number_str;
				out += "\n\tistrue";
				out += number_str;
				out += ":\n\tpush 1 ";
				out += "\n\texit"; out += number_str; out += ":";
				isboolop = true;
				break;
			}
			case '(':
			{
				isconvert = true;
				typeconvertion.push_back(tempEntries[i + 1]);
				i += 2;
				break;
			}
			}
			if (tempEntries[i].idxTI != -1 && idTable.table[tempEntries[i].idxTI].idtype == IT::IDTYPE::OP)
			{
				switch (idTable.table[tempEntries[i].idxTI].value.operation)
				{
				case PLUS:
				{		if (type == IT::INT || type == IT::UNS)
				{
					CS.add(generateInstructions(ADD, tempEntries[i].idxTI, idTable));
				}
				else
				{
					CS.add(generateInstructions(ADDSTR, tempEntries[i].idxTI, idTable));
				}
				break;
				}
				case STAR:	CS.add(generateInstructions(MUL, tempEntries[i].idxTI, idTable));	break;
				case MINUS:	CS.add(generateInstructions(DIFF, tempEntries[i].idxTI, idTable));	break;
				case DIRSLASH:	CS.add(generateInstructions(DIV, tempEntries[i].idxTI, idTable));	break;
				}
			}

		};

		if (isboolop)
		{
			CS.add(out);
		}
		out.clear();
		tempEntries.clear();
	}


	string	generateInstructions(INSTRUCTIONTYPE t, int idxTI, IT::IdTable idTable, IT::IDDATATYPE type, string fucn_name)
	{
		string str; static int ret = 0;
		switch (t)
		{
		case FUNC:
		{
			str += "\n\n"; str += fucn_name; str += " PROC";
			return str;
			break;
		}
		case PARM:
		{
			str += ", "; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id; str += " : ";
			switch (type)
			{
			case IT::STR: str += "DWORD"; ret += 4;  return str; break;
			case IT::INT: str += "SDWORD"; ret += 4;   return str; break;
			case IT::UNS: str += "DWORD"; ret += 4;   return str; break;
			}
			break;
		}
		case PUSH:
		{
			if ((idTable.table[idxTI].iddatatype == IT::INT || idTable.table[idxTI].iddatatype == IT::UNS) && idTable.table[idxTI].idtype != IT::F && idTable.table[idxTI].idtype != IT::OP)
			{
				str += "\n\tpush "; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id;
			}
			else if ((idTable.table[idxTI].iddatatype == IT::STR) && idTable.table[idxTI].idtype == IT::P)
			{
				str += "\n\tpush dword ptr "; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id;
			}
			else if ((idTable.table[idxTI].iddatatype == IT::STR) && idTable.table[idxTI].idtype == IT::L)
			{
				str += "\n\tpush offset "; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id;
			}
			else if ((idTable.table[idxTI].iddatatype == IT::STR) && (idTable.table[idxTI].idtype == IT::V || idTable.table[idxTI].idtype == IT::VF))
			{
				str += "\n\tpush dword ptr "; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id;
			}
			else if (idTable.table[idxTI].idtype != IT::F && idTable.table[idxTI].idtype != IT::OP)
			{
				str += "\n\tpush offset "; str += idTable.table[idxTI].visibility;	str += idTable.table[idxTI].id;
			}
			return str;
			break;
		}
		case RET:
		{	if (idxTI)
		{
			return "\n\tpush 0\n\tcall ExitProcess\n";
		}
		else
		{
			char str[30]; char buf[4];
			strcpy(str, "\n\tpop eax\n\tret ");
			_itoa(ret, buf, 10);
			strcat(str, buf);
			strcat(str, "\n");
			ret = 0;
			return str;  break;
		}
		}
		case ADD:		return "\n\tpop eax\n\tpop ebx\n\tadd eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case BLEFT:		return "\n\tpop ecx\n\tpop eax\n\tsal eax, cl\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case BRIGHT:	return "\n\tpop ecx\n\tpop eax\n\tsar  eax, cl\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case ADDSTR:	return "\n\tcall strcon\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case DIFF:		return "\n\tpop ebx\n\tpop eax\n\tsub eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case MUL:		return "\n\tpop eax\n\tpop ebx\n\timul eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case DIV:		return "\n\tpop ebx\n\tpop eax\n\ttest ebx,ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tidiv ebx\n\tpush eax"; break;
		case ENDP:
		{
			str += "\n"; str += fucn_name; str += " ENDP";
			switch ((strcmp(fucn_name.c_str(), "main") == 0) ? true : false)
			{
			case true: str += "\nend main"; return str;
			case false:						return str;
			}
			break;
		}
		case PRNT:
		{
			if (type == IT::STR)
				return "\n\tcall outputstr";
			else if (type == IT::INT)
				return "\n\tcall outputint";
			else if (type == IT::UNS)
				return "\n\tcall outputuint";
			break;
		}
		case SEM:
		{	if (idxTI != -1 && idTable.table[idxTI].iddatatype == IT::INT && idTable.table[idxTI].idtype != IT::F)
		{
			str += "\n\tpop "; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id;
		}
		else if (idxTI != -1 && idTable.table[idxTI].iddatatype == IT::STR)
		{
			str += "\n\tpush offset "; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id;
		}
		return str;
		break;
		}
		case CALL:
		{
			if (strcmp(idTable.table[idxTI].id, "time")==0 || strcmp(idTable.table[idxTI].id, "date")==0)
			{
				str += "\n\tcall "; str += fucn_name;
				//str += "_proc";
			}
			else
			{
				str += "\n\tcall "; str += idTable.table[idxTI].id; str += "_proc";
			}
			str += "\n\tpush eax";
			return str;
			break;
		}
		case MOV:
		{
			str += "\n\tmov ecx,"; str += idTable.table[idxTI].visibility; str += idTable.table[idxTI].id;
			return str;
			break;
		}
		}
	}
	void print(Log::LOG log, ConstSegment& cnst, DataSegment& dataseg, CodeSegment& codeseg)
	{
		for (int i = 0; i < cnst.size; i++)
			*log.stream << cnst.Data[i];
		for (int i = 0; i < dataseg.size; i++)
			*log.stream << dataseg.Data[i];
		for (int i = 0; i < codeseg.size; i++)
			*log.stream << codeseg.Data[i];
	}
	void DataOut(ofstream& _file, ConstSegment& cnst, DataSegment& dataseg, CodeSegment& codeseg)
	{
		for (int i = 0; i < cnst.size; i++)
			_file << cnst.Data[i];
		for (int i = 0; i < dataseg.size; i++)
			_file << dataseg.Data[i];
		for (int i = 0; i < codeseg.size; i++)
			_file << codeseg.Data[i];
	}
	void startGeneration(wchar_t* file, ConstSegment& CnstS, DataSegment& DS, CodeSegment& CS)
	{
		wchar_t buf[255];
		wcscpy_s(buf, file);
		ofstream _file; _file.open(buf, ios_base::out | ios_base::trunc);						//Çàïèñü  àññåìáëåðà â ôàéë
		if (_file.is_open())
		{
			remove((char*)buf);
		}
		DataOut(_file, CnstS, DS, CS);
		_file.close();
	}
};
