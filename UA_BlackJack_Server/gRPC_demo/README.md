# gRPC使用指南
## 环境配置

按照 [gRPC and Protocol Buffers安装](https://grpc.io/docs/languages/cpp/quickstart/) 中的内容完成 gRPC及其依赖的安装。

其中的两条export命令，只对当前终端有效
```bash
$ export MY_INSTALL_DIR=$HOME/.local
$ export PATH="$MY_INSTALL_DIR/bin:$PATH"
```
为了方便，可以将这两条export写到 ``~/.bashrc`` 中

![rpc1](images/rpc1.png)

之后运行以下命令使配置生效

```bash
$ source ~/.bashrc
```



## 编译运行demo

运行脚本 ``no_proxy_add.py``，该脚本只需要运行一次，以后编译运行项目的时候无需再次运行。

```bash
$ python no_proxy_add.py
$ source /etc/bashrc
```

在 ``gRPC_demo`` 目录下，运行以下命令

```bash
$ mkdir -p cmake/build
$ cd cmake/build
$ cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..
$ make
```

在 ``gRPC_demo/cmake/build`` 下会生成  ``client-syn server-syn client-asyn server-asyn``  这四个可执行文件，前两个表示执行同步请求数据的客户端和服务端，后两个表示执行异步请求的客户端和服务端。同步和异步之间可以混用，例如：

```bash
./client-syn
./server-asyn
```

或者

```bash
./client-asyn
./server-syn
```



## 按需修改demo

**demo.proto**

此文件定义了用于通信的数据格式以及用于服务的具体函数，demo中以 ``GetNameService`` 为例展示了如何定义一个服务、服务下的具体函数以及消息格式，以实现从服务器上获取Name

### 同步RPC

**client-syn.cpp**

实现同步RPC通信的客户端源文件。关键调用为58行，执行后会阻塞直到收到服务器端的消息回复。

```c++
Status status = stub_->GetName(&context, request, &reply);
```

**server-syn.cpp**

实现同步RPC通信的服务端源文件。46~52行为服务端针对客户端的GetName调用所定义的具体操作。

### 异步RPC

异步RPC的通知机制为：

- 客户端发起异步调用请求，之后由gRPC负责将请求结果保存到客户端定义的 ``CompletionQueue`` 队列中，客户端自行决定读取队列中的响应的时机。
- 服务端同样需要发起异步的监听调用，gRPC负责将监听到的请求保存到服务端定义的 ``CompletionQueue`` 队列中，服务端自行决定处理队列中的请求的时机。

**client-asyn.cpp**

实现异步RPC通信的客户端源文件。92~99行为客户端定义的 **调用请求类**，类里封装了一些异步调用的需要用到的成员数据，如``context`` 和 ``response_reader``。用户可以自行添加一些其它数据以及相应的回调函数。一般来说，``reply`` 是必须的，因为需要获取响应的数据。

上面提到的gRPC负责将请求的结果保存到队列中，这里**"请求的结果"**指的就是调用请求类实例的地址。体现在58行，以及66行。

**server-asyn.cpp**

实现异步RPC通信的服务端源文件。72~113行的 ``CallData`` 类和客户端中的调用请求类在功能上类似。``Proceed()`` 方法负责提供关于客户端请求的解析以及响应数据的填充。

**CMakeLists.txt**

CMakeLists.txt中需要修改的地方为注释标注的位置

-  ``# demo dependent`` ，例如 ``demo.proto`` 中的``demo``需要替换成自定义的 ``.proto`` 文件中的 ``package`` 名字。

- ``# cpp file dependent`` ，``server-asyn client-asyn server-syn client-syn`` 替换成自定义的cc源文件的前缀

之后就可以运行以下命令重新编译修改后的项目

```bash
$ cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..
$ make
```

