#ifndef _CADDSESSIONID_H_
#define _CADDSESSIONID_H_
#include "aceinclude.h"
#include <iostream>
#include <list>

typedef std::list<uint16_t> LISTINT; 

class CAddSessionId {
public:
    CAddSessionId();
    ~CAddSessionId();
    void InIt(uint16_t startid,uint16_t endid);
    uint16_t GetId();
    void FreeId(uint16_t id);
    LISTINT GetSessionId();
    uint16_t Getstartid();
    uint16_t Getendid();
private:
    LISTINT m_SessionId;
    uint16_t m_startid;
    uint16_t m_endid;
};

#endif //_CADDSESSIONID_H_


























