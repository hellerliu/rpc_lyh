# demo说明：

    以任意顺序启动rpc_manager(只能单一，服务注册发现中心),rpc_server(只能单一，请求应答rpc的服务端和订阅发布topic的发布端)，rpc_client(可多个，请求应答rpc的客户端和订阅发布topic的订阅端)。

# 功能如下：

1. 当服务器coredump重启后，客户端会自动重连，重发rpc请求，重订阅topic，恢复到以前的通信状态；

2. 支持共享内存，UNIX Domain Socket，tcp三种通信方式；共享内存采用无锁模式，高效和避免进程崩溃后对共享内存的死锁；uds，tcp实现了域划分：本机域和网络域。例如同一服务可在两个不同soc的uds上。支持多soc一个网络域，多本机域通信。

3. 简单的线程模型，只有两个后台线程：采用libevent的网络通信线程，和基于队列的数据处理回调的线程。两个线程都支持postCall，sendCall异步同步函数调用，postMessage，sendMessage异步同步消息调用。并且数据处理回调线程支持协程，可以无阻塞的处理多个基于marl条件变量的等待。例如可在回调中启用协程，来处理rpc请求，或者将长时间任务分配到别的线程等，可避免数据处理线程的阻塞。

4. rpc和订阅发布的序列化支持如下： c/c++结构体(可包含字符串，数组，map等stl数据结构)、protobuf、json、ros1；并在一个头文件中，扩展非常简单。c/c++结构体序列化简单，不用编译中间的idl接口，可方便的用于中间件soc层通信，或者SOC和MCU AutosarCP的通信；protobuf可用于soc内部通信，或者上云转grpc通信；json可用于restful上云，或者js、python、dart、go等脚本语言的调用中间件时的通信；ros1/ros2的序列化的兼容。

5. 服务的定义简单，采用c++的class和虚函数重载即可完成服务的接口定义，和server端的功能描述。rpc支持同步调用，异步回调两种模型。

6. 订阅发布API使用简单，topic自动推导，publish、subscribe、unsubscribe即可完成详细功能。topic消息是多发布者多订阅者模型。

7. 接口简单，不涉及到复杂的c++特性如模板等，易于编写c api，并方便flutter，go等调用FFI调用c接口。

8. 系统稳定，扩展性强。由于代码精简，线程模型合理，规避了多线程安全,内存泄露等问题，并易于维护。

9. 外部接口API均为多线程安全，并且使用简单。只需要常规的c++知识，不需要额外的学习，就能快速的使用。
