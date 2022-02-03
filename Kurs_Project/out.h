#pragma once
#define probel "=,+-:)(%/*{};"
#include <fstream>
#include "Error.h"
#include <string>
#define max_word 1000
#define size_word 20



struct OUT
{
	wchar_t outfile[300];

	int array_size = 0;
	char** word = new char* [max_word];

	std::string* array = new std::string[100];
	std::string* array_lex = new std::string[1000];

	std::ofstream* stream;
	std::ifstream* stream_of_file;

	/*~OUT()
	{
		delete[]outfile;
		delete[]array;
		delete[]stream;
		delete[]stream_of_file;
	}
	*/

};

void out_in_file(unsigned char* source, int size, OUT& out);
OUT getout(wchar_t outfile[]);
OUT getout_2(wchar_t outfile[]);

void out_of_file(OUT out);

void Close(OUT out);
void Close_out(OUT out);
