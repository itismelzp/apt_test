//
// Created by Ushop on 2022/6/21.
//
#include <type_traits>
#include <memory>
#include <string>

//α����
struct TcpConnection{
    void forceClose();
};
struct Buffer{
    int readableBytes(); //�ɶ�ȡ�ֽ���
    const char* peek();  //�����׵�ַ
    void retrieve(size_t size);
};
struct Timestamp;

//ǿ��1�ֽڶ���
#pragma pack(push, 1)
//Э��ͷ
struct msg_header {
    std::int32_t bodyszie; //�����С
};
#pragma pack(pop)

//��������ֽ�������Ϊ10MB
#define MAX_PACKAGE_SIZE 10 * 1024 * 1024

void OnRead(const std::shared_ptr<TcpConnection>& conn,Buffer* pBuffer,Timestamp receivTime){
    while(true){
        if(pBuffer->readableBytes() < (size_t) sizeof(msg_header)){
            //����һ����ͷ��С��ֱ���˳�
            return;
        }

        //ȡ��ͷ��Ϣ
        msg_header header;
        memcpy(&header,pBuffer->peek(),sizeof(msg_header));

        //��ͷ�д��������ر�����
        if(header.bodyszie <= 0 || header.bodyszie > MAX_PACKAGE_SIZE){
            //�ͻ��˷��ͷǷ����ݰ��������������ر���
            //LOGE()
            conn->forceClose();
            return;
        }

        //�յ����ݲ���һ�������İ�
        if (pBuffer-> readableBytes() < (size_t)header.bodyszie + sizeof(msg_header)) return;
        pBuffer->retrieve(sizeof(msg_header));
        //inbuf������ŵ�ǰҪ����İ�
        std::string inbuf;
        inbuf.append(pBuffer->peek(),header.bodyszie);
        pBuffer->retrieve(header.bodyszie);
        //�����ҵ���߼�
        if (!Process(conn,inbuf.c_str(),inbuf.length())){
            //�ͻ��˷��ͷǷ����ݰ��������������ر���
            //LOGE
            conn->forceClose();
            return;
        }
    }
}

void OnRead(const std::shared_ptr<TcpConnection>& conn,Buffer* pBuffer,Timestamp receivTime){
    while(true){
        if(pBuffer->readableBytes() < (size_t) sizeof(msg_header)){
            //����һ����ͷ��С��ֱ���˳�
            return;
        }

        //ȡ��ͷ��Ϣ
        msg_header header;
        memcpy(&header,pBuffer->peek(),sizeof(msg_header));

        //��ͷ�д��������ر�����
        if(header.bodyszie <= 0 || header.bodyszie > MAX_PACKAGE_SIZE){
            //�ͻ��˷��ͷǷ����ݰ��������������ر���
            //LOGE()
            conn->forceClose();
            return;
        }

        //�յ����ݲ���һ�������İ�
        if (pBuffer-> readableBytes() < (size_t)header.bodyszie + sizeof(msg_header)) return;
        pBuffer->retrieve(sizeof(msg_header));
        //inbuf������ŵ�ǰҪ����İ�
        std::string inbuf;
        inbuf.append(pBuffer->peek(),header.bodyszie);
        pBuffer->retrieve(header.bodyszie);
        //�����ҵ���߼�
        if (!Process(conn,inbuf.c_str(),inbuf.length())){
            //�ͻ��˷��ͷǷ����ݰ��������������ر���
            //LOGE
            conn->forceClose();
            return;
        }
    }
}

int main(){

}