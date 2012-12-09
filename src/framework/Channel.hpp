// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/07
// Version: 1.0

#ifndef NETBUS_CHANNEL_H__
#define NETBUS_CHANNEL_H__

#include "../zmqbind/zmqbind.hpp"
#include "IPeer.hpp"

namespace netbus {
	namespace framework {
        const int MAX_URL_SIZE = 80;

		class IPeer;

		//����ͨ��
		class Channel 
		{
		public:
			Channel();

			Channel(int connector, int acceptor, int buf_size,
				const char* url);

			virtual ~Channel();

			virtual int InitChannel(zmqbind::Context& ctx, int id, int time_out) = 0;

			IPeer* GetLocalPeer();

			int SetChannel(int connector, int acceptor, int buf_size,
				const char* url);

		protected:
			int _connnector_id;  
			int _acceptor_id;
			int _buf_size;
			IPeer* _local_peer;
			char _url[MAX_URL_SIZE];

		private:
			NOTCOPYBLE(Channel)
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
#endif // NETBUS_CHANNEL_H__