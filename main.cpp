#include <iostream>
#include <memory>
#include "bulk.h"

ConsoleLog logger;
FileLog saver;

int main( int argc, char** argv )
{
    int maxBlockSize =  (argc > 1) ? std::stoi(argv[1]) : 3;

    auto statBulk = std::make_shared<cBulk>(BulkTypeSize::STATIC_SIZE);

    statBulk->Subscribe(&logger);
    statBulk->Subscribe(&saver);

    auto dynBulk = std::make_shared<cBulk>(BulkTypeSize::DYNAMIC_SIZE);

    dynBulk->Subscribe(&logger);
    dynBulk->Subscribe(&saver);

    DataProcessor dp(maxBlockSize, dynBulk, statBulk);

    std::string command;

    while(getline(std::cin, command)){

        dp.Get( command );

    }
    return 0;
}
