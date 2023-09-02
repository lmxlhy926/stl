### mqtt协议介绍

---

MQTT是一个轻量的发布订阅模式消息传输协议，专门针对低带宽和不稳定网络环境的物联网应用设计。

**特点**：

1. 开放消息协议，简单易实现
2. 发布订阅模式，一对多消息发布
3. 基于TCP/IP网络连接
4. 1字节固定报头，2字节心跳报文，报文结构紧凑
5. 消息QoS支持，可靠传输保证

**应用**：

1. 物联网M2M通信，物联网大数据采集

2. Android消息推送，WEB消息推送

3. 移动即时消息，例如Facebook Messenger

4. 智能硬件、智能家具、智能电器

5. 车联网通信，电动车站桩采集

6. 智慧城市、远程医疗、远程教育

7. 电力、石油与能源等行业市场

**订阅主题通配符**：

每一个MQTT消息都包含一个topic来对它进行分类。MQTT服务器使用topic来确定由哪些订阅者来接收发布给服务器的消息。MQTT支持`hierarchy topic namespace`，在层级里使用'/'来界定等级。如SENSOR/1/HUMIDITY，SENSOR/2/HUMIDITY

订阅主题支持2种通配符：

* '**#**'：表示通配多个层级，例如a/#，匹配a/x, a/b/c/d
* '**+**'：表示通配一个层级，例如a/+，匹配a/x, a/y
* **Notice**：发布者不能在主题名中使用通配符，只有订阅主题可以使用通配符

**服务质量(quality of service)**

在clients和servers之间传递消息时MQTT协议提供三种服务质量，`"at most once"`, `"at lest once"`, `"exactly once"`。QoS是每个被发布的message的一个属性,可以通过相应的参数设置。一个订阅客户端可以设置服务器转发该客户端订阅的报文时使用的最大QoS。因此转发到订阅客户端的报文的QoS可能和发布者发布时赋予的QoS不同，这取决于两者的最小值。

* `at most once`：message最多被转发一次，也可能根本不会被转发。消息传递不会被确认，转发的message也不会被存储。如果client没有连接或者server转发失败，那么message会丢失。QoS0是最快的传输模式。
* `at least once`：message至少被传递一次。如果在sender收到ack之前发生了错误，那么message可能会被传输多次。发送者必须本地存储message，接收者接收到message并且发布后会给发送者返回一个publishack确认消息，此时发送者可以删除本地的存储。
* `exactly once`：message只会被成功的递送一次。sender必须本地存储message直到收到receiver将接收到的message发布后返回的确认信息。QoS2是最安全的也是最慢的传输模式，它比QoS1多了更多的握手和确认序列来确保没有复制信息发生。

**MQTT遗愿消息(Last Will)**

MQTT客户端向服务器端CONNECT请求时，可以设置是否发送遗愿消息(Will Message)标志，和遗愿消息主题(Topic)与内容(Payload)。MQTT客户端异常下线时(客户端断开前未向服务器发送DISCONNECT消息)，MQTT消息服务器会发布遗愿消息。

**MQTT保留消息(Retained Message)**

MQTT客户端向服务器发布(PUBLISH)消息时，可以设置保留消息(Retained Message)标志。保留消息(Retained Message)会驻留在消息服务器，后来的订阅者订阅主题时仍可以接收该消息。



### Paho MQTT c client

---

`Paho MQTT c client`是一个使用ANSI标准C编写的全功能的MQTT客户端库。使用C而不是C++是为了将可移植性最大化。该库包含同步和异步2套API，API的调用分别从`MQTTClient`和`MQTTAsync`开始。同步API更加简单和有帮助，为了实现这个目标一些函数将会阻塞直到操作完成。相反，在异步API里只有`waitForCompletion`这个函数会被阻塞。结果的处理会在回调函数里被完成，这就使异步API非常适合在程序不是主控线程的情景中使用。

**库使用的步骤**

- Create a client object
- Set the options to connect to an MQTT server
- Set up callback functions
- Connect the client to an MQTT server
- Subscribe to any topics the client needs to receive
- Repeat until finished:
  - Publish any messages the client needs to
  - Handle any incoming messages
- Disconnect the client
- Free any memory being used by the client



### 同步API

---

同步API支持2种`mode`：同步模式和异步模式。程序只要调用了`MQTTClient_setCallbacks()`函数就会处于异步模式，否则就处于同步模式。

同步模式下，客户端程序运行在一个单线程下。使用`MQTTClient_publish()`和`MQTTClient_publishMessage()`函数发布消息，使用`MQTTClient_receive`函数接收消息。为了确保QoS1和QoS2等级的消息被成功的递送，客户端程序必须调用`MQTTClient_waitForCompletion`函数。而且客户端程序必须频繁的调用`MQTTClient_receive`或者`MQTTClient_yield`函数来使程序处理`acknowledgements`和`MQTT "pings"`(使网络持续连接到服务器)。

在异步模式下，客户端程序运行在多线程模式。主程序调用client library里的函数来发布和订阅消息，这点和同步模式是相同的，但是握手和保持网络连接的处理在后台执行。通过调用`MQTTClient_setCallbacks()`来注册callbacks，将状态的通知和消息接收也交给客户端程序来处理。但是API不是线程安全的，不能在没有同步机制的提前下在多个线程里调用它。可以使用MQTTAsync API来实现多线程调用。



### 异步API

---

client application运行于多个线程之上。握手和维持网络连接的处理都在后台执行。异步API是线程安全的：**函数可以被多个应用程序线程调用**。状态推送和消息接收都是通过注册回调函数来处理。一些设置回调函数的API：

* `MQTTAsync_setCallbacks()`
* `MQTTAsync_messageArrived()`
* `MQTTAsync_connectionLost()`
* `MQTTAsync_deliveryComplete()`



### connectOptions

---

