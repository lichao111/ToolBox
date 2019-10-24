#This demo is used to test function work in Multhi-Threads 
#build your own work function ,and put it in the third parament of the phread_creat function

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include "/home/oracle/Documents/MTime/MOracleDateTime.h"

#define NUM_THREADS 10 
using namespace std;

string MAddMinuteOfDate(string strDate,int num);
void* Work(void *args);
void* Work2(void *args);
void* Work3(void *args);

int main()
{
        pthread_t tids[NUM_THREADS];
        time_t StartTime = time(NULL);
        int ret  = 0;
        for(int i = 0;i < NUM_THREADS;i++)
        {       
                //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
                ret = pthread_create(&tids[i],NULL,Work3, NULL);
        }
        if(ret != 0)
        {
                cout << "pthread_create error: error_code=" << ret << endl;
        }
        cout<<"running...."<<endl;
        //sleep(20);
        for(int i = 0;i < NUM_THREADS;i++)
        {
            pthread_join(tids[i],NULL);
        }
        cout<<"use time: "<<time(NULL) - StartTime<<" S "<<endl;
        pthread_exit(NULL);

}

void* Work(void *args)
{
    string   StartTime = "20190101000000";
    for(int i = 0 ;i<10000;i++)
    {
        StartTime = MAddMinuteOfDate(StartTime,15);
    }
    cout<<"thread Finish,End Time: "<<StartTime<<endl;
}
void* Work2(void *args)
{
    TMOracleDateTime *t = new TMOracleDateTime;
    Date StartTime = t->MSetDate("20190111236000","YYYYMMDDhh24miss");
    for(int i = 0 ;i<1000000;i++)
    {
        StartTime = t->MAddMinuteOfDate(StartTime,15);
    }
    string t_strtTime = t->MFormatDate(StartTime,"yyyymmddhh24miss");
    cout<<"thread Finish,End Time: "<<t_strtTime<<endl;
}
void* Work3(void *args)
{
    TMOracleDateTime *t = new TMOracleDateTime;
    Date StartTime;
    try
    {
        StartTime = t->MSetDate("20191020","YYYYMMDDhh24miss");
    }
    catch(...)
    {
            std::cerr<<"wrong time"<<std::endl;
            return NULL;
    }
    for(int i = 0 ;i<1000000;i++)
    {
        StartTime = t->MAddMinuteOfDate(StartTime,15);
    }
    string t_strtTime = t->MFormatDate(StartTime,"yyyymmddhh24miss");
    cout<<"thread Finish,End Time: "<<t_strtTime<<endl;
}
string MAddMinuteOfDate(string strDate,int num)
{
    struct tm _p;
    while (strDate.size()<14)
    {
        strDate += '0';
    }
    sscanf(strDate.c_str(), "%4d%2d%2d%2d%2d%2d",
    &_p.tm_year,
    &_p.tm_mon,
    &_p.tm_mday,
    &_p.tm_hour,
    &_p.tm_min,
    &_p.tm_sec);
    _p.tm_year = _p.tm_year - 1900;
    _p.tm_mon = _p.tm_mon - 1;
    time_t dt = mktime(&_p) + (num * 60);
    struct tm _tm;
    localtime_r(&dt, &_tm);
    char tempbuf[20] = { 0 };
    sprintf(tempbuf, "%04d%02d%02d%02d%02d%02d", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
    string s(tempbuf);
    return s;
};
