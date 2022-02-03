#include "out.h"


using namespace std;

void out_in_file(unsigned char* source, int size, OUT& out)
{

	for (int i = 0; i <= size; i++)
	{
		int if_probel = 0;

		if (*(source + i) == ' ' && (*(source + i - 1) == ' ')) continue;               //���� ��� ������� ������ �� ���������� � �� ��������� ������

		if (*(source + i) == ' ')                                                       //���� ������ ������
		{
			for (unsigned int j = 0; j <= strlen(probel); j++)                          //�������� �� ������ ����� ������� � �� �������
			{
				if (*(source + i + 1) == probel[j] || *(source + i - 1) == probel[j] || *(source + i - 1) == '\n')   //���� ������ ����� ������� ��� �� ���� �� ������� �����������
				{
					if_probel = 1;                                                      //��������� ����
					break;                                                              //������� �� �����
				}
			}
			if (if_probel == 1) continue;                                               //���� ���� ������ �� ���������� �� ��������� ������
		}

		if (*(source + i) == '\n' || i == size)                                          //���� ������ ������� ������ ��� ����� ������
		{
			if (*(source + i - 1) != ';' && *(source + i + 1) != '{' && *(source + i - 1) != '{')   //���� ���������� ������ �� ����� ����� � ������� � ��������� �� ����� ������ � ���������� �� ����� ������
			{
				continue;                                                                           //�� ��������� �����������
			}
			*out.stream << '|';                                                                     //� ���� ������ ���������
			*out.stream << '\n';                                                                    //��������� �� ����� ������
			continue;                                                                               //����������
		}
		*out.stream << *(source + i);                                                               //���� ������ �� �����������, �� ������ �������� ������
	}
}

OUT getout(wchar_t outfile[])                                                                       //������� �������� ����� ��� ������
{
	OUT out;
	out.stream = new ofstream;
	out.stream->open(outfile);
	if (out.stream->fail())
		throw ERROR_THROW(112);
	wcscpy_s(out.outfile, outfile);
	return out;
}

OUT getout_2(wchar_t outfile[])                                                                      //������� �������� ����� ��� ������
{
	OUT out;
	out.stream_of_file = new ifstream;
	out.stream_of_file->open(outfile);
	if (out.stream_of_file->fail())
		throw ERROR_THROW(112);
	wcscpy_s(out.outfile, outfile);
	return out;
}

void out_of_file(OUT out)                                                                            //������� ������ �� ����� � �������� ������� ������
{

	//��� �� �������������� ��������� ������ ��� ������
	for (int i = 0; i < max_word; i++)                  //������ ������� ���������� ������������� ������� ���������������� ��� ������ �����
		out.word[i] = new char[size_word] {NULL};
	//

	//��� �� ������� ������ ������ ����� ��� �������� ������
	string line;                                        //���������� ������ ��� ����������
	int i = 0;
	while (getline(*out.stream_of_file, line))          //���� ����������� ������
	{
			if (out.array[i-1] == line) 
				throw ERROR_THROW_IN(131, i, 0);
		out.array[i] = line;                            //���������� � ������ ����� ������
		i++;
		out.array_size++;
		//cout << line << endl;//����������� ������ �����
	}
	//

	//��� �� ������� ������ ������
	int counter = 0;                                     // ������� ��� ������� ������

	for (int a = 0; a < out.array_size; a++)             //�������� �� ������� �����
	{
		unsigned int position = 0;                                // ��������� ������� ���������� ����� 0
		for (unsigned int b = 0; b <= out.array[a].length(); b++)     // �������� �� �������� ������ ������
		{
			if (out.array[a][b] == '|')                  //���� ������ ����� �������� ����� ������
			{
				

				out.word[counter][0] = '|';
				counter++;


				bool flag = false;                       //���� ������
				for (unsigned int i = 0; i < strlen(probel); i++) //������ � �������� �����������
				{
					if (out.array[a][b - 1] == probel[i])//���� ���������� ������ ����� ������ �� �����������
					{
						flag = true;
						break;
					}
				}

				if (!flag)                               //���� �� ����� ������(���� ������)
				{
					for (unsigned int d = 0; position < b; position++, d++)
					{
						out.word[counter][d] = out.array[a][position];
					}
					counter++; // ����������� ������� ������
					position = b + 1;


					


					break;
				}
			}

			else
			{
				for (int unsigned i = 0; i < strlen(probel); i++)                        // ���� �������� ������� �� ���������
				{
					if (out.array[a][b] == '=' && out.array[a][b + 1] == '=') // ���� ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // ��������� ���� ����������� �������� � ������ �� ������� �� ����������� ����������
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // ����������� ������� ������
						out.word[counter][0] = '=';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //���������� ������
						break;
					}

                    #pragma region OperationsOfCompare
					if (out.array[a][b] == '!' && out.array[a][b + 1] == '=') // ���� ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // ��������� ���� ����������� �������� � ������ �� ������� �� ����������� ����������
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // ����������� ������� ������
						out.word[counter][0] = '!';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //���������� ������
						break;
					}

					if (out.array[a][b] == '<' && out.array[a][b + 1] == '=') // ���� ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // ��������� ���� ����������� �������� � ������ �� ������� �� ����������� ����������
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // ����������� ������� ������
						out.word[counter][0] = '<';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //���������� ������
						break;
					}

					if (out.array[a][b] == '>' && out.array[a][b + 1] == '=') // ���� ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // ��������� ���� ����������� �������� � ������ �� ������� �� ����������� ����������
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // ����������� ������� ������
						out.word[counter][0] = '>';
						out.word[counter][1] = '=';
						counter++;
						position = b + 2;                              //���������� ������
						break;
					}

					if (out.array[a][b] == '>') // ���� ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // ��������� ���� ����������� �������� � ������ �� ������� �� ����������� ����������
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // ����������� ������� ������
						out.word[counter][0] = '>';
						counter++;
						position = b + 1;                              //���������� ������
						break;
					}

					if (out.array[a][b] == '<') // ���� ==
					{
						int size = 0;
						for (unsigned int d = 0; position < b; position++, d++)  // ��������� ���� ����������� �������� � ������ �� ������� �� ����������� ����������
						{
							out.word[counter][d] = out.array[a][position];
							size++;
						}
						counter++;                                     // ����������� ������� ������
						out.word[counter][0] = '<';
						counter++;
						position = b + 1;                              //���������� ������
						break;
					}

                    #pragma endregion OperationsOfCompare
					//��� �� ��������� �� ��������� ��������� � �������� � ������ ������ ==,!=,>,<,>=,<=

					if (out.array[a][b] == probel[i] || out.array[a][b] == ' ' || out.array[a][b] == '\'') // ���� ����� ���������� ��� ������� ��� �������
					{
						{
							if (out.array[a][b] == ' ')                         // ���� ����� �������
							{
								int size = 0;
								for (unsigned int d = 0; position < b; position++, d++)  // ��������� ���� ����������� �������� � ������ �� ������� �� ����������� ����������
								{
									out.word[counter][d] = out.array[a][position];
									size++;
								}
								counter++;                                     // ����������� ������� ������
								position = b + 1;                              //���������� ������
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

							else                                               // ���� �� ������
							{
								if (b != 0)
								{
									int size = 0;
									bool flag_for_counter = false;
									for (unsigned int d = 0; position < b; position++, d++) // ��������� ���� ����������� �������� � ������ �� ����������
									{
										out.word[counter][d] = out.array[a][position];
										size++;
										flag_for_counter = true;
									}
									if (flag_for_counter)counter++; // ����������� �������
								}
								out.word[counter][0] = probel[i]; // ���������� ���� ���������
								counter++; // ����������� �������
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