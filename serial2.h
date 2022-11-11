#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdexcept>
#include <exception>
#include <stdio.h>
#include <stdint.h>
#include <mutex>
#include <chrono>
#include<thread>
#include <cmath>
#include "queue"

using namespace std;


 struct ControlData
{
    uint16_t retValue;//_t可以理解为typedef的缩写，2字节
};


class Serial
{
public:

    Serial();
    Serial(const Serial& right) = delete;
    Serial(Serial&& ) = delete;
    ~Serial();


    int openPort();
    int closePort();
    bool isOpened() const;


public:
    int _serialFd;//串口

    int _errorCode;


    // 控制返回值
    struct ControlSerial
    {
        uint16_t retValue;
    }_controlSerial;


   // std::timed_mutex _mutex;//时间间隔



public:

    int control(const ControlData& controlData);

    ControlSerial pack(const ControlData& controlData);

    int send();
   

};
