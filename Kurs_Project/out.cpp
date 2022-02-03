#include "out.h"


using namespace std;

void out_in_file(unsigned char* source, int size, OUT& out)
{

	for (int i = 0; i <= size; i++)
	{
		int if_probel = 0;

		if (*(source + i) == ' ' && (*(source + i - 1) == ' ')) continue;               //если два пробела подряд то пропускаем и не добавляем пробел

		if (*(source + i) == ' ')                                                       //если символ пробел
		{
			for (unsigned int j = 0; j <= strlen(probel); j++)                          //проверка на символ после пробела и до пробела
			{
				if (*(source + i + 1) == probel[j] || *(source + i - 1) == probel[j] || *(source + i - 1) == '\n')   //если символ после пробела или до один из массива сепараторов
				{
					if_probel = 1;                                                      //поднимаем флаг
					break;                                                              //выходим из цикла
				}
			}
			if (if_probel == 1) continue;                                               //если флаг поднят то продолжаем не записывая пробел
		}

		if (*(source + i) == '\n' || i == size)                                          //если символ переход строки или конец текста
		{
			if (*(source + i - 1) != ';' && *(source + i + 1) != '{' && *(source + i - 1) != '{')   //если предыдущий символ не равен точке с запятой и следующий не равен скобке и предыдущий не равен скобке
			{
				continue;                                                                           //не добавляем разделитель
			}
			*out.stream << '|';                                                                     //в ином случае добавляем
			*out.stream << '\n';                                                                    //переходим на новую строку
			continue;                                                                               //продолжаем
		}
		*out.stream << *(source + i);                                                               //если ничего не выполняется, то просто передаем символ
	}
}

OUT getout(wchar_t outfile[])                                                                       //функция открытия файла для записи
{
	OUT out;
	out.stream = new ofstream;
	out.stream->open(outfile);
	if (out.stream->fail())
		throw ERROR_THROW(112);
	wcscpy_s(out.outfile, outfile);
	return out;
}

OUT getout_2(wchar_t outfile[])                                                                      //функция открытия файла для чтения
{
	OUT out;
	out.stream_of_file = new ifstream;
	out.stream_of_file->open(outfile);
	if (out.stream_of_file->fail())
		throw ERROR_THROW(112);
	wcscpy_s(out.outfile, outfile);
	return out;
}

void out_of_file(OUT out)                                                                            //функция чтения из файла и создания массива лексем
{

	//тут мы инициализируем двумерный массив для лексем
	for (int i = 0; i < max_word; i++)                  //каждый элемент двумерного динамического массива инициализируется как массив чаров
		out.word[i] = new char[size_word] {NULL};
	//

	//тут мы создаем массив именно строк для удобства работы
	string line;                                        //обьявление строки для считывания
	int i = 0;
	while (getline(*out.stream_of_file, line))          //пока считывается строка
	{
			if (out.array[i-1] == line) 
				throw ERROR_THROW_IN(131, i, 0);
		out.array[i] = line;                            //записываем в массив строк строку
		i++;
		out.array_size++;
		//cout << line << endl;//увеличиваем размер строк
	}
	//

	//тут мы создаем массив лексем
	int counter = 0;                                     // счетчик для массива лексем

	for (int a = 0; a < out.array_size; a++)             //проходим по массиву строк
	{
		unsigned int position = 0;                                // начальная позиция сепаратора равна 0
		for (unsigned int b = 0; b <= out.array[a].length(); b++)     // проходим по символам каждой строки
		{
			if (out.array[a][b] == '|')                  //если символ равен признаку конца строки
			{
				

				out.word[counter][0] = '|';
				counter++;


				bool flag = false;                       //флаг опущен
				for (unsigned int i = 0; i < strlen(probel); i++) //сверка с массивом сепараторов
				{
					if (out.array[a][b - 1] == probel[i])//если предыдущий символ равен одному из сепараторов
					{
						flag = true;
						break;
					}
				}

				if (!flag)                               //если не конец строки(флаг опущен)
				{
					for (unsigned int d = 0; position < b; position++, d++)
					{
						out.word[counter][d] = out.array[a][position];
					}
					counter++; // увеличиваем счетчик лексем
					position = b + 1;


					


					break;
				}
			}

			else
			{
				for (int unsigned i = 0; i < strlen(probel); i++)                        // цикл проверки символа на сепаратор
				{
					if (out.array[a][b] == '=' && out.array[a][b + 1] == '=') // если ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // запускаем цикл копирования символов в массив до пробела от предыдущего сепаратора
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // увеличиваем счетчик лексем
						out.word[counter][0] = '=';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //пропускаем пробел
						break;
					}

                    #pragma region OperationsOfCompare
					if (out.array[a][b] == '!' && out.array[a][b + 1] == '=') // если ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // запускаем цикл копирования символов в массив до пробела от предыдущего сепаратора
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // увеличиваем счетчик лексем
						out.word[counter][0] = '!';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //пропускаем пробел
						break;
					}

					if (out.array[a][b] == '<' && out.array[a][b + 1] == '=') // если ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // запускаем цикл копирования символов в массив до пробела от предыдущего сепаратора
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // увеличиваем счетчик лексем
						out.word[counter][0] = '<';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //пропускаем пробел
						break;
					}

					if (out.array[a][b] == '>' && out.array[a][b + 1] == '=') // если ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // запускаем цикл копирования символов в массив до пробела от предыдущего сепаратора
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // увеличиваем счетчик лексем
						out.word[counter][0] = '>';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //пропускаем пробел
						break;
					}

					if (out.array[a][b] == '>') // если ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // запускаем цикл копирования символов в массив до пробела от предыдущего сепаратора
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // увеличиваем счетчик лексем
						out.word[counter][0] = '>';
						counter++;
						position = b + 1;                              //пропускаем пробел
						break;
					}

					if (out.array[a][b] == '<') // если ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // запускаем цикл копирования символов в массив до пробела от предыдущего сепаратора
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // увеличиваем счетчик лексем
						out.word[counter][0] = '<';
						counter++;
						position = b + 1;                              //пропускаем пробел
						break;
					}

                    #pragma endregion OperationsOfCompare
					//тут мы проверяли на операторы сравнения и заносили в массив лексем ==,!=,>,<,>=,<=

					if (out.array[a][b] == probel[i] || out.array[a][b] == ' ' || out.array[a][b] == '\'') // если равно сепаратору или пробелу или ковычке
					{
						{
							if (out.array[a][b] == ' ')                         // если равно пробелу
							{
								int size = 0;
								for (unsigned int d = 0; position < b; position++, d++)  // запускаем цикл копирования символов в массив до пробела от предыдущего сепаратора
								{
									out.word[counter][d] = out.array[a][position];
									size++;
								}
								counter++;                                     // увеличиваем счетчик лексем
								position = b + 1;                              //пропускаем пробел
								break;
							}

							if (out.array[a][b] == '\'')
							{
								int d = 1;
								b++;
								out.word[counter][0] = '\'';
								for (d; out.array[a][b] != '\''; d++, b++)
								{
									out.word[counter][d] = out.array[a][b];
								}

								out.word[counter][d] = '\'';
								out.word[counter][d + 1] = '\0';

								counter++;
								position = b + 1;
								break;
							}

							else                                               // если не пробел
							{
								if (b != 0)
								{
									int size = 0;
									bool flag_for_counter = false;
									for (unsigned int d = 0; position < b; position++, d++) // запускаем цикл копирования символов в массив до сепаратора
									{
										out.word[counter][d] = out.array[a][position];
										size++;
										flag_for_counter = true;
									}
									if (flag_for_counter)counter++; // увеличиваем счетчик
								}
								out.word[counter][0] = probel[i]; // записываем туда сепаратор
								counter++; // увеличиваем счетчик
								position = b + 1;
								break;
							}


						}

					}
				}
			}
		}
	}
}






void Close(OUT out)
{
	out.stream->close();
	delete out.stream;
}

void Close_out(OUT out)
{
	out.stream_of_file->close();
	delete out.stream_of_file;
}