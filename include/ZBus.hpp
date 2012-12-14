// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/10
// Version: 1.0

#ifndef zerobus_ZBUS_H__
#define zerobus_ZBUS_H__

#include "CommonDefine.hpp"
#include "ConfigParser.hpp"

#include "zmqbind/Context.hpp"

#include <map>

namespace zerobus 
{
	namespace framework
	{
		class Channel;

		/**
		* Channel manager.
		*/
		class ZBus
		{
		public:
			/**
			* A destructor.
			*/
			~ZBus();

			/**
			* Instance a zbus.
			*/
			static ZBus& Instance();

			/**
			* Load channels defination xml.
			* @param xml file name.
			* @return success return 0, else return -1.
			*/
			int LoadConfig(const char* szChannelDefXML);

			/**
			* Initialize all channels connected/bind the peer id.
			* @param peer id.
			* @return success return 0, else return -1.
			*/
			int InitChannels(int peer_id);

			/**
			* Get a channel.
			* @param connector peer id.
			* @param acceptor peer id.
			* @return success return the channel, else return NULL.
			*/
			Channel* GetChannel(int connector_id, int acceptor_id);

		private:
			std::map<ChannelID, Channel*> _channel_map; //channel map
			ConfigParser _config_parser;               // parser config
			zerobus::zmqbind::Context _ctx;            //context

		private:
			ZBus();

		private:
			NOTCOPYBLE(ZBus)
		};
	}
}
#endif // zerobus_ZBUS_H__
