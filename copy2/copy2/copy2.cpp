
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>  
#include <errno.h>  
#include <iostream>
#include <string>
#include <limits>

#define  BUF_SIZE   256 

int main()
{
    HANDLE  hIn, hOut;
    DWORD  nIn, nOut;
    CHAR Buffer[BUF_SIZE];

    std::string filePath1;
    std::string filePath2;

    std::cout << "Path to file 1: ";
    std::getline(std::cin, filePath1);
    std::cout << "Path to file 2: ";
    std::getline(std::cin, filePath2);

    std::wstring stemp1 = std::wstring(filePath1.begin(), filePath1.end());
    std::wstring stemp2 = std::wstring(filePath2.begin(), filePath2.end());

    hIn = CreateFile(stemp1.c_str(),
        GENERIC_READ, 0, NULL,
        OPEN_EXISTING, 0, NULL);

    if (hIn == INVALID_HANDLE_VALUE)
    {
        printf("Нельзя открыть входной файл.Ошибка: %х \n", GetLastError());
        return 2;
    }

    hOut = CreateFileW(
        stemp2.c_str(),
        GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hOut == INVALID_HANDLE_VALUE)
    {
        printf("Нельзя открыть выходной файл.Ошибка: %х \n", GetLastError());
        return 3;
    }

    while (ReadFile(hIn, Buffer, BUF_SIZE, &nIn, NULL) && nIn > 0)
    {
        WriteFile(hOut, Buffer, nIn, &nOut, NULL);
        if (nIn != nOut)
        {
            printf("Неисправимая  ошибка  записи : %x \n", GetLastError());
            return  4;
        }
    }
    CloseHandle(hIn);
    CloseHandle(hOut);
    return  0;
}
