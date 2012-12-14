// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#ifndef zerobus_IPEER_H__
#define zerobus_IPEER_H__

#include "CommonDefine.hpp"

#include "zmqbind/zmqbind.hpp"

namespace zerobus {
	namespace framework {

		enum PRSERVED_MSG {
			PRSERVED_MSG_CONN = 1,
			PRSERVED_MSG_CONN_RSP = 2,
			PRSERVED_MSG_HEARTBEAT = 3
		};

		void FreeCallback(void *, void *);
	
		class zerobus::zmqbind::Context;

		//peer abstract
		class IPeer {
		public:
			IPeer() {};
			virtual ~IPeer() {};

			virtual int Init(zerobus::zmqbind::Context& ctx) = 0;

			virtual int Send(const void* pmsg, int len) = 0;

			virtual int SendToPeer(IPeer* peer, const void* pmsg, int len) = 0;

			virtual int Recv(void* pmsg, int& len) = 0;

			virtual void* GetRawSocket() = 0;

			virtual int KeepAlive() = 0;

			virtual int SetPeerOpt(int option_name, const void *option_value, size_t len) = 0;

		private:
			NOTCOPYBLE(IPeer)
		};

		//Active peer, bind in an endpoint
		class ActivePeer : public virtual IPeer
		{
		public:
			ActivePeer() {};

			virtual ~ActivePeer() {};

			virtual int Connect(const char* endpoint, int time_out, const char* dealer_name) = 0;
		};

		//Passive peer, connect to remote endpoint
		class PassivePeer : public virtual IPeer
		{
		public:
			PassivePeer() {};

			virtual ~PassivePeer() {};

			virtual int Bind(const char* endpoint) = 0;
		};
	}
}

#endif // zerobus_IPEER_H__
