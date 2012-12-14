// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/10
// Version: 1.0

#include "ZBus.hpp"
#include "Channel.hpp"
#include "Error.hpp"

namespace zerobus {
	namespace framework {
		ZBus::ZBus()
		{
			_ctx.Init(10, 1000);
		}

		ZBus::~ZBus()
		{
			if (_channel_map.size() != 0)
			{
				std::map<ChannelID, Channel*>::iterator it = _channel_map.begin();

				for (; it != _channel_map.end(); ++it)
				{
					if (it->second)
					{
						delete it->second;
						it->second = NULL;
					}
				}
			}
		}

		ZBus& ZBus::Instance()
		{
			static ZBus zbus;
			return zbus;
		}

		int ZBus::LoadConfig(const char* szChannelDefXML)
		{
			int ret = -1;

			if (szChannelDefXML)
			{
				ret = _config_parser.LoadXml(szChannelDefXML);
			}

			return ret;
		}

		int ZBus::InitChannels(int peer_id)
		{
			std::vector<ChannelDef>::const_iterator it = _config_parser._channels.begin();
			int ret = 0;

			for (; it != _config_parser._channels.end(); ++it)
			{
				if (it->_channel_id._accpetor_id == peer_id || it->_channel_id._connector_id == peer_id)
				{
					Channel* pchannel = ChannelFactory::Instance().GetChannel(it->_type);

					pchannel->SetChannel(it->_channel_id._connector_id, it->_channel_id._accpetor_id, it->_bufsize
						, it->_url);
					ret = pchannel->InitChannel(_ctx, peer_id, 10000);

					if (ret != 0)
					{
						return ret;
					}

					std::pair<std::map<ChannelID, Channel*>::iterator, bool> result;
					result = _channel_map.insert(std::pair<ChannelID, Channel*>(it->_channel_id, pchannel));

					if (result.second == false)
					{
						gErrorCode = ERRO_INIT_CHANNEL_INSTER_MAP;
						return -1;
					}
				}
			}

			return ret;
		}

		Channel* ZBus::GetChannel(int connector_id, int acceptor_id)
		{
			ChannelID id;
			id._accpetor_id = acceptor_id;
			id._connector_id = connector_id;

			std::map<ChannelID, Channel*>::iterator it = _channel_map.find(id);

			if (it != _channel_map.end())
			{
				return it->second;
			}

			id._accpetor_id = connector_id;
			id._connector_id = acceptor_id;

			it = _channel_map.find(id);

			if (it != _channel_map.end())
			{
				return it->second;
			}

			return NULL;
		}
	}
}
