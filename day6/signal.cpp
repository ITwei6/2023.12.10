#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;

void PrintPending()
{
    sigset_t set;//位图变量，输出型参数

    sigpending(&set);//将penging表带出来

    for(int signo=1;signo<=31;signo++)
    {
        if(sigismember(&set,signo))//如果signo信号在set表里就为真
        cout<<"1";
        else
        cout<<"0";
    }
    cout<<endl;

}

void handler(int signo)
{
   cout<<"catch a signal "<<signo<<endl;
}
//保存信号主要靠三张表---block表，pending表和handler表

//现在我们想主动的屏蔽一个信号该如何实现呢？利用sigprocmask系统调用

int main()
{
     //试一试看看能不能将所有信号都屏蔽掉呢？-->不能9和19无法屏蔽
     sigset_t bset,oset;
     sigemptyset(&bset);
     sigemptyset(&oset);

     for(int i=1;i<=31;i++)
     {
        sigaddset(&bset,i);//添加所有信号
     }
     sigprocmask(SIG_SETMASK,&bset,&oset);//屏蔽所有信号

    // //0.首先对2号信号进行捕捉
    // signal(2,handler);
    // //1.对二号信号进行屏蔽
    // //首先要信号是在位图里，所以我们需要利用位图来修改信号，需要位图变量
    // sigset_t bset,oset;//输入型参数，输出型参数，保存就是为了恢复
    // sigemptyset(&bset);//对这两个位图变量初始化
    // sigemptyset(&oset);
    // sigaddset(&bset,2);//将二号信号添加到bset位图里
    // sigprocmask(SIG_SETMASK,&bset,&oset);//覆盖式屏蔽bset位图里的信号

    int cnt=0;
    //2.重复打印当前进程的pending表
    while(true)
    {
        PrintPending();
        sleep(1);
        cnt++;
        if(cnt==10)
        {
          
          cout<<"unblock signal"<<endl;
          sigprocmask(SIG_SETMASK,&oset,nullptr);//将原来的旧表覆盖
        }
    }
}