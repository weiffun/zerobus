// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#include "Dealer2DealerChannel.hpp"
#include "Error.hpp"

#include <string.h>

namespace zerobus {
	namespace framework {

		void FreeCallback(void *data, void *hint)
		{

		}

		Dealer2DealerChannel::Dealer2DealerChannel() : Channel()
		{

		}

		Dealer2DealerChannel::Dealer2DealerChannel(int connector, int acceptor, int buf_size, 
			const char* url) : Channel(connector, acceptor, buf_size, url)
		{

		}

		Dealer2DealerChannel::~Dealer2DealerChannel()
		{
		
		}

		int Dealer2DealerChannel::InitChannel(zmqbind::Context& ctx, int id, int time_out)
		{
			gErrorCode = 0;

			if (IsInitized())
			{
				gErrorCode = ERRO_CHANNEL_INITED;
				return -1;
			}

			if (id == _connnector_id)
			{
				//设置本地端为主动peer
				_local_peer = &_active;

				int ret = InitLocalPeer(ctx);

				if (ret != 0)
				{
					_local_peer = NULL;
					return ret;
				}

				//connect
				ret = _active.Connect(_url, time_out, NULL);

				if (ret != 0)
				{
					_local_peer = NULL;
					return ret;
				}

				return 0;
			}
			else if (id == _acceptor_id)
			{
				//设置本地端为被动peer
				_local_peer = &_passive;

				int ret = InitLocalPeer(ctx);

				if (ret != 0)
				{
					_local_peer = NULL;
					return ret;
				}

				ret = _passive.Bind(_url);

				if (ret != 0)
				{
					_local_peer = NULL;
					return ret;
				}

				return 0;
			}

			gErrorCode = ERRO_ID_NOT_BELONGTO_CHANNEL;
			return -1;
		}

		DealerPeer::DealerPeer() : _socket(NULL)
		{

		}

		DealerPeer::~DealerPeer()
		{
			if (_socket)
			{
				_socket->Close();
			}
		}

		int DealerPeer::Init(zmqbind::Context& ctx)
		{
			if (_socket)
				return -1;

			_socket = ctx.CreateSocket(zmqbind::SOCKET_TYPE_DEALER);

			if (_socket)
				return 0;

			return -2;
		}

		int DealerPeer::Send(const void* pmsg, int len)
		{
			if (!_socket)
				return -1;

			zmqbind::Message message;

			int ret = message.Init(const_cast<void *>(pmsg), len, FreeCallback, NULL);

			if (ret != 0)
			{
				return ret;
			}

			return _socket->Send(&message, ZMQ_DONTWAIT);
		}

		int DealerPeer::SendToPeer(IPeer* peer, const void* pmsg, int len)
		{
			if (!peer)
				return -2;

			return peer->Send(pmsg, len);
		}

		int DealerPeer::Recv(void* pmsg, int& len)
		{
			if (!_socket)
				return -1;

			zmqbind::Message message;

			int ret = message.Init();

			if (ret != 0)
			{
				return ret;
			}

			int nbyte = _socket->Recv(&message, ZMQ_DONTWAIT);

			if (nbyte <= 0)
			{
				len = 0;
				return ret;
			}

			len = nbyte;
			memcpy(pmsg, message.Data(), len);

			return nbyte;
		}

		void* DealerPeer::GetRawSocket()
		{
			if (!_socket)
				return NULL;

			return _socket->GetRawSocket();
		}

		int DealerPeer::KeepAlive()
		{
			char data = PRSERVED_MSG_HEARTBEAT;

			return Send(&data, sizeof(data));
		}

		int DealerPeer::SetPeerOpt(int option_name, const void *option_value, size_t len)
		{
			if (!_socket)
				return -1;

			return _socket->SetSocketOpt(option_name, option_value, len);
		}


		DealerActivePeer::DealerActivePeer()
		{
		
		}

		DealerActivePeer::~DealerActivePeer()
		{
		
		}


		int DealerActivePeer::Connect(const char* endpoint, int time_out, const char* dealer_name)
		{
			int ret = 0;

			if (!_socket)
				return -1;

			//设置标识
			if (dealer_name)
			{
				ret = _socket->SetSocketOpt(ZMQ_IDENTITY, dealer_name, strlen(dealer_name));

				if (ret != 0)
				{
					return ret;
				}
			}

			//建立连接, zeromq是异步建立连接
			ret = _socket->Connect(endpoint);

			if (ret != 0 || time_out == 0)
			{
				return ret;
			}

			//发送和接受包测试连接状况

			zmq_pollitem_t items [] = { { _socket->GetRawSocket(), 0, ZMQ_POLLIN, 0 } };

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

		}

		int DealerPassivePeer::Bind(const char* endpoint)
		{
			if (!_socket)
				return -1;

			return _socket->Bind(endpoint);
		}
	
	}
}
