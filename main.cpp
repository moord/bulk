#include <iostream>
#include "bulk.h"

using namespace std;

int main( int argc, char** argv )
{
    int maxBlockSize =  (argc > 1) ? std::stoi(argv[1]) : 3;

    DataProcessor dp(maxBlockSize);

    std::string command;

    while(getline(cin, command)){

        dp.Get( command );

    }
    return 0;
}
