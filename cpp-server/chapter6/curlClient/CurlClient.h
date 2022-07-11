/** 
 * ��װcurl��HTTP�⣬CurlClient.h
 * zhangyl 2019.08.27
 */

#ifndef __CURL_CLIENT_H__
#define __CURL_CLIENT_H__

#include <string>

#include "curl.h"

class CCurlClient final
{
public:
    CCurlClient();
    ~CCurlClient();

    CCurlClient(const CCurlClient& rhs) = delete;
    CCurlClient& operator=(const CCurlClient& rhs) = delete;

    /**
     * ��ʼ��libcurl
     * ���̰߳�ȫ�����������ڳ����ʼ��ʱ����һ�Σ���������̵߳���curl_easy_init������
     */
    static void init();
    /**
     * ����ʼ��libcurl
     * ���̰߳�ȫ�����������ڳ����˳�ʱ����һ��
     */
    static void uninit();

    /** ����HTTP GET����
     * @param url �������ַ
     * @param headers �������͵��Զ���HTTPͷ��Ϣ������Զ���ͷ֮��ʹ��\r\n�ָ���һ����\r\n�����������Զ���HTTPͷ��Ϣ����������Ϊnullptr
     * @param response �������ɹ�����洢HTTP��������ע�⣺response�ں�����������׷��ģʽ��Ҳ����˵�����һ��response��ֵ����գ�������������ʱ��׷�ӣ������Ǹ��ǣ�
     * @param autoRedirect ����õ�HTTP 3xx��״̬���Ƿ��Զ��ض������µ�URL
     * @param reserveHeaders ����Ľ���У��洢��response�����Ƿ���HTTPͷ��Ϣ
     * @param connTimeout ���ӳ�ʱʱ�䣬��λΪ�루����ĳЩHTTP URI��Դ���õģ������Ƿ��س�ʱ������Խ��ò������õô�һ�㣩
     * @param readTimeout ��ȡ���ݳ�ʱʱ�䣬��λΪ�루����ĳЩHTTP URI��Դ���õģ������Ƿ��س�ʱ������Խ��ò������õô�һ�㣩
     */
    bool get(const char* url, const char* headers, std::string& response, bool autoRedirect = false,
             bool reserveHeaders = false, int64_t connTimeout = 1L, int64_t readTimeout = 5L);
    
    /** ����HTTP POST����
     * @param url �������ַ
     * @param headers �������͵��Զ���HTTPͷ��Ϣ������Զ���ͷ֮��ʹ��\r\n�ָ���һ����\r\n�����������Զ���HTTPͷ��Ϣ����������Ϊnullptr
     * @param postParams post��������
     * @param response �������ɹ�����洢HTTP����Ľ����ע�⣺response�ں�����������׷��ģʽ��Ҳ����˵�����һ��response��ֵ����գ�������������ʱ��׷�ӣ������Ǹ��ǣ�
     * @param autoRedirect ����õ�HTTP 3xx��״̬���Ƿ��Զ��ض������µ�URL
     * @param bReserveHeaders ����Ľ���У��洢��response���Ƿ���HTTPͷ��Ϣ
     * @param connTimeout ���ӳ�ʱʱ�䣬��λΪ�루����ĳЩHTTP URI��Դ���õģ������Ƿ��س�ʱ������Խ��ò������õô�һ�㣩
     * @param readTimeout ��ȡ���ݳ�ʱʱ�䣬��λΪ�루����ĳЩHTTP URI��Դ���õģ������Ƿ��س�ʱ������Խ��ò������õô�һ�㣩
     */
    bool post(const char* url, const char* headers, const char* postParams, std::string& response, bool autoRedirect = false, bool reserveHeaders = false, int64_t connTimeout = 1L, int64_t readTimeout = 5L);


private:
    static bool  m_bGlobalInit;
};

#endif //!__CURL_CLIENT_H__