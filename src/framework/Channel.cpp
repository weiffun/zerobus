// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#include "Channel.hpp"
#include "IPeer.hpp"
#include "Dealer2DealerChannel.hpp"
#include "Error.hpp"

#include "zmqbind/zmqbind.hpp"

#include <string.h>

namespace zerobus {
	namespace framework {
		Channel::Channel() : _connnector_id(-1), _acceptor_id(-1), _buf_size(1000), _local_peer(NULL)
		{
			memset(_url, 0, sizeof(_url));
		}

		Channel::Channel(int connector, int acceptor, int buf_size, const char* url) : _connnector_id(connector),
			_acceptor_id(acceptor), _buf_size(buf_size), _local_peer(NULL)
		{
			memset(_url, 0, sizeof(_url));

			if (url)
			{
				int len = strlen(url);

				if (len <= MAX_URL_SIZE)
				{
					memcpy(_url, url, len);
				}
			}
		}

		Channel::~Channel()
		{

		}

		int Channel::ReInitChannel(zerobus::zmqbind::Context& ctx, int id, int time_out)
		{
			if (_local_peer)
			{
				delete _local_peer;
			}

			return this->InitChannel(ctx, id, time_out);
		}

		int Channel::SetChannel(int connector, int acceptor, int buf_size,
			const char* url)
		{
			_connnector_id = connector;
			_acceptor_id = acceptor;
			_buf_size = buf_size;

			if (url)
			{
				int len = strlen(url);

				if (len <= MAX_URL_SIZE)
				{
					memcpy(_url, url, len);
					return 0;
				}
			}
			
			return -1;
		}

		int Channel::Send(const void* pmsg, int len)
		{
			return _local_peer->Send(pmsg, len);
		}

		int Channel::Recv(void* pmsg, int& len)
		{
			return _local_peer->Recv(pmsg, len);
		}

		int Channel::SendToChannel(Channel* channel, const void* pmsg, int len)
		{
			if (channel)
			{
				return _local_peer->SendToPeer(channel->_local_peer, pmsg, len);
			}

			return -1;
		}

		void* Channel::GetRawSocket()
		{
			if (_local_peer)
				return _local_peer->GetRawSocket();
			
			return NULL;
		}

		int Channel::KeepAlive()
		{
			if (!_local_peer)
				return -1;

			return _local_peer->KeepAlive();
		}

		bool Channel::IsInitized()
		{
			if (_local_peer)
			{
				return true;
			}

			return false;
		}

		int Channel::InitLocalPeer(zerobus::zmqbind::Context& ctx)
		{
			if (_local_peer)
			{
				int ret = _local_peer->Init(ctx);

				if (ret != 0)
				{
					return ret;
				}

				if (_buf_size >= 0)
				{
					//send water mark
					ret = _local_peer->SetPeerOpt(ZMQ_SNDHWM, &_buf_size, sizeof (int));

					if (ret < 0)
					{
						return ret;
					}

					//recv water mark
					ret = _local_peer->SetPeerOpt(ZMQ_RCVHWM, &_buf_size, sizeof (int));
				}
				
				return ret;
			}

			return -1;
		}

		ChannelFactory& ChannelFactory::Instance()
		{
			static ChannelFactory factory;
			return factory;
		}

		Channel* ChannelFactory::GetChannel(int type)
		{
			switch (type)
			{
			case CHANNEL_DEALER2DEALER:
				{
					return new Dealer2DealerChannel();
				}
			default:
				{
					return NULL;
				}
			}
		}
	}
}
