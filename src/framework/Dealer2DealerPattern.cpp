// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#include "Dealer2DealerPattern.hpp"

#include <string.h>

namespace netbus {
	namespace framework {

		void FreeCallback(void *data, void *hint)
		{

		}

		DealerActivePeer::DealerActivePeer()
		{
		
		}

		DealerActivePeer::~DealerActivePeer()
		{
			_socket.Close();
		}

		int DealerActivePeer::Init(zmqbind::Context& ctx)
		{
			return _socket.Init(ctx, zmqbind::SOCKET_TYPE_DEALER);
		}

		int DealerActivePeer::Connect(const char* endpoint, int time_out, const char* dealer_name)
		{
			int ret = 0;

			//设置标识
			if (dealer_name)
			{
				ret = _socket.SetSocketOpt(ZMQ_IDENTITY, dealer_name, strlen(dealer_name));

				if (ret != 0)
				{
					return ret;
				}
			}

			//建立连接, zeromq是异步建立连接
			ret = _socket.Connect(endpoint);

			if (ret != 0 || time_out == 0)
			{
				return ret;
			}

			//发送和接受包测试连接状况

			zmq_pollitem_t items [] = { { _socket.GetRawSocket(), 0, ZMQ_POLLIN, 0 } };

			if (time_out < 0)
			{
				while (1) {
					SendConnectMsg();
					zmqbind::Poll(items, 1, 1000);
					if (items [0].revents & ZMQ_POLLIN) {
						return 0;
					}
				}
			}
			else
			{
				int delta = 500;
				int time = time_out;

				do 
				{
					SendConnectMsg();
					zmqbind::Poll(items, 1, delta);
					if (items [0].revents & ZMQ_POLLIN) {
						return 0;
					}

					time -= delta;
				} while (time > 0);
			}
			
			return -2;
		}
	
		int DealerActivePeer::Send(const void* pmsg, int len)
		{
			zmqbind::Message message;

			int ret = message.Init(const_cast<void *>(pmsg), len, FreeCallback, NULL);

			if (ret != 0)
			{
				return ret;
			}

			return _socket.Send(&message, ZMQ_DONTWAIT);
		}

		int DealerActivePeer::SendToPeer(IPeer* peer, const void* pmsg, int len)
		{
			if (!peer)
				return -2;

			return peer->Send(pmsg, len);
		}

		int DealerActivePeer::Recv(void* pmsg, int& len)
		{
			zmqbind::Message message;

			int ret = message.Init();

			if (ret != 0)
			{
				return ret;
			}

			int nbyte = _socket.Recv(&message, ZMQ_DONTWAIT);

			if (nbyte <= 0)
			{
				len = 0;
				return ret;
			}

			len = nbyte;
			memcpy(pmsg, message.Data(), len);

			return nbyte;
		}

		int DealerActivePeer::KeepAlive()
		{
			char data = PRSERVED_MSG_HEARTBEAT;

			return Send(&data, sizeof(data));
		}

		int DealerActivePeer::SendConnectMsg()
		{
			char data = PRSERVED_MSG_CONN;

			return Send(&data, sizeof(data));
		}

		DealerPassivePeer::DealerPassivePeer()
		{

		}

		DealerPassivePeer::~DealerPassivePeer()
		{
			_socket.Close();
		}

		int DealerPassivePeer::Init(zmqbind::Context& ctx)
		{
			return _socket.Init(ctx, zmqbind::SOCKET_TYPE_DEALER);
		}

		int DealerPassivePeer::Bind(const char* endpoint)
		{
			return _socket.Bind(endpoint);
		}

		int DealerPassivePeer::Send(const void* pmsg, int len)
		{
			zmqbind::Message message;

			int ret = message.Init(const_cast<void *>(pmsg), len, FreeCallback, NULL);

			if (ret != 0)
			{
				return ret;
			}

			return _socket.Send(&message, ZMQ_DONTWAIT);
		}

		int DealerPassivePeer::SendToPeer(IPeer* peer, const void* pmsg, int len)
		{
			if (!peer)
				return -2;

			return peer->Send(pmsg, len);
		}

		int DealerPassivePeer::Recv(void* pmsg, int& len)
		{
			zmqbind::Message message;

			int ret = message.Init();

			if (ret != 0)
			{
				return ret;
			}

			int nbyte = _socket.Recv(&message, ZMQ_DONTWAIT);

			if (nbyte <= 0)
			{
				len = 0;
				return ret;
			}

			len = nbyte;
			memcpy(pmsg, message.Data(), len);

			if (nbyte == 1 && (*(char*)pmsg) == PRSERVED_MSG_CONN)
			{
				char data = PRSERVED_MSG_CONN_RSP;
				Send(&data, sizeof(data));
			}

			return nbyte;
		}

		int DealerPassivePeer::KeepAlive()
		{
			char data = PRSERVED_MSG_HEARTBEAT;

			return Send(&data, sizeof(data));
		}
	
	}
}
