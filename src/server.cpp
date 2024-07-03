
#include "RpcCore/RpcManager.h"
#include "mytest/MySubscrible.h"
#include "MyTestCallService.h"

using namespace rpc;

int main()
{
    std::shared_ptr<mytest::MyTestCallService> service{new mytest::MyTestCallService};

    PublishTopic topic{std::in_place_type_t<mytest::SubscribeData>{}, PublishPolicy::RETAIN, 1, 2};
    PublishTopic topic1{std::in_place_type_t<mytest::SubscribeData>{}};
    PublishTopic topic2{std::in_place_type_t<std::string>{}, "strdata"};
    PublishTopic topicJson{std::in_place_type_t<mytest::JsonData>{}};

    get_rpc_manager().initalize({service}, {topic, topic1, topic2, topicJson});

    mytest::SubscribeData data{0, 0};
    while (true)
    {
        data.test++;
        std::string shmData = data.test % 2 == 1 ? "12345" : "234567890";
        get_rpc_manager().publish(data, std::move(shmData));
        get_rpc_manager().publish(data);
        get_rpc_manager().publish("strdata", std::string{"test"});

        mytest::JsonData jsonMsg;
        jsonMsg.x = 1;
        jsonMsg.msg = "123";
        get_rpc_manager().publish(jsonMsg);
        sleep(10);
    }
    return 0;
}
