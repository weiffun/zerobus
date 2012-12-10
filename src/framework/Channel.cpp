// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/11/24
// Version: 1.0

#include "Channel.hpp"
#include "Dealer2DealerPattern.hpp"
#include "../zmqbind/zmqbind.hpp"
#include "Error.hpp"

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
			if (_local_peer)
			{
				delete _local_peer;
				_local_peer = NULL;
			}
		}

		int Channel::SetChannel(int connector, int acceptor, int buf_size,
			const char* url)
		{
			_connnector_id = connector;
			_acceptor_id = acceptor;
			_buf_size = buf_size;
			_local_peer = NULL;

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

		int Channel::KeepAlive()
		{
			return _local_peer->KeepAlive();
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

		Dealer2DealerChannel::Dealer2DealerChannel() : Channel()
		{

		}

		Dealer2DealerChannel::Dealer2DealerChannel(int connector, int acceptor, int buf_size, 
			const char* url) : Channel(connector, acceptor, buf_size, url)
		{

		}

		Dealer2DealerChannel::~Dealer2DealerChannel()
		{
			if (_local_peer)
			{
				delete _local_peer;
				_local_peer = NULL;
			}
		}

		int Dealer2DealerChannel::InitChannel(zmqbind::Context& ctx, int id, int time_out)
		{
			gErrorCode = 0;

			if (id == _connnector_id)
			{
				//创建主动peer
				DealerActivePeer* peer = new DealerActivePeer();
				if (!peer)
				{
					gErrorCode = ERRO_MALLOC_MEM_FAILED;
					return -2;
				}

				int ret = peer->Init(ctx);

				if (ret != 0)
				{
					return ret;
				}

				ret = peer->Connect(_url, time_out, NULL);

				if (ret != 0)
				{
					return ret;
				}

				_local_peer = peer;
				return 0;
			}
			else if (id == _acceptor_id)
			{
				//创建被动peer
				DealerPassivePeer* peer = new DealerPassivePeer();
				if (!peer)
				{
					gErrorCode = ERRO_MALLOC_MEM_FAILED;
					return -2;
				}

				int ret = peer->Init(ctx);

				if (ret != 0)
				{
					return ret;
				}

				ret = peer->Bind(_url);

				if (ret != 0)
				{
					return ret;
				}

				_local_peer = peer;
				return 0;
			}

			gErrorCode = ERRO_ID_NOT_BELONGTO_CHANNEL;
			return -1;
		}

	}
}
