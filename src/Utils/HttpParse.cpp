#include  "HttpParse.h"


#define HTTP_PARSE_TOKEN_CR     '\r'
#define HTTP_PARSE_TOKEN_LF     '\n'
#define HTTP_PARSE_TOKEN_BLANK  ' '
#define HTTP_MethodName_GET     "GET"
#define HTTP_MethodName_POST    "POST"
#define HTTP_MethodName_HEAD    "HEAD"
#define HTTP_HeaderName_Host    "Host:"



int http_parse_field_line(char *pHttpPkt,
                                          uint32_t          dwPktLen,
                                          HTTP_FIELD_NAME ucFieldFlag,
                                          char          **ppFieldValue,
                                          uint32_t         *pdwValueLen);
int http_parse_get_field_len(char *pReqLine,uint32_t dwReqLen,uint32_t *pDwValLen);

HTTP_PARSE_RET  http_get_field_value(char *pHttpPkt,
                                          uint32_t          dwPktLen,
                                          HTTP_FIELD_NAME ucFieldFlag,
                                          char          **ppFieldValue,
                                          uint32_t         *pdwValueLen)
{
    char    *pCurVal         = pHttpPkt;
    char    *pPreVal         = pHttpPkt;
    char    *pTempVal        = pHttpPkt;
    uint32_t   dwTempPktLen    = dwPktLen;
    uint32_t   tempLen         = 0;
    int   dwRet           = 0;
    
    if((NULL == pHttpPkt)    || 
       (0    == dwPktLen)    || 
       (NULL == ppFieldValue)||
       (NULL == pdwValueLen) ||
       (ucFieldFlag >= HTTP_PARSE_FIELD_MAX))
    {
        ACE_ASSERT(0);
        return HTTP_PARSE_PARAM_ERROR;
    }
    while(dwTempPktLen > 0)
    {
        dwTempPktLen--;
        switch(*pCurVal)
        {
            case HTTP_PARSE_TOKEN_CR:
                pPreVal = pCurVal;
                pCurVal++;
                tempLen++;
                break;

            case HTTP_PARSE_TOKEN_LF:
                if(*pPreVal == HTTP_PARSE_TOKEN_CR)
                {   
                    tempLen++;
                    if(HTTP_PARSE_FIELD_URL == ucFieldFlag)
                    {
                        dwRet = http_parse_field_line(pTempVal,
                                                      tempLen,
                                                      ucFieldFlag,
                                                      ppFieldValue,
                                                      pdwValueLen);
                        if(0 == dwRet)
                        {
                            return HTTP_PARSE_SUCCESS;
                        }
                        else
                        {               
                            return HTTP_PARSE_NOT_FOUND;
                        }
                    }
                    else if(HTTP_PARSE_FIELD_HOST == ucFieldFlag)
                    
                    {

                        dwRet = http_parse_field_line(pTempVal,
                                                      tempLen,
                                                      ucFieldFlag,
                                                      ppFieldValue,
                                                      pdwValueLen);
                        if(0 == dwRet)
                        {
                            return HTTP_PARSE_SUCCESS;
                        }
                                                      
                    }
                    else if(HTTP_PARSE_FIELD_REQTYPE == ucFieldFlag)
                    {
                        dwRet = http_parse_field_line(pTempVal,
                                                      tempLen,
                                                      ucFieldFlag,
                                                      ppFieldValue,
                                                      pdwValueLen);
                        if(0 == dwRet)
                        {
                            return HTTP_PARSE_SUCCESS;
                        }
                                    
                    }                    
                    
                }
          
                pPreVal = pCurVal;
                pCurVal++;
                pTempVal = pCurVal;
                tempLen = 0;
                break;
             default:
                {
                    pPreVal = pCurVal;
                    pCurVal++;
                    tempLen++;
                    break;
                }
       
        }
    }      
    return HTTP_PARSE_NOT_FOUND;
}

int http_parse_field_line(char           *pReqLine,
                                  uint32_t          dwReqLen,
                                  HTTP_FIELD_NAME ucFieldFlag,
                                  char          **ppFieldValue,
                                  uint32_t         *pdwValueLen)
{
    char    *pCurVal  = pReqLine;
    uint32_t   dwTemp   = dwReqLen;
    uint32_t   dwValLen = 0;
    if((NULL ==  pReqLine)    ||
       (0    == dwReqLen)     ||
       (NULL == ppFieldValue) ||
       (NULL == pdwValueLen))
    {
        return -1;
    }

    switch(ucFieldFlag)
    {
        case HTTP_PARSE_FIELD_URL:
            if(0 == strncmp(pReqLine,HTTP_MethodName_GET,
                            strlen(HTTP_MethodName_GET)))
            {
                if(dwTemp <= (strlen(HTTP_MethodName_GET)+1))
                {
                    return -1;     
                }
                pCurVal += strlen(HTTP_MethodName_GET)+1;  /*包括一个空格*/
                dwTemp  -= strlen(HTTP_MethodName_GET)+1;

                if(0 != http_parse_get_field_len(pCurVal,
                                                             dwTemp,
                                                             &dwValLen))

                {
                    return -1;    
                }

                *ppFieldValue = pCurVal;
                *pdwValueLen  = dwValLen; 

                return 0; 
            }
            else if(0 == strncmp(pReqLine,HTTP_MethodName_POST,
                            strlen(HTTP_MethodName_POST)))
            {
               if(dwTemp <= (strlen(HTTP_MethodName_POST)+1))
                {
                    return -1;     
                }
                pCurVal += strlen(HTTP_MethodName_POST)+1;  /*包括一个空格*/
                dwTemp  -= strlen(HTTP_MethodName_POST)+1;

                if(0 != http_parse_get_field_len(pCurVal,
                                                             dwTemp,
                                                             &dwValLen))

                {
                    return -1;    
                }

                *ppFieldValue = pCurVal;
                *pdwValueLen  = dwValLen; 

                return 0; 
            }
            break;
        case HTTP_PARSE_FIELD_HOST:
            if(0 == strncmp(pReqLine,
                            HTTP_HeaderName_Host, 
                            strlen(HTTP_HeaderName_Host)))
            {
                if(dwTemp <= (strlen(HTTP_HeaderName_Host)+1))
                {
                    return -1;     
                }
                pCurVal += strlen(HTTP_HeaderName_Host)+1;  /*包括一个空格*/
                dwTemp  -= strlen(HTTP_HeaderName_Host)+1;

                if(0 != http_parse_get_field_len(pCurVal,dwTemp,&dwValLen))
                {
                    return -1;    
                }

                *ppFieldValue = pCurVal;
                *pdwValueLen  = dwValLen; 

                return 0; 
            }
            break;
        case HTTP_PARSE_FIELD_REQTYPE:
         if((0 == strncmp(pReqLine,HTTP_MethodName_HEAD, strlen(HTTP_MethodName_HEAD)))||
             (0 == strncmp(pReqLine,HTTP_MethodName_POST, strlen(HTTP_MethodName_POST)))||
             (0 == strncmp(pReqLine,HTTP_MethodName_GET, strlen(HTTP_MethodName_GET))))
            {
                if(dwTemp <= (strlen(HTTP_MethodName_GET)+1))
                {
                    return -1;     
                }

                if(0 != http_parse_get_field_len(pCurVal,dwTemp,&dwValLen))

                {
                    return -1;    
                }

                *ppFieldValue = pCurVal;
                *pdwValueLen  = dwValLen; 

                return 0; 
            }
            break;
        default:
            break;
    }

    return -1;

}

int http_parse_get_field_len(char *pReqLine,uint32_t dwReqLen,uint32_t *pDwValLen)
{
    uint32_t  dwValLen = 0;
    char   *pCurVal  = pReqLine; 
    if((NULL == pReqLine) || (0 >= dwReqLen) || (NULL == pDwValLen))
    {
        return -1;
    }

    while(dwReqLen-- > 0)
    {
        if((HTTP_PARSE_TOKEN_BLANK == *pCurVal) || 
           (HTTP_PARSE_TOKEN_CR    == *pCurVal) ||
           (HTTP_PARSE_TOKEN_LF    == *pCurVal))
        {
            *pDwValLen = dwValLen;
            return 0;
        }

        pCurVal ++;
        dwValLen ++;   
    }

    return -1;

}

