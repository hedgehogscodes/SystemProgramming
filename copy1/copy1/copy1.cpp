
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <errno.h>  
#include <iostream>
#include <string>

#define BUF_SIZE 256 


int main()
{
    FILE* in_file, * out_file;
    char rec[BUF_SIZE];
    size_t bytes_in;
    size_t bytes_out;

    std::string filePath1;
    std::string filePath2;

    std::cout << "Path to file 1: ";
    std::getline(std::cin, filePath1);
    std::cout << "Path to file 2: ";
    std::getline(std::cin, filePath2);

    in_file = fopen(filePath1.c_str(), "rb");
    if (in_file == NULL)
    {
        perror(filePath1.c_str());
        return 2;
    }

    out_file = fopen(filePath2.c_str(), "wb");
    if (out_file == NULL)
    {
        perror(filePath2.c_str());
        return 3;
    }

    while ((bytes_in = fread(rec, 1, BUF_SIZE, in_file)) > 0)
    {
        bytes_out = fwrite(rec, 1, bytes_in, out_file);
        if (bytes_out != bytes_in)
        {
            perror("Неисправимая ошибка записи.");
            return 4;
        }
    }
    fclose(in_file);
    fclose(out_file);
}
