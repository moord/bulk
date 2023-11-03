#include "bulk.h"

#include <iostream>
#include <list>
#include <iterator>
#include <fstream>
#include <thread>
#include <algorithm>

 //---------------------------------------------------------------------------

 void cBulk::Subscribe(Observer *obs) {
    m_subs.insert(obs);
 }
 //---------------------------------------------------------------------------

 void cBulk::Notify(){
     if( !cmds.empty() ){
         for (auto sub : m_subs)
         {
             sub->update( std::chrono::system_clock::now(), cmds);
         }

         cmds.clear();
     }
 }
 //---------------------------------------------------------------------------

void cBulk::Complete( ){
    Notify();
}
//---------------------------------------------------------------------------

void cBulk::Add( std::string cmd, int maxSize ){
    if( cmds.size() == 0){
        start = std::chrono::system_clock::now();
    }

    cmds.push_back(cmd);

    if( (type == BulkTypeSize::STATIC_SIZE && cmds.size() >= maxSize)) {
        Notify();
    }
}
//---------------------------------------------------------------------------

void DataProcessor::Process(){  //
    if (cmd == "{") {
        if( activeBulk->GetType() == BulkTypeSize::STATIC_SIZE  ){
            activeBulk->Complete();
            activeBulk = dynamicBulk;
        }
        depth++;
    }
    else if (cmd == "}") {
        if( activeBulk->GetType() != BulkTypeSize::STATIC_SIZE  ){
            if (--depth == 0) {
                activeBulk->Complete();
                activeBulk = staticBulk;
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
