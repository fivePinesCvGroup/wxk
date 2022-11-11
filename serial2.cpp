#include <fcntl.h>//文件控制头文件，用于文件及其描述符的操作控制常数符号的定义。
#include <unistd.h>//Linux标准头文件，定义了各种符号常数和类型，并声明了各种函数。如，定义了__LIBRARY__，则还包括系统调用号和内嵌汇编_syscall0()等。
#include <termios.h>//终端输入输出函数头文件，主要定义控制异步通信口的终端接口。
#include <errno.h>//错误号头文件，包含系统中各种出错号。(Linus从minix中引进的)。
#include <iostream>//数据流输入／输出
#include <fstream>//文件输入／输出
#include <string>//字符串类
#include <string.h>//字符串头文件，主要定义了一些有关字符串操作的嵌入函数。
#include <stdexcept>//标准异常类
#include <exception>//异常处理类
#include <stdio.h>
#include <stdint.h>//定义了 int16_t 、 uint32_t 、 int64_t 等整型
#include <mutex>//Mutex又称互斥量，状态锁
#include <chrono>// 时间库
#include<thread>//此头文件主要声明了std::thread线程类
#include <cmath>//定义数学函数
#include "queue"//STL 队列容器
#include"serial2.h"

using namespace std;


Serial::Serial():
        _serialFd(-1)
{
    // static_assert(sizeof(ControlSerial) == 1, "Size of backdata is not 4");
}

Serial::~Serial()
{
    tcflush(_serialFd, TCIOFLUSH); //tcflush函数刷清（扔掉）输入缓存（终端驱动法度已接管到，但用户法度尚未读）或输出缓存（用户法度已经写，但尚未发送）,清空终端未完成的输入/输出请求和数据
}

int Serial::openPort()
{
    _serialFd = open("/dev/ttyTHS2", O_RDWR | O_NOCTTY | O_NONBLOCK);
    //对应妙算上的UART1，O_RDWR为读、写打开，
        //O_NOCTTY为O_NOCTTY，如果pathname指的是终端设备，则不将此设备分配作为此进程的控制终端。
        //O_NONBLOCK，如果pathname指的是一个FIFO、一个块特殊文件或一个字符特殊文件，则此选择项为此文件的本次打开操作和后续的I/O操作设置非阻塞方式。
    if (_serialFd == -1)
    {
        cout<<"erro"<<endl;
        return _errorCode = 1;;
    }


    termios tOption;                                // termios结构体，串口配置结构体
    tcgetattr(_serialFd, &tOption);                 //tcgetattr函数用于获取与终端相关的参数,获取当前设置
    cfmakeraw(&tOption);//cfmakeraw()函数可以将终端设置为原始模式，在原始模式下，所有的输入数据以字节为单位被处理
    cfsetispeed(&tOption, B115200);                 // 接收波特率
    cfsetospeed(&tOption, B115200);                 // 发送波特率
    tcsetattr(_serialFd, TCSANOW, &tOption);        //csetattr函数用于设置终端的相关参数。参数fd为打开的终端文件描述符，参数optional_actions用于控制修改起作用的时间，TCSANOW：立刻对值进行修改
    tOption.c_cflag &= ~PARENB;                     //启用奇偶校验码的生成和检测功能
    tOption.c_cflag &= ~CSTOPB;                     //
    tOption.c_cflag &= ~CSIZE;                     //字体大小
    tOption.c_cflag |= CS8;                        //CS5/6/7/8表示发送或接收字符时使用5/6/7/8比特。
    tOption.c_cflag &= ~INPCK;
    tOption.c_cflag |= (115200 | CLOCAL | CREAD);  // 设置波特率，本地连接，接收使能
    tOption.c_cflag &= ~(INLCR | ICRNL);
    tOption.c_cflag &= ~(IXON);
    tOption.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tOption.c_oflag &= ~OPOST;
    tOption.c_oflag &= ~(ONLCR | OCRNL);
    tOption.c_iflag &= ~(ICRNL | INLCR);
    tOption.c_iflag &= ~(IXON | IXOFF | IXANY);
    tOption.c_cc[VTIME] = 1;                        //只有设置为阻塞时这两个参数才有效
    tOption.c_cc[VMIN] = 2;
    tcflush(_serialFd, TCIOFLUSH);                  //TCIOFLUSH刷新输入、输出队列。

    return _errorCode = 0;
}

int Serial::closePort()
{
    tcflush(_serialFd, TCIOFLUSH);
    if (-1 == close(_serialFd))
    cout<<"erro"<<endl;
}

bool Serial::isOpened() const
{
    return (_serialFd != -1);
}

int Serial::control(const ControlData& controlData)
{
    //pack包装字符串并传入
    _controlSerial = pack(controlData);
    return send();
}


Serial::ControlSerial Serial::pack(const ControlData& ctrl)//打包结构体里的数据
{
    return ControlSerial
            {
                ctrl.retValue
            };
}


int Serial::send()
{
    tcflush(_serialFd, TCOFLUSH);

    int sendCount;
    try
    {
        sendCount  = write(_serialFd, &_controlSerial, sizeof(ControlSerial));//将缓存区里的东西写入到串口中，controlSerial
    }
    catch(exception e)
    {
        return _errorCode = 1;
    }
    return _errorCode;
}
 int main()
{

    ControlData data_;
     uint16_t val;
     Serial test;
     test.openPort();
while(1)
{
   
    if(test.isOpened())
    {
        cin>>val;
        data_.retValue = val;
        test.control(data_);
    }
}
    
    test.closePort();
}

