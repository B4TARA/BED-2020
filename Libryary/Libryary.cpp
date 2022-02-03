#pragma once
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <time.h>
#include "Libryary.h"

#pragma warning(disable: 4996)

extern "C"
{
	static bool isHex = false;
	static bool isBin = false;
	char* _stdcall strcon(char* str1, char* str2)
	{
		char* buf = (char*)malloc(255);
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		if (!str1 || !str2)
		{
			std::cout << "ERROR: Null string in strcon function!!!" << std::endl;
			system("pause");
			ExitProcess(0);
		}
		int size = 0;
		for (; str2[size] != '\0'; size++)
			buf[size] = str2[size];
		for (int i = 0; str1[i] != '\0'; size++, i++)
			buf[size] = str1[i];
		buf[size] = '\0';
		return  buf;
	}
	void _stdcall Date()
	{
		tm* timeinf;
		time_t timet;
		time(&timet);
		timeinf = localtime(&timet);
		char temp[100];
		strftime(temp, sizeof(temp), "%d.%m.%y", timeinf);
		std::cout << temp << std::endl;
	}
	void _stdcall Time()
	{
		tm* timeinf;
		time_t timet;
		time(&timet);
		timeinf = localtime(&timet);
		char temp[100];
		strftime(temp, sizeof(temp), "%T", timeinf);
		std::cout << temp << std::endl;
	}
	void _stdcall outputstr(char* s)
	{
		if (s != nullptr)
		{
			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);
			std::cout << s << std::endl;

		}
		else
			std::cout << "Empty line!";
	}
	void _stdcall outputint(int i)
	{
		std::cout << i << std::endl;;
	}
	void _stdcall outputuint(unsigned int i)
	{
		std::cout << i << std::endl;
	}
}