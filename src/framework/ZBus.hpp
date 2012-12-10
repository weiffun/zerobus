// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/10
// Version: 1.0

#ifndef zerobus_ZBUS_H__
#define zerobus_ZBUS_H__

#include "Commondefine.hpp"
#include "ConfigParser.hpp"
#include "../zmqbind/Context.hpp"

#include <map>

namespace zerobus 
{
	namespace framework
	{
		class Channel;

		//zbus
		class ZBus
		{
		public:
			~ZBus();

			static ZBus& Instance();

			int LoadConfig(const char* szChannelDefXML);

			int InitChannels(int peer_id);

			Channel* GetChannel(int connector_id, int acceptor_id);

		private:
			std::map<ChannelID, Channel*> _channel_map;
			ConfigParser _config_parser;
			zerobus::zmqbind::Context _ctx;

		private:
			ZBus();

		private:
			NOTCOPYBLE(ZBus)
		};
	}
}
#endif // zerobus_ZBUS_H__
