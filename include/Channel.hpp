// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/07
// Version: 1.0

#ifndef zerobus_CHANNEL_H__
#define zerobus_CHANNEL_H__

#include "../zmqbind/zmqbind.hpp"
#include "IPeer.hpp"
#include "Commondefine.hpp"

namespace zerobus {
	namespace framework {

		class IPeer;

		//网络通道
		class Channel 
		{
		public:
			Channel();

			Channel(int connector, int acceptor, int buf_size,
				const char* url);

			virtual ~Channel();

			virtual int InitChannel(zmqbind::Context& ctx, int id, int time_out) = 0;

			int SetChannel(int connector, int acceptor, int buf_size,
				const char* url);

			int Send(const void* pmsg, int len);

			int SendToChannel(Channel* peer, const void* pmsg, int len);

			int Recv(void* pmsg, int& len);

			//获取zeromq socket
			void* GetRawSocket() { return _local_peer->GetRawSocket(); };

			int KeepAlive();

		protected:
			int _connnector_id;  
			int _acceptor_id;
			int _buf_size;
			IPeer* _local_peer;
			char _url[MAX_URL_SIZE];

		private:
			NOTCOPYBLE(Channel)
		};

		class ChannelFactory
		{
		public:
			static ChannelFactory& Instance();

			Channel* GetChannel(int type);
			
		private:
			ChannelFactory() { };
			NOTCOPYBLE(ChannelFactory)
		};

		class Dealer2DealerChannel : public Channel
		{
		public:
			Dealer2DealerChannel();

			Dealer2DealerChannel(int connector, int acceptor, int buf_size,
				const char* url);

			virtual ~Dealer2DealerChannel();

			virtual int InitChannel(zmqbind::Context& ctx, int id, int time_out);

		private:
			NOTCOPYBLE(Dealer2DealerChannel)
		};
	}
}
#endif // zerobus_CHANNEL_H__
