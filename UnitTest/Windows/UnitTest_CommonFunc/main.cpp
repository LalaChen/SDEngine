#include <iostream>

#include "SDEngineCommonFunction.h"

using namespace SDE::Basic;

int main(int argc, char **argv)
{
    std::cout << "SDEngine Common Func Unit Test" << std::endl;

    //1. String Format.
    {
        int six = 666666;
        std::string result;
        result = StringFormat("Test six = %d and convert string", six);
        std::cout << "Test 1 : StringFormat => " << result << ". size = "<< result.size() <<". length = "<< result.length()<< std::endl;
    }

    return 0;
}