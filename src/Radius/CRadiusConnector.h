/***********************************************************************
 * Copyright (c) 2017 The OpenBras project authors. All Rights Reserved. 
**********************************************************************/

#ifndef CRADIUSCONNECTOR_H
#define CRADIUSCONNECTOR_H

#include "aceinclude.h"
#include <stdint.h>
#include <unordered_map>
#include "openportal.h"
#include "CReferenceControl.h"
//#include "CRadiusScheme.h"
//class CPortalClient;
class CRadiusMessage;
class CRadiusScheme;
class TransactionResponse;
class CRadiusTransaction;
class CRadiusConnector : public CReferenceControl,public ACE_Event_Handler
{
public:
    CRadiusConnector(CRadiusScheme* scheme);
    virtual ~CRadiusConnector();
    virtual int StartConnect(const ACE_INET_Addr &peeraddr);
    virtual int StopConnect();

    virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);

    //ACE_Event_Handler interface，子类可以重载这些函数
    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual ACE_HANDLE get_handle (void) const;
    virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
    int SendData(const char *data, size_t datasize);

    std::string GetSharedKey();
    int GetTimeOut();
    int GetRetrans();
    int FindTransaction(uint8_t id, CCmAutoPtr<CRadiusTransaction> &trans);
    int SendMessage(CRadiusMessage &accessReqMsg,TransactionResponse callback);
protected:
    ACE_HANDLE m_handler;
    CCmAutoPtr<CRadiusScheme> m_pscheme;
    ACE_INET_Addr m_peeraddr;
    ACE_Thread_Mutex m_mutex;
    std::unordered_map<uint8_t, CCmAutoPtr<CRadiusTransaction> > m_trans;
    int m_accessRequest;       // Client send
    int m_accessAccept;       // Server send
    int m_accessReject;       // Server send
    int m_accountingRequest;  // Client send
    int m_accountingResponse;  // Server send
    int m_nIdentifierBase;
};

#endif//CDHCPSERVER_H


