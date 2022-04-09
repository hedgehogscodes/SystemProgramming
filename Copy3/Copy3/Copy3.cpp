#define _CRT_SECURE_NO_WARNINGS

#include  <windows.h>  
#include  <stdio.h> 
#include <iostream>
#include <string>


int main()
{
	std::string filePath1;
	std::string filePath2;

	std::cout << "Path to file 1: ";
	std::getline(std::cin, filePath1);
	std::cout << "\nPath to file 2: ";
	std::getline(std::cin, filePath2);

	std::wstring stemp1 = std::wstring(filePath1.begin(), filePath1.end());
	std::wstring stemp2 = std::wstring(filePath2.begin(), filePath2.end());

	if (!CopyFile(stemp1.c_str(),stemp2.c_str(),FALSE))
	{
		printf("Ошибка CopyFile : %x\n", GetLastError());
		return 2;
	}
	return 0;
}