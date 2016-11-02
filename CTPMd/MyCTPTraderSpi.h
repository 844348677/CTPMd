#pragma once
#ifndef MYCTPTRADERSPI_H_INCLUDED
#define MYCTPTRADERSPI_H_INCLUDED
#include "api/ctp/ThostFtdcTraderApi.h"
class MyCTPTraderSpi : public CThostFtdcTraderSpi
{
    public:

        MyCTPTraderSpi(CThostFtdcTraderApi *pUserApi);
        ~MyCTPTraderSpi();

        //用户登陆 需要输入 brokerid userid pw
        void ReqUserLogin(char *broker_id, char *user_id, char *p_w);
        //登陆成功之后，需要调用查询合约列表
        void qryCode(char * exchangeID);
        //查询交易所
        void qryExchange();


        ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
        virtual void OnFrontConnected();
        ///登录请求响应
        virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        ///请求查询合约响应
        virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
        ///请求查询交易所响应
        virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    //private:
        CThostFtdcTraderApi * m_pUserApi;
};

#endif // MYCTPTRADERSPI_H_INCLUDED
