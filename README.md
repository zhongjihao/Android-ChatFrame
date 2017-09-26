ChatFrame工程分为三大部分，每部分都是相互独立。

第一部分：后台任务处理框架Action和binding模型
   重点需要掌握Action和Binding的实现机制，具体实现在action包和binding包，数据逻辑处理可以抽象为Action模型，如发送消息可以使用SendMessageAction，
   接收消息可以定义ReceiveMessageAction等等。

   每个Action的启动方式两种：不带参数如 start()，带参数start(ActionMonitor monitor)。带参方式需要使用者自己继承ActionMonitor同时实现ActionMonitor.ActionCompletedListener接口。
   不管哪种方式启动action都会回调executeAction()方法，该方法中可以优先处理紧急任务，如果还需要处理非紧急任务，只需在executeAction()方法中调用requestBackgroundWork()即可回调到
   doBackgroundWork()方法，该方法执行后会回调processBackgroundResponse或processBackgroundFailure，其中processBackgroundResponse方法参数即为doBackgroundWork方法的返回值。

   采用start(ActionMonitor monitor)方法启动Action还会回调onActionSucceeded或onActionFailed，其中这两个方法最后一个参数result有两个来源，如果在executeAction方法中调用了requestBackgroundWork()方法，
   则参数result来自processBackgroundResponse或processBackgroundFailure的返回值; 反之参数result则来自executeAction方法的返回值。
   
   两种Action的启动方式具体用法详见SendMessageAction类和DialAction类。



第二部分：电话模块之核心模块Ril架构实现
   framework层RIL.java是Ril客户端模块的核心文件，它采用LocalSocket和底层RIL服务端模块进行跨进程通信，即采用本地Socket的TCP方式通信，TCP通信特点：数据无边界，可靠传输。
   RIL模块中采用如下方式处理TCP通信：
   1、发送数据：先要发送数据长度; 然后再发送数据内容
   2、接收数据：先要接收数据长度; 然后根据长度接收相应的数据
   3、同时需要判断是否要进行字节序转换

   RIL客户端模块具体见本工程framewok包，Ril服务端模块详见RilNativeApp工程，该工程是系统级应用，不支持NDK编译，需要在Android源码环境下编译，push到手机/system/app/RilService/中，同时需要将编译生成的库
   libril_native.so push到手机/system/lib/目录下


   

	   




