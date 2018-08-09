#include "common.h"

std::ifstream log_file;

struct option long_options[] = {
{"alogrithm",   required_argument, 0,  'a' },
{0,         0,                 0,  0 }
};

bool ParameterParse::isVaild()
{
    return this->is_valid;
}

ParameterParse::ParameterParse(int argc, char **argv)
{
    this->argc = argc;
    this->argv = argv; 
    this->is_valid = true;

    int c, option_index = 0;
    bool process_stop_flag = true;
    while(process_stop_flag)
    {
        c = getopt_long(this->argc, this->argv, "a:", long_options, &option_index); 
        switch(c)
        {
            case ('a'):
                if((is_valid = is_valid_alogrithm(optarg)) == false)
                {
                    std::cout<<optarg<<" is invalid."<<std::endl;
                    process_stop_flag = false;
                    break;
                }
                this->alog = alog; 
                break;
            case ('?'):
                std::cout<<"Exist invalid parameters: "<<argv[optind - 1]<<std::endl;;
                this->is_valid = false;
                process_stop_flag = false;
                break;
            case (-1): //all parameters are processed
                process_stop_flag = false;
                break;
        }
    }
    if(optind < argc)
    {
        std::cout<<"remain unprocessed parameters:"; 
        while(optind < argc)
            std::cout<<argv[optind++]<<" ";
        std::cout<<"\n";
        this->is_valid = false;
        return;
    }
}



bool is_valid_alogrithm(std::string alog)
{
    if(0 == strcmp(alog.c_str(), "id3"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool open_log(const char *filename, std::ios_base::openmode mode)
{
    log_file.open(filename, mode);
    return log_file; //transform to bool
}

bool close_log(const char *filename)
{
    log_file.close(); 
}
