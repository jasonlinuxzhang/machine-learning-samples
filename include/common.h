#ifndef _COMMON
#define _COMMON
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <getopt.h>
#include <string.h>


class ParameterParse
{
public:
    ParameterParse(int argc, char **argv);
    bool isVaild();
    std::string getValue(std::string key, std::string default_value);
private:
    std::string alog;

    int argc;
    char **argv;

    bool is_valid;
};

bool is_valid_alogrithm(std::string alog);

bool open_log(const char *filename, std::ios_base::openmode mode);

bool close_log(const char *filename);

#endif
