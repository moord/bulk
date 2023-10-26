#ifndef BULK_H
#define BULK_H

#include <chrono>
#include <deque>
#include <string>

enum BulkTypeSize { STATIC_SIZE = 0, DYNAMIC_SIZE = 1 };

//////////////////////////////////////////////////////////////
// класс Накопитель пачек команд
//////////////////////////////////////////////////////////////

class cBulk{
private:
    std::chrono::time_point<std::chrono::system_clock>  start; // время получения 1 команды
    std::deque<std::string> cmds;                              // очередь команд

    // ..

    void DisplayAndSave(); // вывести очередь команд в консоль и сохранить в файл
public:
    cBulk( BulkTypeSize AType): type(AType){};

    void Add( std::string cmd, int maxSize = 0 ); // добавить команду в очередь команд
    void Complete();                              // завершить

    BulkTypeSize type;
};

//////////////////////////////////////////////////////////////
// класс Обработчик команд
//////////////////////////////////////////////////////////////

class DataProcessor{
private:
    cBulk dynamicBulk{BulkTypeSize::DYNAMIC_SIZE};
#ifndef COMMON_STATIC_BULK
    cBulk staticBulk{BulkTypeSize::STATIC_SIZE};
#endif
    cBulk *activeBulk;

    int depth;
    int N;

    //..

    std::string cmd;

    void Process();                             // обработка команды
public:

    DataProcessor(int AN ):N(AN), depth(0){
        activeBulk = &staticBulk;
    };

    ~DataProcessor(){
        if( activeBulk->type == BulkTypeSize::STATIC_SIZE  ){
            activeBulk->Complete();
        }
    };

    void Get( const char *, std::size_t);       // принять и обработать буфер команд
    void Get( std::string & );                  // принять и обработать одиночную команду
};

#endif // BULK_H
