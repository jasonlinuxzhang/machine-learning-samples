#include <iostream>
#include "common.h"
#include "dt.h"


extern std::ofstream log_file;

int main(int argc, char *argv[])
{
    ParameterParse paramparse = ParameterParse(argc, argv);    

    if(!paramparse.isVaild())
    {
        std::cout<<"Parameter parse fail."<<std::endl;
        return -1;
    }

    if(!open_log("/tmp/log", std::ios_base::out|std::ios_base::app))
    {
        std::cout<<"open log fail."<<std::endl;
        return -1;
    }


    ID3 train("/home/jason/disk/machine_learning/machine-learning-samples/train.dat");

    train.loadData();
    
    close_log("/tmp/log");
 
    train.dumpData();
}
