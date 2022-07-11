//��ͻ��˽���Э���ͷ
typedef struct tagNtPkgHead
{
    unsigned char   bStartFlag;     //Э�����ʼ��־ 0xFF
    unsigned char   bVer;           //�汾��
    unsigned char   bEncryptFlag;   //���ܱ�־(���������,��Ϊ0)
    unsigned char   bFrag;          //�Ƿ��а���Ƭ(1 �а���Ƭ 0 �ް���Ƭ)
    unsigned short  wLen;           //�ܰ���
    unsigned short  wCmd;           //�����
    unsigned short  wSeq;           //�������к�,ҵ��ʹ��
    unsigned short  wCrc;           //Crc16У����
    unsigned int    dwSID;          //�ỰID
    unsigned short  wTotal;         //�а���Ƭʱ����Ƭ����
    unsigned short  wCurSeq;        //�а���Ƭʱ����Ƭ��ţ���0��ʼ���޷�ƬʱҲΪ0
} NtPkgHead, *PNtPkgHead;

UINT CSocketClient::RecvDataThreadProc(LPVOID lpParam)
{
    LOG_NORMAL("Start recv data thread.");
    DWORD           dwWaitResult;
    std::string     strPkg;
    //��ʱ���һ�������İ����ݵı���
    std::string     strTotalPkg;
    unsigned short  uPkgLen = 0;
    unsigned int    uBodyLen = 0;
    unsigned int    uTotalPkgLen = 0;
    unsigned int    uCmd = 0;
    NtPkgHead       pkgHead;
    unsigned short  uTotal = 0;
    //��¼��һ�εİ���Ƭ��ţ�����Ƭ��Ŵ�0��ʼ
    unsigned short  uCurSeq = 0;
    int             nWaitTimeout = 1;

    CSocketClient* pSocketClient = (CSocketClient*)lpParam;

    while (!m_bExit)
    {      
        //����Ƿ�������
        if (!pSocketClient->CheckReceivedData())
        {
            //����10����
            Sleep(10);
            continue;
        }
            
        //�������ݣ�������pSocketClient->m_strRecvBuf��
        if (!pSocketClient->Recv())
        {
            LOG_ERROR("Recv data error");
                
            //�����ݳ�����ս��ջ�������������һЩ�ر����ӡ������ȶ�����
            pSocketClient->m_strRecvBuf.clear();

            Reconnect();
            continue;
        }

        //һ��Ҫ����һ��ѭ������������Ϊ����һƬ�������ж������
        while (true)
        {
            //�жϵ�ǰ�յ��������Ƿ�һ����ͷ��С
            if (pSocketClient->m_strRecvBuf.length() < sizeof(NtPkgHead))
                break;

            memset(&pkgHead, 0, sizeof(pkgHead));
            memcpy_s(&pkgHead, sizeof(pkgHead), pSocketClient->m_strRecvBuf.c_str(), sizeof(pkgHead));
			
			//�԰���Ϣͷ����
            if (!CheckPkgHead(&pkgHead))
            {
                //�����ͷ���鲻ͨ��������������������Ѿ����������ˣ�ֱ����յ���
                //������һЩ�ر����Ӳ������Ķ���             
                LOG_ERROR("Check package head error, discard data %d bytes", (int)pSocketClient->m_strRecvBuf.length());
                
                pSocketClient->m_strRecvBuf.clear();

                Reconnect();
                break;
            }

            //�жϵ�ǰ�����Ƿ�һ�������Ĵ�С
            uPkgLen = ntohs(pkgHead.wLen);
            if (pSocketClient->m_strRecvBuf.length() < uPkgLen)
                break;

            strPkg.clear();
            strPkg.append(pSocketClient->m_strRecvBuf.c_str(), uPkgLen);

            //����ȡ���������Ƴ��Ѿ���������ݲ���
            pSocketClient->m_strRecvBuf.erase(0, uPkgLen);
       
            uTotal = ::ntohs(pkgHead.wTotal);
            uCurSeq = ::ntohs(pkgHead.wCurSeq);
            //�޷�Ƭ���һ����Ƭ
            if (uCurSeq == 0)
            {
                strTotalPkg.clear();
                uTotalPkgLen = 0;
            }

            uBodyLen = uPkgLen - sizeof(NtPkgHead);
            uTotalPkgLen += uBodyLen;
            strTotalPkg.append(strPkg.data() + sizeof(NtPkgHead), uBodyLen);

            //�޷ְ� �� �ְ������һ���� ����װ��İ����ͳ�ȥ
            if (uTotal == 0 || (uTotal != 0 && uTotal == uCurSeq + 1))
            {
                uCmd = ::ntohs(pkgHead.wCmd);

                //ProxyPackage�ǽ���������ҵ�������
                ProxyPackage proxyPackage;
                //����ҵ���
                proxyPackage.nCmd = uCmd;
                //����������
                proxyPackage.nLength = uTotalPkgLen;
                //������������
                proxyPackage.pszJson = new char[uTotalPkgLen];
                memset(proxyPackage.pszJson, 0, uTotalPkgLen * sizeof(char));
                memcpy_s(proxyPackage.pszJson, uTotalPkgLen, strTotalPkg.c_str(), strTotalPkg.length());

                //��һ�������İ�����ҵ����
                pSocketClient->m_pNetProxy->AddPackage((const char*)&proxyPackage, sizeof(proxyPackage));
            }
        }// end inner-while-loop


    }// end outer-while-loop


    LOG_NORMAL("Exit recv data thread.");

    return 0;
}