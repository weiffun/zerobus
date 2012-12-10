// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#ifndef zerobus_DEALER2DEALERPATTERN_H__
#define zerobus_DEALER2DEALERPATTERN_H__

#include "../zmqbind/zmqbind.hpp"
#include "IPeer.hpp"

namespace zerobus {
	namespace framework {

		//主动端，主动发起连接类
		class DealerActivePeer : public ActivePeer
		{
		public:
			DealerActivePeer();

			~DealerActivePeer();

			int Init(zmqbind::Context& ctx);

			//同步建立连接, 发送sync包到对端
			//等待time_out毫秒无数据包返回-2，-1毫秒表示无限等待
			int Connect(const char* endpoint, int time_out, const char* dealer_name);

			//异步发送数据
			int Send(const void* pmsg, int len);

			//发送到指定peer
			int SendToPeer(IPeer* peer, const void* pmsg, int len);

			//异步接收数据
			int Recv(void* pmsg, int& len);

			//获取原始套接字
			void* GetRawSocket() { return _socket.GetRawSocket(); };

			int KeepAlive();

		private:
			int SendConnectMsg();

		private:
			zmqbind::Socket _socket;
		};

		//被动端，绑定在指定端口
		class DealerPassivePeer : public PassivePeer
		{
		public:
			DealerPassivePeer();

			~DealerPassivePeer();

			int Init(zmqbind::Context& ctx);

			//绑定到指定端口
			int Bind(const char* endpoint);

			//发送数据
			int Send(const void* pmsg, int len);

			//发送到指定peer
			int SendToPeer(IPeer* peer, const void* pmsg, int len);

			//接收数据
			int Recv(void* pmsg, int& len);

			//获取原始套接字
			void* GetRawSocket() { return _socket.GetRawSocket(); };

			int KeepAlive();

		private:
			zmqbind::Socket _socket;
		};
	}
}

#endif // zerobus_DEALER2DEALERPATTERN_H__
