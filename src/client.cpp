
#include "RpcCore/RpcManager.h"
#include "mytest/MySubscrible.h"
#include "mytest/IMyTestCall.h"

using namespace rpc;

void testDataRecv(const std::string &topic, mytest::SubscribeData &data, std::string &shm)
{
    printf("topic:%s %s %ld\n", topic.c_str(), shm.c_str(), data.test);

    ShmConfig state;
    if (shm.empty() == false && get_rpc_manager().getShmConfig(topic, state))
    {
        printf("shm:%d %d\n", state.maxLen, state.num);
    }
}

void testString(const std::string &topic, std::string &data, std::string &)
{
    printf("topic:%s %s\n", topic.c_str(), data.c_str());
}

void testJsonData(const std::string &topic, mytest::JsonData &data, std::string &)
{
    printf("topic:%s %s\n", topic.c_str(), data.msg.value().c_str());
}

mytest::IMyTestCall gCall;

int main()
{
    get_rpc_manager().initalize();

    get_rpc_manager().subscribe("strdata", &testString);
    get_rpc_manager().subscribe(&testDataRecv);
    get_rpc_manager().subscribe(&testDataRecv, true);
    get_rpc_manager().subscribe(&testJsonData);

    int seq = 0;
    while (true)
    {
        seq++;
        gCall.async(&mytest::IMyTestCall::fiber_test, std::string{"run in fiber"}, [](int res, test::echoResp &resp)
                    { printf("fiber_test async res:%d %d\n", res, resp.resp()); });

        gCall.async(&mytest::IMyTestCall::fiber_thread, mytest::SubscribeStr{10, "123456"}, [](int res, test::echoResp &resp)
                    { printf("fiber_thread async res:%d %d\n", res, resp.resp()); });

        gCall.async(&mytest::IMyTestCall::addOne, seq, [](int res, int &resp)
                    { printf("addOne async res:%d %d\n", res, resp); });
        int resp = 0;
        int res = gCall.addOne(seq, resp);
        printf("addOne sync resp:%d %d\n", res, resp);

        gCall.async(&mytest::IMyTestCall::returnError, rpc::VoidParam{}, [](int res, rpc::VoidParam &resp)
                    { 
                        if(res == 0)
                            return;                       
                        auto str = rpc_error_str<mytest::MyTestCallErrCode>(res);
                        if(str.empty() == false)
                        {
                            printf("returnError return error:%d %s\n", res, str.data());
                        } });

        int arrReq[2] = {10, 11};
        int arrResp[2];
        res = gCall.array_test(arrReq, arrResp);
        printf("array_test sync res:%d %d %d\n", res, arrResp[0], arrResp[1]);

        if (seq == 10)
        {
            get_rpc_manager().unsubscribe<mytest::SubscribeData>();
        }

        sleep(5);
    }
    return 0;
}