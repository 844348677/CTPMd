#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MyCTPMdSpi.h"
#include "api/ctp/ThostFtdcMdApi.h"

using namespace std;

MyCTPMdSpi::MyCTPMdSpi(CThostFtdcMdApi *pUserApi):m_pUserApi(pUserApi){}
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


        char * Instrumnet[] = {"IF1608","IF1609"};
        m_pUserApi->SubscribeMarketData(Instrumnet,2);

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
    cout <<"Turnover成交金额:"<<pDepthMarketData->Turnover<<endl;
    cout <<"UpdateTime最后修改时间:"<<pDepthMarketData->UpdateTime<<endl;
    cout <<"UpdateMillisec最后修改毫秒:"<<pDepthMarketData->UpdateMillisec<<endl;
    cout <<"BidPrice申买价一:"<<pDepthMarketData->BidPrice1<<endl;
    cout <<"BidVolume1申买量一:"<<pDepthMarketData->BidVolume1<<endl;
    cout <<"AskPrice1申卖价一:"<<pDepthMarketData->AskPrice1<<endl;
    cout <<"AskVolume1申卖量一:"<<pDepthMarketData->AskVolume1<<endl;
    cout << endl;
}
