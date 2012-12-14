// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/10
// Version: 1.0

#ifndef zerobus_CONFIGPARSER_H__
#define zerobus_CONFIGPARSER_H__

#include "CommonDefine.hpp"

#include "tinyxml/tinyxml.h"

#include <vector>

namespace zerobus 
{
	namespace framework
	{
		class ConfigParser
		{
			friend class ZBus;

		public:
			ConfigParser() { };
			~ConfigParser() { };

			int LoadXml(const char* szXmlFile);

		private:
			int Parser(TiXmlElement* pItem, TiXmlDocument& doc);
			
		private:
			std::vector<ChannelDef> _channels;

		private:
			NOTCOPYBLE(ConfigParser)
		};
	}
}
#endif // zerobus_CONFIGPARSER_H__
