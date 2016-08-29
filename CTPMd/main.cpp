#include "api/ctp/ThostFtdcMdApi.h"
#include "api/ctp/ThostFtdcTraderApi.h"
#include "MyCTPMdSpi.h"
#include "MyCTPTraderSpi.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <thread>
#define DBL_MAX   1.7976931348623158e+308

using namespace std;

extern vector<CThostFtdcDepthMarketDataField> m_dataList;

void testConnect();
void testConnectSQL();
void testTime();
void testTraderConnect();

void saveDataToFile(vector<CThostFtdcDepthMarketDataField> & dataList){
    cout << dataList.size()<< endl;
    for(int i=0;i<dataList.size();++i){
        CThostFtdcDepthMarketDataField pDepthMarketData = dataList[i];

        char filename[80] =  "./";
        strcat(filename,pDepthMarketData.TradingDay);
        strcat(filename,"/");
        strcat(filename,pDepthMarketData.InstrumentID);
        strcat(filename,".txt");

        char filename2[80] = "./";
        strcat(filename2,pDepthMarketData.TradingDay);
        strcat(filename2,".txt");
        cout <<"  " << filename <<"  " << filename2 << endl;
        //string filename = "./"+pDepthMarketData->TradingDay+"/"+pDepthMarketData->InstrumentID+".txt";
        ofstream _file;
        ofstream _file2;
        _file.open(filename,ios::app);
        _file2.open(filename2,ios::app);

        _file << pDepthMarketData.TradingDay <<",";
        _file << pDepthMarketData.InstrumentID <<",";
        _file << pDepthMarketData.ExchangeID <<",";
        _file << pDepthMarketData.ExchangeInstID <<",";
        _file << pDepthMarketData.LastPrice <<",";
        _file << pDepthMarketData.PreSettlementPrice <<",";
        _file << pDepthMarketData.PreClosePrice <<",";
        _file << pDepthMarketData.PreOpenInterest <<",";
        _file << pDepthMarketData.OpenPrice <<",";
        _file << pDepthMarketData.HighestPrice <<",";
        _file << pDepthMarketData.LowestPrice <<",";
        _file << pDepthMarketData.Volume <<",";
        _file <<setprecision(15)<< pDepthMarketData.Turnover <<",";
        _file << pDepthMarketData.OpenInterest <<",";
        if(pDepthMarketData.ClosePrice == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.ClosePrice <<",";
        if(pDepthMarketData.SettlementPrice == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.SettlementPrice <<",";
        _file << pDepthMarketData.UpperLimitPrice <<",";
        _file << pDepthMarketData.LowerLimitPrice <<",";
        if(pDepthMarketData.PreDelta == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.PreDelta <<",";
        if(pDepthMarketData.CurrDelta == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.CurrDelta <<",";
        _file << pDepthMarketData.UpdateTime <<",";
        _file << pDepthMarketData.UpdateMillisec <<",";
        if(pDepthMarketData.BidPrice1 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.BidPrice1 <<",";
        _file << pDepthMarketData.BidVolume1 <<",";
        if(pDepthMarketData.AskPrice1 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.AskPrice1 <<",";
        _file << pDepthMarketData.AskVolume1 <<",";
        if(pDepthMarketData.BidPrice2 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.BidPrice2 <<",";
        _file << pDepthMarketData.BidVolume2 <<",";
        if(pDepthMarketData.AskPrice2 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.AskPrice2 <<",";
        _file << pDepthMarketData.AskVolume2 <<",";
        if(pDepthMarketData.BidPrice3 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.BidPrice3 <<",";
        _file << pDepthMarketData.BidVolume3 <<",";
        if(pDepthMarketData.AskPrice3 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.AskPrice3 <<",";
        _file << pDepthMarketData.AskVolume3 <<",";
        if(pDepthMarketData.BidPrice4 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.BidPrice4 <<",";
        _file << pDepthMarketData.BidVolume4 <<",";
        if(pDepthMarketData.AskPrice4 == DBL_MAX)
            _file << ",";
         else
            _file << pDepthMarketData.AskPrice4 <<",";
        _file << pDepthMarketData.AskVolume4 <<",";
        if(pDepthMarketData.BidPrice5 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.BidPrice5 <<",";
        _file << pDepthMarketData.BidVolume5 <<",";
        if(pDepthMarketData.AskPrice5 == DBL_MAX)
            _file << ",";
        else
            _file << pDepthMarketData.AskPrice5 <<",";
        _file << pDepthMarketData.AskVolume5 <<",";
        _file << pDepthMarketData.AveragePrice <<",";
        _file << pDepthMarketData.ActionDay <<endl;

        _file2 << pDepthMarketData.TradingDay <<",";
        _file2 << pDepthMarketData.InstrumentID <<",";
        _file2 << pDepthMarketData.ExchangeID <<",";
        _file2 << pDepthMarketData.ExchangeInstID <<",";
        _file2 << pDepthMarketData.LastPrice <<",";
        _file2 << pDepthMarketData.PreSettlementPrice <<",";
        _file2 << pDepthMarketData.PreClosePrice <<",";
        _file2 << pDepthMarketData.PreOpenInterest <<",";
        _file2 << pDepthMarketData.OpenPrice <<",";
        _file2 << pDepthMarketData.HighestPrice <<",";
        _file2 << pDepthMarketData.LowestPrice <<",";
        _file2 << pDepthMarketData.Volume <<",";
        _file2 <<setprecision(15)<< pDepthMarketData.Turnover <<",";
        _file2 << pDepthMarketData.OpenInterest <<",";
        if(pDepthMarketData.ClosePrice == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.ClosePrice <<",";
        if(pDepthMarketData.SettlementPrice == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.SettlementPrice <<",";
        _file2 << pDepthMarketData.UpperLimitPrice <<",";
        _file2 << pDepthMarketData.LowerLimitPrice <<",";
        if(pDepthMarketData.PreDelta == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.PreDelta <<",";
        if(pDepthMarketData.CurrDelta == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.CurrDelta <<",";
        _file2 << pDepthMarketData.UpdateTime <<",";
        _file2 << pDepthMarketData.UpdateMillisec <<",";
        if(pDepthMarketData.BidPrice1 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.BidPrice1 <<",";
        _file2 << pDepthMarketData.BidVolume1 <<",";
        if(pDepthMarketData.AskPrice1 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.AskPrice1 <<",";
        _file2 << pDepthMarketData.AskVolume1 <<",";
        if(pDepthMarketData.BidPrice2 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.BidPrice2 <<",";
        _file2 << pDepthMarketData.BidVolume2 <<",";
        if(pDepthMarketData.AskPrice2 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.AskPrice2 <<",";
        _file2 << pDepthMarketData.AskVolume2 <<",";
        if(pDepthMarketData.BidPrice3 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.BidPrice3 <<",";
        _file2 << pDepthMarketData.BidVolume3 <<",";
        if(pDepthMarketData.AskPrice3 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.AskPrice3 <<",";
        _file2 << pDepthMarketData.AskVolume3 <<",";
        if(pDepthMarketData.BidPrice4 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.BidPrice4 <<",";
        _file2 << pDepthMarketData.BidVolume4 <<",";
        if(pDepthMarketData.AskPrice4 == DBL_MAX)
            _file2 << ",";
         else
            _file2 << pDepthMarketData.AskPrice4 <<",";
        _file2 << pDepthMarketData.AskVolume4 <<",";
        if(pDepthMarketData.BidPrice5 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.BidPrice5 <<",";
        _file2 << pDepthMarketData.BidVolume5 <<",";
        if(pDepthMarketData.AskPrice5 == DBL_MAX)
            _file2 << ",";
        else
            _file2 << pDepthMarketData.AskPrice5 <<",";
        _file2 << pDepthMarketData.AskVolume5 <<",";
        _file2 << pDepthMarketData.AveragePrice <<",";
        _file2 << pDepthMarketData.ActionDay <<endl;

        _file.close();
        _file2.close();
    }
}

void writeDataHalfDay(){
/*
tm_sec	int	seconds after the minute	0-61*
tm_min	int	minutes after the hour	0-59
tm_hour	int	hours since midnight	0-23
tm_mday	int	day of the month	1-31
tm_mon	int	months since January	0-11
tm_year	int	years since 1900
tm_wday	int	days since Sunday	0-6
tm_yday	int	days since January 1	0-365
tm_isdst	int	Daylight Saving Time flag
*/
    int uniqueSec=100;//用来判断，执行的基本单元为1秒
    while(true){
         time_t timer;
         time(&timer);
         tm* t_tm = localtime(&timer);

         if(t_tm->tm_hour == 12 || t_tm->tm_hour == 16){
            if(t_tm->tm_min == 0){
                if(uniqueSec != t_tm->tm_sec){
                    if(t_tm->tm_sec == 30){ //在12:00:30和4:00:30秒的时候写数据，之后清空dataList
                        saveDataToFile(m_dataList);
                        m_dataList.clear();
                        cout << "数据存储完毕，全部清空。 " << m_dataList.size() << endl;
                    }
                    uniqueSec = t_tm->tm_sec;
                    //cout << t_tm->tm_hour<<":"<< t_tm->tm_min<<":"<<t_tm->tm_sec<<"   " <<m_dataList.size()  << endl;
                }
            }
         }
    }
}

int main()
{
    cout << "Hello world1!" << endl;

    thread t1(testTraderConnect);
    thread t2(writeDataHalfDay);
    t1.join();
    t2.join();
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

    vector<string> testArray;
    testArray.push_back("IF1608");
    testArray.push_back("IH1608");
    testArray.push_back("IC1608");
    char * testCharArray[3];
    for(int i=0;i<3;i++){
        testCharArray[i]  = new char[testArray[i].size()+1];
        strcpy(testCharArray[i],testArray[i].c_str());
        cout << testCharArray[i] << endl;
    }
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
