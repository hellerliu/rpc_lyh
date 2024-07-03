#pragma once

#include <RpcBase/BaseService.h>
#include "mytest/TestMessage.pb.h"

namespace mytest
{
    enum MyTestCallErrCode
    {
        ReturnTestError = 1
    };

    class IMyTestCall : public rpc::BaseService
    {
    public:
        IMyTestCall(bool isClient = true) : BaseService(isClient)
        {
            addFunction<&IMyTestCall::addOne>();
            addFunction<&IMyTestCall::protoMsg>();
            addFunction<&IMyTestCall::fiber_test>(true);
            addFunction<&IMyTestCall::fiber_thread>(true);
            addFunction<&IMyTestCall::returnError>();
            addFunction<&IMyTestCall::array_test>();
        }

        CLIENT_CALL(addOne, int, int);
        CLIENT_CALL(protoMsg, test::echoReq, test::echoResp);
        CLIENT_CALL(fiber_test, std::string, test::echoResp);
        CLIENT_CALL(fiber_thread, SubscribeStr, test::echoResp);
        CLIENT_CALL(returnError, rpc::VoidParam, rpc::VoidParam);

        virtual int array_test(const int (&req)[2], int (&resp)[2])
        {
            return clientCall(__FUNCTION__, req, resp);
        }
    };

}