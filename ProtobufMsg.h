#pragma once

#include <Message.pb.h>
#include <memory>
#include <map>
#include "Client.h"

using std::shared_ptr;

class BaseHandler
{

public:
    BaseHandler(pbmsg::MSG type):type_(type){
        Register (this);
    }
    virtual ~BaseHandler(){}
    pbmsg::MSG GetType() const { return type_; }
    //���崦��������������ʵ��.
    virtual void Process( const shared_ptr<pbmsg::Message> & msg, const shared_ptr<Client> & client) = 0;

    //ע����Ϣ������
    static void Register( BaseHandler *);

    //ִ��ָ������Ϣ,��ѯ������������Process��
    static void Execute( const shared_ptr<pbmsg::Message> & msg, const shared_ptr<Client> & client);

private:
    pbmsg::MSG type_;

private:
    static std::map<pbmsg::MSG , BaseHandler *> handers;

};

// ÿ����Ϣ��ʵ��Process��һ���ػ��汾
template< pbmsg::MSG Type>
class MessageHandler : public BaseHandler
{
public:
MessageHandler(void):BaseHandler(Type){}
    ~MessageHandler(void){}

    void Process( const shared_ptr<pbmsg::Message> & msg, const shared_ptr<Client> & client);

private:
    static MessageHandler thisHandler;

};
