#include <iostream>
#include "api/ctp/ThostFtdcMdApi.h"
#include "MyCTPMdSpi.cpp"
#include <stdio.h>
#include "api/ctp/ThostFtdcTraderApi.h"
#include "MyCTPTraderSpi.cpp"

using namespace std;

void testConnect();
void testConnectSQL();
void testTime();
void testTraderConnect();

int main()
{
    cout << "Hello world1!" << endl;

    //testConnect();
    //testConnectSQL();
    testTime();
    testTraderConnect();
    return 0;
}
void testConnect(){
    cout << "testConnect():" << endl;
    CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi("./outfile/",true);
    MyCTPMdSpi sh(pUserApi);
    pUserApi->RegisterSpi(&sh);

    pUserApi->RegisterFront("tcp://180.168.146.187:10010");
    pUserApi->Init();
    pUserApi->Join();
}
void testConnectSQL(){
    cout << "testConnectSQL:" << endl;
}
void testTime(){
     time_t timer;
     time(&timer);
     tm* t_tm = localtime(&timer);
     cout<<"today is "<<t_tm->tm_year+1900<<" "<<t_tm->tm_mon+1<<" "<<t_tm->tm_mday<<endl;
}
void testTraderConnect(){
    cout << "testTraderConnect():" << endl;
    CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./outfile2/");

    MyCTPTraderSpi sh(pUserApi);
    pUserApi->RegisterSpi(&sh);

    pUserApi->RegisterFront("tcp://180.168.146.187:10000");
    pUserApi->Init();
    pUserApi->Join();
}
