// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#ifndef NETBUS_IPEER_H__
#define NETBUS_IPEER_H__

#define NOTCOPYBLE(TYPE) \
		TYPE(const TYPE&);\
		void operator=(const TYPE&);\

namespace netbus {
	namespace framework {

		enum PRSERVED_MSG {
			PRSERVED_MSG_CONN = 1,
			PRSERVED_MSG_CONN_RSP = 2,
			PRSERVED_MSG_HEARTBEAT = 3
		};

		void FreeCallback(void *, void *);
	

		//网络端抽象接口
		class IPeer {
		public:
			IPeer() {};
			virtual ~IPeer() {};

			virtual int Init(zmqbind::Context& ctx) = 0;

			//发送数据
			virtual int Send(const void* pmsg, int len) = 0;

			//发送到指定peer
			virtual int SendToPeer(IPeer* peer, const void* pmsg, int len) = 0;

			//接收数据
			virtual int Recv(void* pmsg, int& len) = 0;

			//获取原始套接字
			virtual void* GetRawSocket() = 0;

			virtual int KeepAlive() = 0;

		private:
			NOTCOPYBLE(IPeer)
		};

		//主动端，主动发起连接类
		class ActivePeer : public IPeer
		{
		public:
			ActivePeer() {};

			virtual ~ActivePeer() {};

			//同步建立连接, 发送sync包到对端
			//等待time_out毫秒无数据包返回-2，-1毫秒表示无限等待
			virtual int Connect(const char* endpoint, int time_out, const char* dealer_name) = 0;
		};

		//被动端，绑定在指定端口
		class PassivePeer : public IPeer
		{
		public:
			PassivePeer() {};

			virtual ~PassivePeer() {};

			virtual int Bind(const char* endpoint) = 0;
		};
	}
}

#endif // NETBUS_IPEER_H__
