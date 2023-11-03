#include "logger.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>


//---------------------------------------------------------------------------

void ConsoleLog::update( std::chrono::time_point<std::chrono::system_clock> start, std::deque<std::string> cmds ){
    std::cout << "bulk: ";
    std::copy(cmds.begin(), --cmds.end(), std::ostream_iterator<std::string>(std::cout, ", "));
    std::cout << cmds.back() << "\n";
}
//---------------------------------------------------------------------------

void FileLog::update( std::chrono::time_point<std::chrono::system_clock>  start, std::deque<std::string> cmds ){
    std::ostringstream stream;
    std::copy(cmds.begin(), cmds.end(), std::ostream_iterator<std::string>(stream, ""));
    std::string fileName = "bulk" + std::to_string( std::chrono::system_clock::to_time_t(start)) + "_" + std::to_string(std::hash<std::string> {}(stream.str())) + ".log";

    std::ofstream out(fileName);
    while(!cmds.empty()) {
        out << cmds.front() << "\n";
        cmds.pop_front();
    }
    out.close();
}
//---------------------------------------------------------------------------

