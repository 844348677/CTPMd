#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MyCTPTraderSpi.h"
#include "api/ctp/ThostFtdcTraderApi.h"
#include "api/ctp/ThostFtdcMdApi.h"
#include "MyCTPMdSpi.h"
#include<vector>
#include <unistd.h>
using namespace std;

vector<string> codeList;

MyCTPTraderSpi::MyCTPTraderSpi(CThostFtdcTraderApi *pUserApi):m_pUserApi(pUserApi){}
MyCTPTraderSpi::~MyCTPTraderSpi(){}

//用户登陆 需要输入 brokerid userid pw 由外界的函数调用触发。
void MyCTPTraderSpi::ReqUserLogin(char *broker_id, char *user_id, char *p_w){
    cout << endl;
    cout << "Trader ###ReqUserLogin###: "<< endl;
    CThostFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin,0,sizeof(reqUserLogin));
    strcpy(reqUserLogin.BrokerID,broker_id);
    strcpy(reqUserLogin.UserID,user_id);
    strcpy(reqUserLogin.Password,p_w);
    m_pUserApi->ReqUserLogin(&reqUserLogin,0);
    cout << endl;
}

void MyCTPTraderSpi::OnFrontConnected(){
    //链接到前置机之后，该方法就被调用
    cout << endl;
    cout << "Trader ###OnFrontConnected###: " << endl;
    cout << endl;
}
void MyCTPTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    //登陆的回调函数，打印一些没有用的信息
    cout << "Trader ###OnRspUserLogin###:" << endl;
    cout << "ErrorCode=" << pRspInfo->ErrorID  << " , RequestID=" << nRequestID << " , Chain=" << bIsLast  << endl;  //<< " ,ErrorMsg= " <<pRspInfo->ErrorMsg << endl;
    cout << "FrontID=" << pRspUserLogin->FrontID << " , SessionID=" << pRspUserLogin->SessionID << " , MaxOrderRef="<< pRspUserLogin->MaxOrderRef << " , TradingDay="<< m_pUserApi->GetTradingDay() << endl;
    if(pRspInfo->ErrorID != 0){ //登陆错误的时候打印信息。
        cout << "Failed to login,errorcode= " << pRspInfo->ErrorID << " ,errormsg= " << pRspInfo->ErrorMsg << " requestid " << nRequestID
        << " ,chain= " << bIsLast << endl;
    }
    cout << endl;
}

void MyCTPTraderSpi::qryCode(char * exchangeID){
    cout << endl;
    cout << "Trader ###qryCode###: "<< endl;
    codeList.clear();
    CThostFtdcQryInstrumentField pQryInstrument;
    memset(&pQryInstrument,0,sizeof(pQryInstrument));
    strcpy(pQryInstrument.ExchangeID,exchangeID);
    //strcpy(pQryInstrument.ExchangeID,"CZCE");"CFFEX"
    int ret = m_pUserApi->ReqQryInstrument(&pQryInstrument,0);
    cout << endl;
}
void MyCTPTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    cout << endl;
    cout << "Trader ###OnRspQryInstrument###: " << endl;
    cout << "InstrumentID: " << pInstrument->InstrumentID << " , ExchangeID: " << pInstrument->ExchangeID << " , bIsLast: " << bIsLast << endl;
    codeList.push_back(pInstrument->InstrumentID);

    if(bIsLast == 1){
        cout << "end. codelist.size(): " << codeList.size() << endl;
    }
    cout << endl;
}

void MyCTPTraderSpi::qryExchange(){
    cout << endl;
    cout << "Trader ###qryExchange###: "<< endl;
    //	virtual int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) = 0;
    CThostFtdcQryExchangeField pQryExchange;
    memset(&pQryExchange,0,sizeof(pQryExchange));
    //strcpy(pQryExchange.ExchangeID,"CFFEX");
    int ret = m_pUserApi->ReqQryExchange(&pQryExchange,0);
    cout <<" test " << endl;
}
void MyCTPTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    cout << endl;
    cout << "Trader ###OnRspQryExchange###: " << endl;
    cout << "InstrumentID: " << pExchange->ExchangeID << " , ExchangeID: " << pExchange->ExchangeName << " , ExchangeProperty: " << pExchange->ExchangeProperty << " , bIsLast: " << bIsLast << endl;

    if(bIsLast == 1){
        cout << "end "  << endl;
    }
    cout << endl;
}

