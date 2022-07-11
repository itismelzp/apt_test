/**
 * ��װcurl��HTTP�⣬CurlClient.cpp
 * zhangyl 2019.08.27
 */

#include "CurlClient.h"
#include <iostream>
#include <string>

//��������Ӧʱ�Ļص�����
size_t reqReply(void* ptr, size_t size, size_t nmemb, void* stream)
{
    std::string* str = (std::string*)stream;
    (*str).append((char*)ptr, size * nmemb);
    return size * nmemb;
}

bool CCurlClient::m_bGlobalInit = false;

CCurlClient::CCurlClient()
{
}

CCurlClient::~CCurlClient()
{
}

void CCurlClient::init()
{
    if (!m_bGlobalInit)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        m_bGlobalInit = true;
    }
}

void CCurlClient::uninit()
{
    if (m_bGlobalInit)
    {
        curl_global_cleanup();
        m_bGlobalInit = false;
    }
}

//HTTP GET  
bool CCurlClient::get(const char* url, const char* headers, std::string& response,
                      bool autoRedirect/* = false*/, bool reserveHeaders/* = false*/, int64_t connTimeout/* = 1L*/, int64_t readTimeout/* = 5L*/)
{
    //��ʼ��CURL����
    CURL* curl = curl_easy_init();
    if (curl == nullptr)
        return false;

    bool isHttps = ((int)*(url+4) == (int )'s');
    //���������URL 
    curl_easy_setopt(curl, CURLOPT_URL, url);
    //��ʹ��HTTPS�������Ҫʹ��HTTPS�������һ����������Ϊtrue
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, isHttps);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, isHttps);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reqReply);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
    //����SIGALRM+sigsetjmp/siglongjmp�ĳ�ʱ����
    //����������ʱ���ƣ���Ϊ�û����޸���һ��ȫ�ֱ����������ڶ��߳��¿��ܳ�������
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connTimeout); 
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, readTimeout);

    //����HTTP 3xx״̬���Ƿ��Զ��ض�λ
    if (autoRedirect)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

    if (reserveHeaders)
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    //����Զ���HTTPͷ��Ϣ
    if (headers != nullptr)
    {
        struct curl_slist* chunk = NULL;
        chunk = curl_slist_append(chunk, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }

    //����HTTP����
    CURLcode res = curl_easy_perform(curl);

    //�ͷ�CURL����
    curl_easy_cleanup(curl);

    return res == CURLcode::CURLE_OK;
}

//http POST  
bool CCurlClient::post(const char* url, const char* headers, const char* postParams, std::string& response,
                       bool autoRedirect/* = false*/, bool reserveHeaders/* = false*/, int64_t connTimeout/* = 1L*/, int64_t readTimeout/* = 5L*/)
{
    //��ʼ��CURL����
    CURL* curl = curl_easy_init();
    if (curl == nullptr)
        return false;

    bool isHttps = ((int)*(url+4) == (int )'s');

    //��������ʽΪpost
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    //���������URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    //����POST����Ĳ���
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams);
    //������HTTPS
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, isHttps);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, isHttps);
    //������ػص����� 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reqReply);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
    //����SIGALRM+sigsetjmp/siglongjmp�ĳ�ʱ���ƣ�
    //����������ʱ���ƣ���Ϊ�û����޸���һ��ȫ�ֱ����������ڶ��߳��¿��ܻ��������
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connTimeout);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, readTimeout);

    //����HTTP 3xx״̬���Ƿ��Զ��ض�λ
    if (autoRedirect)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

    if (reserveHeaders)
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    //����Զ���ͷ��Ϣ
    if (headers != nullptr)
    {
        struct curl_slist* chunk = NULL;
        chunk = curl_slist_append(chunk, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }

    //����HTTP����
    CURLcode res = curl_easy_perform(curl);

    //�ͷ�CURL����
    curl_easy_cleanup(curl);

    return res == CURLcode::CURLE_OK;
}

int main(){
    CCurlClient::init();
    CCurlClient client;
    std::string response;
    if(!client.get("http://192.168.1.253:20103/sys/getTime", nullptr,response)){
//    if(!client.get("https://cg.zhipu-china.com/sys/getTime", nullptr,response)){
        std::cout << "fail" << std::endl;
    }
    std::cout << response << std::endl;
    CCurlClient::uninit();
}
