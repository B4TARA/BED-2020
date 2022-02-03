#include "SemAn.h"

bool SemanticAnalyze(LT::LexTable& lexTable, IT::IdTable& idTable)
{
	bool Standart_LIB = false;
	int isMain = 0;
	bool isFunction = false;
	bool choiÒe = true;
	int isBrace = 0;
	bool isReturn = false;
	bool expression_Num = true;
	bool isIf = false;

	for (int i = 0, j; i < lexTable.size; i++)
	{
        #pragma region Õ≈¬≈–ÕŒ≈  ŒÀ»◊≈—“¬Œ œ¿–¿Ã≈“–Œ¬ ¬€«€¬¿≈ÃŒ… ‘”Õ ÷»» »À» Õ≈¬≈–Õ€≈ œ¿–¿Ã≈“–€ (DATA, TIME)
		if (LT::GetEntry(lexTable, i).lexema == '@')
		{

			if ((strcmp(IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id, "date") == 0) || (strcmp(IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id, "time") == 0))
			{
				int parm_q = 0;
				int b = i - 1;
				int parm_lexema = LT::GetEntry(lexTable, i + 1).lexema & 0x0f;
				while ((lexTable.table[b].lexema == LEX_ID || lexTable.table[b].lexema == LEX_LITERAL) && parm_q < parm_lexema) //·ÂÂÏ Ô‡‡ÏÂÚ˚ 
				{
					parm_q++;
					b--;
				}
				if (parm_q != 0)
					throw ERROR_THROW_IN(137, lexTable.table[i].sn, lexTable.table[i].position);
				continue;
			}
		}
        #pragma endregion //‡·ÓÚ‡ÂÚ

		switch (lexTable.table[i].lexema)
		{
		case LEX_INTEGER:
		{
            #pragma region »Ãﬂ »ƒ≈Õ“»‘» ¿“Œ–¿ Õ≈ ÃŒ∆≈“ ¡€“‹  Àﬁ◊≈¬€Ã —ÀŒ¬ŒÃ
			if (lexTable.table[i + 1].lexema != LEX_ID && lexTable.table[i + 1].lexema != LEX_FUNCTION && lexTable.table[i + 1].lexema != LEX_RIGHTTHESIS)
				throw ERROR_THROW_IN(132, lexTable.table[i].sn, lexTable.table[i].position);
			break;
            #pragma endregion // ‡·ÓÚ‡ÂÚ
		}

		case LEX_FUNCTION:
		{
            #pragma region “»œ ‘”Õ ÷»» » “»œ ¬Œ«¬–¿Ÿ¿≈ÃŒ√Œ «Õ¿◊≈Õ»ﬂ Œ“À»◊¿ﬁ“—ﬂ
			int p = i;
			bool isBooleanR = false;
			bool isIntR = false;
			if (lexTable.table[i - 1].lexema == LEX_INTEGER && lexTable.table[i + 1].lexema == LEX_ID && lexTable.table[i + 2].lexema == LEX_LEFTTHESIS)
			{
				isFunction = true;
				isReturn = true;
			}
			while (lexTable.table[p].lexema != LEX_RETURN)
			{
				p++;
			}
			for (int iter = p + 1; lexTable.table[iter].lexema != LEX_SEMICOLON; iter++)
			{
				if (lexTable.table[iter].idxTI != -1)
					if ((idTable.table[lexTable.table[iter].idxTI].idtype == IT::IDTYPE::V ||
						idTable.table[lexTable.table[iter].idxTI].idtype == IT::IDTYPE::L ||
						idTable.table[lexTable.table[iter].idxTI].idtype == IT::IDTYPE::VF) &&
						(idTable.table[lexTable.table[iter].idxTI].iddatatype != idTable.table[lexTable.table[i + 1].idxTI].iddatatype))
					{
						throw ERROR_THROW_IN(139, lexTable.table[i].sn, lexTable.table[i].position);
					}
			}
			break;
            #pragma endregion  // ‡·ÓÚ‡ÂÚ 
		}

		case LEX_RETURN:
		{
            #pragma region ‘”Õ ÷»ﬂ Õ»◊≈√Œ Õ≈ ¬Œ«¬–¿Ÿ¿≈“ (MAIN)
			if (isFunction == true)
			{
				isReturn = false;
				isFunction = false;
				break;
			}
			break;
            #pragma endregion
		}

		case LEX_ID:
		{
            #pragma region œ–Œ¬≈– ¿ ƒ»¿œ¿«ŒÕ¿ «Õ¿◊≈Õ»…
			if (LT::GetEntry(lexTable, i).idxTI != -1)
			{

				switch (IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).iddatatype)
				{
				case IT::INT:
				{
					if (IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).value.vint > pow(2, 31) - 1 || IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).value.vint < -pow(2, 31))
					{
						throw  ERROR_THROW_IN(146, lexTable.table[i].sn, lexTable.table[i].position);
					}
					break;
				}
				case IT::UNS:
				{
					if (IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).value.vuint > pow(2, 32) - 1 || IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).value.vint < 0)
					{
						throw  ERROR_THROW_IN(147, lexTable.table[i].sn, lexTable.table[i].position);
					}
					break;
				}
				}
			}
            #pragma endregion  //ÌÂ ‡·ÓÚ‡ÂÚ 

            #pragma region Õ≈À‹«ﬂ œ–»—¬¿»¬¿“‹ «Õ¿◊≈Õ»≈ ‘”Õ ÷»»
			if (IT::IsId(idTable, IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id) != -1 && idTable.table[IT::IsId(idTable, IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id)].idtype == IT::IDTYPE::F && lexTable.table[i + 1].lexema == LEX_EQUAL)
			{
				throw ERROR_THROW_IN(136, lexTable.table[i].sn, lexTable.table[i].position);
				choiÒe = false;
			}
            #pragma endregion //ÌÂ ‡·ÓÚ‡ÂÚ

            #pragma region Õ≈—ŒŒ“¬≈“—“¬»≈ œ–»—¬¿≈ÃŒ√Œ “»œ¿ “»œ” œ≈–≈Ã≈ÕÕŒ…

			if (LT::GetEntry(lexTable, i + 1).lexema == '=')
			{
				int j = i;
				bool func_check = true;
				while (LT::GetEntry(lexTable, j).lexema != LEX_SEMICOLON)
				{
					j++;
					func_check = true;

					if (LT::GetEntry(lexTable, j).idxTI != -1)
					{
						if (idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::F ||
							idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::V ||
							idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::P ||
							idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::L)
						{
							if (idTable.table[LT::GetEntry(lexTable, j).idxTI].iddatatype != idTable.table[LT::GetEntry(lexTable, i).idxTI].iddatatype)
							{
								if ((idTable.table[LT::GetEntry(lexTable, j).idxTI].iddatatype != IT::UNS &&
									idTable.table[LT::GetEntry(lexTable, j).idxTI].value.vint >= 0) &&
									idTable.table[LT::GetEntry(lexTable, i).idxTI].iddatatype == IT::UNS)
								{
									idTable.table[LT::GetEntry(lexTable, j).idxTI].iddatatype = IT::UNS;
									idTable.table[LT::GetEntry(lexTable, j).idxTI].value.vuint = idTable.table[LT::GetEntry(lexTable, j).idxTI].value.vint;
									break;
								}
								int l = j;
								while (LT::GetEntry(lexTable, l).lexema != LEX_SEMICOLON)
								{
									if (LT::GetEntry(lexTable, l).lexema == '@')
									{
										func_check = false;
										break;
									}
									l++;
								}
								if (!func_check)continue;
								throw ERROR_THROW_IN(140, lexTable.table[i].sn, lexTable.table[i].position);
								choiÒe = false;
								break;
							}
						}
					}
				}
				IT::IDDATATYPE idtu = IT::IDDATATYPE::DEF;
				IT::IDDATATYPE idti = IT::IDDATATYPE::DEF;
				if (lexTable.table[i+3].idxTI != -1 && IT::GetEntry(idTable, LT::GetEntry(lexTable, i + 3).idxTI).value.operation == 'u')
					idtu = IT::IDDATATYPE::UNS;
				else if (lexTable.table[i + 3].idxTI != -1 && IT::GetEntry(idTable, LT::GetEntry(lexTable, i + 3).idxTI).value.operation == 'i')
					idti = IT::IDDATATYPE::INT;
				if (idtu == IT::IDDATATYPE::DEF && idti == IT::IDDATATYPE::DEF)break;
				if (lexTable.table[i].idxTI != -1 && IT::IsId(idTable, IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id) != -1 &&
					idTable.table[IT::IsId(idTable, IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id)].iddatatype ==
					idtu)
				{
					break;
				}
				if (lexTable.table[i].idxTI != -1 && IT::IsId(idTable, IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id) != -1 &&
					idTable.table[IT::IsId(idTable, IT::GetEntry(idTable, LT::GetEntry(lexTable, i).idxTI).id)].iddatatype ==
					idti)
				{
					break;
				}
				throw ERROR_THROW_IN(134, lexTable.table[i].sn, lexTable.table[i].position);
			}
			break;
            #pragma endregion //–¿¡Œ“¿≈“
		}

		case '@':
		{
            #pragma region Õ≈—ŒŒ“¬≈“—“¬»≈ »À» Õ≈¬≈–ÕŒ≈  ŒÀ-¬Œ œ¿–¿Ã≈“–Œ¬ ¬€«€¬¿≈ÃŒ… ‘”Õ ÷»»
			int int_p = 0;
			int str_p = 0;
			int uint_p = 0;
			int char_p = 0;
			int bool_p = 0;

			int int_p2 = 0;
			int str_p2 = 0;
			int uint_p2 = 0;
			int char_p2 = 0;
			int bool_p2 = 0;
			int a = 0;
			int l = 0;
			for (int iter = 0; iter < lexTable.size; iter++)
			{
				if (LT::GetEntry(lexTable, iter).idxTI != -1 && strcmp(idTable.table[LT::GetEntry(lexTable, i).idxTI].id, idTable.table[LT::GetEntry(lexTable, iter).idxTI].id) == 0)
				{
					a = iter;
					l = a;
					a++;
					break;
				}
			}
			while (lexTable.table[a].lexema != LEX_RIGHTTHESIS)
			{
				if (LT::GetEntry(lexTable, a).idxTI != -1)
				{
					if (idTable.table[LT::GetEntry(lexTable, a).idxTI].iddatatype == IT::IDDATATYPE::STR)
						str_p++;
					else if (idTable.table[LT::GetEntry(lexTable, a).idxTI].iddatatype == IT::IDDATATYPE::INT)
						int_p++;
					else if (idTable.table[LT::GetEntry(lexTable, a).idxTI].iddatatype == IT::IDDATATYPE::UNS)
						uint_p++;
				}
				a++;
			}
			int parm_lexema = 0;
			int i_buf = i;
			while ((lexTable.table[i - 1].lexema == LEX_ID || lexTable.table[i - 1].lexema == LEX_LITERAL) && parm_lexema < (lexTable.table[i + 1].lexema - '0'))
			{
				if (idTable.table[LT::GetEntry(lexTable, i).idxTI].iddatatype == IT::IDDATATYPE::STR)
					str_p2++;
				else if (idTable.table[LT::GetEntry(lexTable, i).idxTI].iddatatype == IT::IDDATATYPE::INT)
					int_p2++;
				else if (idTable.table[LT::GetEntry(lexTable, i).idxTI].iddatatype == IT::IDDATATYPE::UNS)
					uint_p2++;
				i--;
				parm_lexema++;
			}
			if (int_p != int_p2 || str_p != str_p2 || uint_p != uint_p2)
				throw ERROR_THROW_IN(138, lexTable.table[i].sn, lexTable.table[i].position);
			i = i_buf;
			break;
            #pragma endregion  //–¿¡Œ“¿≈“
		}

		case LEX_LESS:
		case LEX_MORE:
		case LEX_SAME:
		case LEX_NOTSAME:
		{
            #pragma region Õ¿–”ÿ≈Õ»≈ ‘Œ–Ã€ ÀŒ√»◊≈— Œ√Œ ¬€–¿∆≈Õ»ﬂ
			for (int iter = i + 1; lexTable.table[iter].lexema != LEX_SEMICOLON; iter++)
			{
				if (lexTable.table[iter].lexema == LEX_EQUAL ||
					lexTable.table[iter].lexema == LEX_MORE ||
					lexTable.table[iter].lexema == LEX_LESS ||
					lexTable.table[iter].lexema == LEX_SAME)
				{
					throw ERROR_THROW_IN(149, lexTable.table[i].sn, lexTable.table[i].position);
					break;
				}
			}
			for (int iter = i - 1; lexTable.table[iter].lexema != LEX_SEMICOLON; iter--)
			{
				if (lexTable.table[iter].lexema == LEX_EQUAL ||
					lexTable.table[iter].lexema == LEX_MORE ||
					lexTable.table[iter].lexema == LEX_LESS ||
					lexTable.table[iter].lexema == LEX_SAME)
				{
					throw ERROR_THROW_IN(149, lexTable.table[i].sn, lexTable.table[i].position);
					break;
				}
			}
            #pragma endregion

            #pragma region Õ≈—ŒŒ“¬≈“—“¬»≈ œ–»—¬¿≈ÃŒ√Œ “»œ¿ “»œ” œ≈–≈Ã≈ÕÕŒ…
			i++;
			int j = i;
			bool func_check = true;
			while (LT::GetEntry(lexTable, j).lexema != LEX_SEMICOLON && LT::GetEntry(lexTable, j).lexema != LEX_RIGHTTHESIS)
			{
				j++;
				func_check = true;

				if (LT::GetEntry(lexTable, j).idxTI != -1)
				{
					if (idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::F ||
						idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::V ||
						idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::P ||
						idTable.table[LT::GetEntry(lexTable, j).idxTI].idtype == IT::IDTYPE::L)
					{
						if (idTable.table[LT::GetEntry(lexTable, j).idxTI].iddatatype != idTable.table[LT::GetEntry(lexTable, i).idxTI].iddatatype)
						{
							if ((idTable.table[LT::GetEntry(lexTable, j).idxTI].iddatatype != IT::UNS &&
								idTable.table[LT::GetEntry(lexTable, j).idxTI].value.vint >= 0) &&
								idTable.table[LT::GetEntry(lexTable, i).idxTI].iddatatype == IT::UNS)
							{
								idTable.table[LT::GetEntry(lexTable, j).idxTI].iddatatype = IT::UNS;
								idTable.table[LT::GetEntry(lexTable, j).idxTI].value.vuint = idTable.table[LT::GetEntry(lexTable, j).idxTI].value.vint;
								break;
							}
							int l = j;
							while (LT::GetEntry(lexTable, l).lexema != LEX_SEMICOLON)
							{
								if (LT::GetEntry(lexTable, l).lexema == '@')
								{
									func_check = false;
									break;
								}
								l++;
							}
							if (!func_check)continue;
							throw ERROR_THROW_IN(134, lexTable.table[i].sn, lexTable.table[i].position);
							choiÒe = false;
							break;
						}
					}
				}
			}
			break;
            #pragma endregion
		}

		case LEX_LEFTBRACE:
		{
            #pragma region Õ¿–”ÿ≈Õ¿ —“–” “”–¿ œ–Œ√–¿ÃÃÕŒ√Œ ¡ÀŒ ¿
			isBrace++;
			if ((isBrace == 2 && !isIf) || (isBrace == 4 && isIf))
				throw ERROR_THROW(142, -1, -1);
			break;
            #pragma endregion
		}

		case LEX_IF:
		{
			isIf = true;
			break;
		}
		case LEX_BRACELET:
		{
			if (isIf) isIf = false;
			isBrace--;
			break;
		}
		case LEX_LITERAL:
		{
			if (isIf) isIf = false;
			isBrace--;
			break;
		}
		}

	}

    #pragma region ‘”Õ ÷»ﬂ Õ≈ ¬Œ«¬–¿Ÿ¿≈“ «Õ¿◊≈Õ»≈
	if (isReturn == true)
	{
		throw ERROR_THROW(141);
		choiÒe = false;
	}
    #pragma endregion

	return choiÒe;
}