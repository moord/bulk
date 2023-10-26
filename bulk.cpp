#include "bulk.h"

#include <iostream>
#include <list>
#include <iterator>
#include <fstream>

#ifdef COMMON_STATIC_BULK
    cBulk staticBulk(BulkTypeSize::STATIC_SIZE);
#endif

//---------------------------------------------------------------------------

void cBulk::DisplayAndSave(){
    if( !cmds.empty() ){
        // вывод в консоль
        std::cout << "bulk: ";
        std::copy(cmds.begin(), --cmds.end(), std::ostream_iterator<std::string>(std::cout, ", "));
        std::cout << cmds.back() << "\n";

        // открываем файл для сохранения
        std::string fileName = "bulk" + std::to_string( std::chrono::system_clock::to_time_t(start)) + "_" + std::to_string(std::rand()) + ".log";
        std::ofstream out(fileName);
        while(!cmds.empty()) {
            out << cmds.front() << "\n";
            cmds.pop_front();
        }
        out.close();
    }
}
//---------------------------------------------------------------------------

void cBulk::Complete( ){
    DisplayAndSave();
}
//---------------------------------------------------------------------------

void cBulk::Add( std::string cmd, int maxSize ){
    if( cmds.size() == 0){
        start = std::chrono::system_clock::now();
    }

    cmds.push_back(cmd);

    if( (type == BulkTypeSize::STATIC_SIZE && cmds.size() >= maxSize)) {
        DisplayAndSave();
    }
}
//---------------------------------------------------------------------------

void DataProcessor::Process(){  //
    if (cmd == "{") {
        if( activeBulk->type == BulkTypeSize::STATIC_SIZE  ){
            activeBulk->Complete();
            activeBulk = &dynamicBulk;
        }
        depth++;
    }
    else if (cmd == "}") {
        if( activeBulk->type != BulkTypeSize::STATIC_SIZE  ){
            if (--depth == 0) {
                activeBulk->Complete();
                activeBulk = &staticBulk;
            }
        }
    }
    else if(cmd.size() > 0 ){
        activeBulk->Add(cmd,N);
    }
    cmd ="";
}
//---------------------------------------------------------------------------

void DataProcessor::Get( const char *data, std::size_t size) {
    for (auto i = 0; i < size; ++i) {
       if( data[i] == '\n' ){
            Process();
       } else{
           cmd += data[i];
       }
    }
}
//---------------------------------------------------------------------------

void DataProcessor::Get( std::string &ACmd ){
    cmd = ACmd;
    Process();
}
//---------------------------------------------------------------------------
