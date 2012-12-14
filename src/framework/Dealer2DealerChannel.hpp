// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#ifndef NETBUS_DEALER2DEALERCHANNEL_H__
#define NETBUS_DEALER2DEALERCHANNEL_H__

#include "IPeer.hpp"
#include "Channel.hpp"
#include "CommonDefine.hpp"

#include "zmqbind/zmqbind.hpp"

namespace zerobus {
	namespace framework {

		class DealerPeer : public virtual IPeer
		{
		public:
			DealerPeer();

			~DealerPeer();

			virtual int Init(zmqbind::Context& ctx);

			virtual int Send(const void* pmsg, int len);

			virtual int SendToPeer(IPeer* peer, const void* pmsg, int len);

			virtual int Recv(void* pmsg, int& len);

			virtual void* GetRawSocket();

			virtual int KeepAlive();

			virtual int SetPeerOpt(int option_name, const void *option_value, size_t len);

		protected:
			zmqbind::Socket* _socket;
		};

		//dealer
		class DealerActivePeer : public ActivePeer, DealerPeer
		{
		public:
			DealerActivePeer();

			~DealerActivePeer();

			int Connect(const char* endpoint, int time_out, const char* dealer_name);

		private:
			int SendConnectMsg();
		};

		class DealerPassivePeer : public PassivePeer, DealerPeer
		{
		public:
			DealerPassivePeer();

			~DealerPassivePeer();

			int Bind(const char* endpoint);
		};

		//channel
		class Dealer2DealerChannel : public Channel
		{
		public:
			Dealer2DealerChannel();

			Dealer2DealerChannel(int connector, int acceptor, int buf_size,
				const char* url);

			virtual ~Dealer2DealerChannel();

			virtual int InitChannel(zmqbind::Context& ctx, int id, int time_out);

		private:
			DealerActivePeer _active;
			DealerPassivePeer _passive;

		private:
			NOTCOPYBLE(Dealer2DealerChannel)
		};
	}
}

#endif // NETBUS_DEALER2DEALERCHANNEL_H__
