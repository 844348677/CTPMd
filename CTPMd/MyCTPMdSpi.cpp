#include "api/ctp/ThostFtdcMdApi.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MyCTPMdSpi.h"
#include <vector>
#include <fstream>
#include <iomanip>
#include <thread>
#include <unistd.h>
#define DBL_MAX   1.7976931348623158e+308

using namespace std;

vector<CThostFtdcDepthMarketDataField> dataList;
extern vector<string> codeList;

MyCTPMdSpi::MyCTPMdSpi(CThostFtdcMdApi *pUserApi):m_pUserApi(pUserApi){}
//MyCTPMdSpi::MyCTPMdSpi(CThostFtdcMdApi *pUserApi,vector<string> codeList):m_pUserApi(pUserApi),m_codeList(codeList){}
MyCTPMdSpi::~MyCTPMdSpi(){}

//用户登陆 需要输入 brokerid userid pw 由外界的函数调用触发。
void MyCTPMdSpi::ReqUserLogin(char *broker_id, char *user_id, char *p_w){
    cout << endl;
    cout << "MD ###ReqUserLogin###: "<< endl;
    CThostFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin,0,sizeof(reqUserLogin));
    strcpy(reqUserLogin.BrokerID,broker_id);
    strcpy(reqUserLogin.UserID,user_id);
    strcpy(reqUserLogin.Password,p_w);
    //sleep(10); //unistd.h
    int ret = m_pUserApi->ReqUserLogin(&reqUserLogin,0);
    cout << endl;
}

void MyCTPMdSpi::OnFrontConnected(){
    cout << endl;
    cout << "MD ###OnFrontConnected###: " << endl;
    cout << endl;
}

void MyCTPMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    //登陆的回调函数，打印一些没有用的信息
    cout << "MD ###OnRspUserLogin###:" << endl;
    cout << "ErrorCode=" << pRspInfo->ErrorID  << " , RequestID=" << nRequestID << " , Chain=" << bIsLast  << endl;  //<< " ,ErrorMsg= " <<pRspInfo->ErrorMsg << endl;
    cout << "FrontID=" << pRspUserLogin->FrontID << " , SessionID=" << pRspUserLogin->SessionID << " , MaxOrderRef="<< pRspUserLogin->MaxOrderRef << " , TradingDay="<< m_pUserApi->GetTradingDay() << endl;
    if(pRspInfo->ErrorID != 0){ //登陆错误的时候打印信息。
        cout << "Failed to login,errorcode= " << pRspInfo->ErrorID << " ,errormsg= " << pRspInfo->ErrorMsg << " requestid " << nRequestID
        << " ,chain= " << bIsLast << endl;
    }
    cout << endl;
}

void MyCTPMdSpi::dataRecive(){
    cout << endl;
    cout << "MD ###dataRecive###: " << endl;
    char * Instrument[codeList.size()];
    for(int i=0;i<codeList.size();i++){
        Instrument[i] = new char[codeList.size()+1];
        strcpy(Instrument[i],codeList[i].c_str());
        cout <<"SubscribeMarketData: "<< Instrument[i] << endl;
    }
    m_codeList = codeList;
    m_pUserApi->SubscribeMarketData(Instrument,codeList.size());  //订阅行情，每一个合约调用一次OnRspSubMarketData，之后的数据在OnRtnDepthMarketData方法中返回
    cout << endl;
}
void MyCTPMdSpi::dataStop(){
    cout << endl;
    cout << "MD ###dataStop###: " << endl;
    char * Instrument[m_codeList.size()];
    for(int i=0;i<m_codeList.size();i++){
        Instrument[i] = new char[m_codeList.size()+1];
        strcpy(Instrument[i],m_codeList[i].c_str());
        cout << "UnSubscribeMarketData: " << Instrument[i] << endl;
    }
    m_pUserApi->UnSubscribeMarketData(Instrument,m_codeList.size());
    cout << endl;
}

void MyCTPMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    cout << endl;
    cout << "MD ###OnRspSubMarketData###: " << endl;
    cout << endl;
}
void MyCTPMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        cout << endl;
    cout << "MD ###OnRspUnSubMarketData###: " << endl;
    cout << endl;
};

void MyCTPMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData){
/*
    cout << "MD ###OnRtnDepthMarketData###: " << endl;

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
*/
    //全部写入到内存中，之后写到文件中
    dataList.push_back(*pDepthMarketData);
}
