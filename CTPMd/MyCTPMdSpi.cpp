#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MyCTPMdSpi.h"
#include "api/ctp/ThostFtdcMdApi.h"
#include <vector>
#include <fstream>
#include <iomanip>
#define DBL_MAX   1.7976931348623158e+308

using namespace std;

MyCTPMdSpi::MyCTPMdSpi(CThostFtdcMdApi *pUserApi):m_pUserApi(pUserApi){}
MyCTPMdSpi::MyCTPMdSpi(CThostFtdcMdApi *pUserApi,vector<string> codeList):m_pUserApi(pUserApi),m_codeList(codeList){}
MyCTPMdSpi::~MyCTPMdSpi(){}

void MyCTPMdSpi::OnFrontConnected(){
    cout << "OnFrontConnected:" << endl;

    //请求登陆
    CThostFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin,0,sizeof(reqUserLogin));
    strcpy(reqUserLogin.BrokerID,"9999");
    strcpy(reqUserLogin.UserID,"065801");
    strcpy(reqUserLogin.Password,"123456");
    int ret = m_pUserApi->ReqUserLogin(&reqUserLogin,0);

}

void MyCTPMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    cout << "OnRspUserLogin:"<< endl;
    cout << "ErrorCode= " << pRspInfo->ErrorID << " ,ErrorMsg= " << pRspInfo->ErrorMsg << endl;
    cout << "RequestID= " <<nRequestID << " ,Chain= " <<bIsLast << endl;
    if(pRspInfo->ErrorID != 0){
        cout << "Faileed to login,errorcode= " << pRspInfo->ErrorID << " ,errormsg= " <<pRspInfo->ErrorMsg << " requestid= " << nRequestID
        << " ,chian= " << bIsLast << endl;
    }
    cout << "Login success.Begin receiving data" << endl;

    for(int i=0;i<m_codeList.size();i++){
        cout << m_codeList[i] << endl;
    }
    char * Instrument[18];
    for(int i=0;i<18;i++){
        Instrument[i] = new char[m_codeList.size()+1];
        strcpy(Instrument[i],m_codeList[i].c_str());
        cout << Instrument[i] << endl;
    }
        //char * Instrumnet[1];
        //Instrumnet[0] = m_instrument;
    m_pUserApi->SubscribeMarketData(Instrument,18);

}

void MyCTPMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    cout << "OnRspSubMarketData:" << endl;

}

void MyCTPMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData){
    cout << "OnRtnDepthMarketData:" << endl;

    cout <<"TradingDay交易日:"<< pDepthMarketData->TradingDay<< endl;
    cout <<"InstrumentID合约代码:"<<pDepthMarketData->InstrumentID<<endl;
    cout <<"ExchangeID交易所代码:"<<pDepthMarketData->ExchangeID<<endl;
    cout <<"ExchangeInstID合约在交易所的代码:"<<pDepthMarketData->ExchangeInstID<<endl;
    cout <<"LastPrice最新价:"<<pDepthMarketData->LastPrice<<endl;
    cout <<"PreSettlementPrice上次结算价:"<<pDepthMarketData->PreSettlementPrice<<endl;
    cout <<"Volume数量:"<<pDepthMarketData->Volume<<endl;
    cout <<"Turnover成交金额:"<<setprecision(10)<<pDepthMarketData->Turnover<<endl;
    cout <<"UpdateTime最后修改时间:"<<pDepthMarketData->UpdateTime<<endl;
    cout <<"UpdateMillisec最后修改毫秒:"<<pDepthMarketData->UpdateMillisec<<endl;
    cout <<"BidPrice申买价一:"<<pDepthMarketData->BidPrice1<<endl;
    cout <<"BidVolume1申买量一:"<<pDepthMarketData->BidVolume1<<endl;
    cout <<"AskPrice1申卖价一:"<<pDepthMarketData->AskPrice1<<endl;
    cout <<"AskVolume1申卖量一:"<<pDepthMarketData->AskVolume1<<endl;
    cout << "double_max: " <<pDepthMarketData->BidPrice3 << endl;
    if(pDepthMarketData->BidPrice3 ==  DBL_MAX)
        cout << "aaaaa " << endl;
    cout << endl;

    //把结果写到文件当中
    char filename[80] =  "./";
    strcat(filename,pDepthMarketData->TradingDay);
    strcat(filename,"/");
    strcat(filename,pDepthMarketData->InstrumentID);
    strcat(filename,".txt");

    cout <<"  " << filename << endl;
    //string filename = "./"+pDepthMarketData->TradingDay+"/"+pDepthMarketData->InstrumentID+".txt";
    ofstream _file;
    _file.open(filename,ios::app);
    _file << pDepthMarketData->TradingDay <<",";
    _file << pDepthMarketData->InstrumentID <<",";
    _file << pDepthMarketData->ExchangeID <<",";
    _file << pDepthMarketData->ExchangeInstID <<",";
    _file << pDepthMarketData->LastPrice <<",";
    _file << pDepthMarketData->PreSettlementPrice <<",";
    _file << pDepthMarketData->PreClosePrice <<",";
    _file << pDepthMarketData->PreOpenInterest <<",";
    _file << pDepthMarketData->OpenPrice <<",";
    _file << pDepthMarketData->HighestPrice <<",";
    _file << pDepthMarketData->LowestPrice <<",";
    _file << pDepthMarketData->Volume <<",";
    _file <<setprecision(15)<< pDepthMarketData->Turnover <<",";
    _file << pDepthMarketData->OpenInterest <<",";
    if(pDepthMarketData->ClosePrice == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->ClosePrice <<",";
    if(pDepthMarketData->SettlementPrice == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->SettlementPrice <<",";
    _file << pDepthMarketData->UpperLimitPrice <<",";
    _file << pDepthMarketData->LowerLimitPrice <<",";
    if(pDepthMarketData->PreDelta == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->PreDelta <<",";
    if(pDepthMarketData->CurrDelta == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->CurrDelta <<",";
    _file << pDepthMarketData->UpdateTime <<",";
    _file << pDepthMarketData->UpdateMillisec <<",";
    if(pDepthMarketData->BidPrice1 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->BidPrice1 <<",";
    _file << pDepthMarketData->BidVolume1 <<",";
    if(pDepthMarketData->AskPrice1 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->AskPrice1 <<",";
    _file << pDepthMarketData->AskVolume1 <<",";
    if(pDepthMarketData->BidPrice2 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->BidPrice2 <<",";
    _file << pDepthMarketData->BidVolume2 <<",";
    if(pDepthMarketData->AskPrice2 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->AskPrice2 <<",";
    _file << pDepthMarketData->AskVolume2 <<",";
    if(pDepthMarketData->BidPrice3 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->BidPrice3 <<",";
    _file << pDepthMarketData->BidVolume3 <<",";
    if(pDepthMarketData->AskPrice3 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->AskPrice3 <<",";
    _file << pDepthMarketData->AskVolume3 <<",";
    if(pDepthMarketData->BidPrice4 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->BidPrice4 <<",";
    _file << pDepthMarketData->BidVolume4 <<",";
    if(pDepthMarketData->AskPrice4 == DBL_MAX)
        _file << ",";
     else
        _file << pDepthMarketData->AskPrice4 <<",";
    _file << pDepthMarketData->AskVolume4 <<",";
    if(pDepthMarketData->BidPrice5 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->BidPrice5 <<",";
    _file << pDepthMarketData->BidVolume5 <<",";
    if(pDepthMarketData->AskPrice5 == DBL_MAX)
        _file << ",";
    else
        _file << pDepthMarketData->AskPrice5 <<",";
    _file << pDepthMarketData->AskVolume5 <<",";
    _file << pDepthMarketData->AveragePrice <<",";
    _file << pDepthMarketData->ActionDay <<endl;

    _file.close();

}
