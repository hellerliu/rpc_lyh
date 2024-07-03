#pragma once

#include <string>
#include "RpcBase/Serialization.h"
#include <string>
#include <optional>
#include "reflect/reflection_json.h"

namespace mytest
{
    struct SubscribeData
    {
        unsigned long test;
        unsigned long time;
    };
    static_assert(rpc::is_valid_struct_serialize_v<SubscribeData>);

    struct SubscribeStr
    {
        unsigned long test;
        std::string str;
        template <typename T>
        void serialize(T &ar)
        {
            ar & test & str;
        }
    };
    static_assert(rpc::is_valid_struct_serialize_v<SubscribeStr>);

    struct JsonData
    {
        int x;
        std::optional<std::string> msg;
    };
}

namespace reflect
{
    DEFINE_STRUCT_SCHEMA(mytest::JsonData,
                         DEFINE_STRUCT_FIELD(x),
                         DEFINE_STRUCT_FIELD(msg));
}