#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MyCTPTraderSpi.h"
#include "api/ctp/ThostFtdcTraderApi.h"

using namespace std;

MyCTPTraderSpi::MyCTPTraderSpi(CThostFtdcTraderApi *pUserApi):m_pUserApi(pUserApi){}
MyCTPTraderSpi::~MyCTPTraderSpi(){}

void MyCTPTraderSpi::OnFrontConnected(){
    cout << "OnFrontConnected:" << endl;

    CThostFtdcReqUserLoginField reqUserLogin;

    memset(&reqUserLogin,0,sizeof(reqUserLogin));
    strcpy(reqUserLogin.BrokerID,"9999");
    strcpy(reqUserLogin.UserID,"065801");
    strcpy(reqUserLogin.Password,"123456");

    int ret= m_pUserApi -> ReqUserLogin(&reqUserLogin,0);
}
void MyCTPTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    cout << "OnRspUserLogin:" << endl;
    cout << "ErrorCode= " << pRspInfo->ErrorID << " ,ErrorMsg= " <<pRspInfo->ErrorMsg << endl;
    cout << "RequestID= " << nRequestID << " ,Chain= " << bIsLast << endl;
    if(pRspInfo->ErrorID != 0){
        cout << "Failed to login,errorcode= " << pRspInfo->ErrorID << " ,errormsg= " << pRspInfo->ErrorMsg << " requestid " << nRequestID
        << " ,chain= " << bIsLast << endl;
    }
    cout << "FrontID: " << pRspUserLogin->FrontID << endl;
    cout << "SessionID: " << pRspUserLogin->SessionID << endl;
    cout << "MaxOrderRef: " << pRspUserLogin->MaxOrderRef << endl;


    CThostFtdcQryInstrumentField pQryInstrument;
    memset(&pQryInstrument,0,sizeof(pQryInstrument));
    strcpy(pQryInstrument.ExchangeID,"CFFEX");
    m_pUserApi->ReqQryInstrument(&pQryInstrument,0);

}

void MyCTPTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    cout << "OnRspQryInstrument:" << endl;
    cout << "InstrumentID: " << pInstrument->InstrumentID <<endl;
    cout << "ExchangeID: " << pInstrument->ExchangeID << endl;

    cout << endl;
}

