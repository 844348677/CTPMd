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
#include <unistd.h>
#define DBL_MAX   1.7976931348623158e+308
#include <sys/stat.h> 　
#include <sys/types.h> 　
#include <dirent.h>
using namespace std;

//非测试  ！
char * front_ip_trader = "";
char * front_ip_md = "";
char * broker_id = "";
char * user_id = "";
char * p_w = "";

char * exchangeID = "SHFE"; //设置需要下载数据的市场，查询该市场的多有合约号。 CFFEX   SHFE   DCE   CZCE

extern vector<CThostFtdcDepthMarketDataField> dataList; //数据
extern vector<string> codeList; //合约

void searchCodelist();
void reciveData();
void writeDataHalfDay();
int IsFolderExist(const char* path);
void mkdir(char * path);
void saveDataToFile(vector<CThostFtdcDepthMarketDataField> & dataList);

int main()
{
    cout << "ctp recieve data and save data into file. " << endl;

    thread t1(searchCodelist);
    thread t2(writeDataHalfDay);
    thread t3(reciveData);
    t1.join();
    t2.join();
    t3.join();

    return 0;
}

void searchCodelist(){
    cout <<"(线程用来查询对应市场的所有合约号)searchCodelist!!!!!!!: " << endl;

    CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./outfile3/");
    MyCTPTraderSpi sh(pUserApi);
    pUserApi->RegisterSpi(&sh);

    pUserApi->RegisterFront(front_ip_trader);
    pUserApi->Init();
    sleep(2);
    sh.ReqUserLogin(broker_id,user_id,p_w);
    sleep(2);
    sh.qryCode(exchangeID);
    sleep(2);

}
void reciveData(){
    cout << "(线程用来订阅合约号接受数据)reciveData!!!!!!!: "<< endl;

    CThostFtdcMdApi* pUserApiMd = CThostFtdcMdApi::CreateFtdcMdApi("./outfile/",true);
    MyCTPMdSpi sh(pUserApiMd);
    pUserApiMd->RegisterSpi(&sh);

    pUserApiMd->RegisterFront(front_ip_md);

    pUserApiMd->Init();
    sleep(2);
    sh.ReqUserLogin(broker_id,user_id,p_w);
    sleep(20);
    sh.dataRecive();
    sleep(10);

    pUserApiMd->Join();
}
void writeDataHalfDay(){
    cout << "(线程用来把内存中的数据写到文件中)writeDataHalfDay!!!!!!!: " << endl;

    while(true){
        time_t timer;
        time(&timer);
        tm* t_tm = localtime(&timer);

        if(t_tm->tm_hour == 12 || t_tm->tm_hour == 16){
            if(t_tm->tm_min == 0){

                saveDataToFile(dataList);
                dataList.clear();
                cout << "数据存储完毕，全部清空。dataList.size(): " << dataList.size() << endl;
                cout << t_tm->tm_hour<<":"<< t_tm->tm_min<<":"<<t_tm->tm_sec<<"   " <<dataList.size()  << endl;
            }
         }
        sleep(1*60); //每隔一分钟执行一次查询时间操作，在12点和16点时候保存数据，不知道会不会跳过这个时间段，没有执行程序
    }
}
int IsFolderExist(const char* path){
    DIR *dp;
    if ((dp = opendir(path)) == NULL){
        return 0; //不存在该文件夹为0
    }
    closedir(dp);
    return -1; //存在该文件夹为1
}
void mkdir(char * path){
    //char * path = "./20161032/";
    if(IsFolderExist(path) == 0){   //不存在该文件夹则创建该文件夹
        int isCreate = mkdir(path,S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
        if( !isCreate )
        printf("create path: %s\n",path);
        else
        printf("create path failed! error code : %s \n",isCreate,path);
    }
}
//把数据存到文件中，存两份，一份为程序目录 日期.txt，另一份 ./日期/合约.txt
void saveDataToFile(vector<CThostFtdcDepthMarketDataField> & dataList){
    cout <<"dataList.size(): " << dataList.size()<< endl;
    if(dataList.size() != 0)
        mkdir(dataList[0].TradingDay);
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

