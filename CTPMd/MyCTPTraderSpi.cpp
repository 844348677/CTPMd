#include <iostream>
#include <string.h>
#include <stdio.h>
#include "MyCTPTraderSpi.h"
#include "api/ctp/ThostFtdcTraderApi.h"
#include "api/ctp/ThostFtdcMdApi.h"
#include "MyCTPMdSpi.h"
#include<vector>

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
    cout << "TradingDay:" << m_pUserApi->GetTradingDay() << endl;

    CThostFtdcQryInstrumentField pQryInstrument;
    memset(&pQryInstrument,0,sizeof(pQryInstrument));
    strcpy(pQryInstrument.ExchangeID,"CFFEX");
    int ret = m_pUserApi->ReqQryInstrument(&pQryInstrument,0);

    cout << "ret: " << ret <<endl;
}

//int indexN = 0;
vector<string> codeList;
void MyCTPTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    cout << "OnRspQryInstrument:" << endl;
    cout << "InstrumentID: " << pInstrument->InstrumentID <<endl;
    cout << "ExchangeID: " << pInstrument->ExchangeID << endl;
    //codeArray[index] =  pInstrument->InstrumentID;
    //++indexN;
    codeList.push_back(pInstrument->InstrumentID);
    //cout << codeList[codeList.size()-1]<< endl;
    //char * instrument = pInstrument->InstrumentID;
    //cout << "Instrument:" << instrument << endl;
    cout << endl;

    //以下东西需要提取出去
    if(codeList.size() == 18){
        CThostFtdcMdApi* pUserApiMd = CThostFtdcMdApi::CreateFtdcMdApi("./outfile/",true);
        MyCTPMdSpi sh(pUserApiMd,codeList);
        pUserApiMd->RegisterSpi(&sh);

        pUserApiMd->RegisterFront("tcp://180.166.11.33:41213");
        //pUserApiMd->RegisterFront("tcp://180.168.146.187:10010");
        pUserApiMd->Init();
        pUserApiMd->Join();
    }


}



