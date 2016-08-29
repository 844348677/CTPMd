#pragma once
#ifndef MYCTPMDSPI_H_INCLUDED
#define MYCTPMDSPI_H_INCLUDED
#include <vector>
#include <string>
#include "api/ctp/ThostFtdcMdApi.h"
using namespace std;


class MyCTPMdSpi : public CThostFtdcMdSpi
{
    public:

        MyCTPMdSpi(CThostFtdcMdApi *pUserApi);
        MyCTPMdSpi(CThostFtdcMdApi *pUserApi,vector<string> codeList);
        ~MyCTPMdSpi();

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
        virtual void OnFrontConnected();
	///登录请求响应
        virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    ///订阅行情应答
        virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    ///深度行情通知
        virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

        static void threadtest();
        static void threadtest2(CThostFtdcMdApi * pUserApi);

    private:
        CThostFtdcMdApi * m_pUserApi;
        vector<string> m_codeList;

};

#endif // MYCTPMDSPI_H_INCLUDED
