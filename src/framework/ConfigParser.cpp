// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/10
// Version: 1.0

#include "ConfigParser.hpp"
#include "Error.hpp"

namespace zerobus {
	namespace framework {
		
		int ConfigParser::LoadXml(const char* szXmlFile)
		{
			TiXmlDocument channel_def(szXmlFile);
		    bool load_ok = channel_def.LoadFile();

			if (!load_ok)
			{
				gErrorCode = ERRO_CONFIG_TINYXML;
				SetTinyXMLError(channel_def.ErrorDesc());
				return -1;
			}

			_channels.clear();

			TiXmlElement* root = channel_def.RootElement();
			
			TiXmlElement* item = root->FirstChildElement("channel");
			int ret = -1;

			while(item)
			{
				ret = Parser(item, channel_def);

				if (ret != 0)
				{
					gErrorCode = ERRO_CONFIG_TINYXML;
					SetTinyXMLError(channel_def.ErrorDesc());
				}

				item = item->NextSiblingElement("channel");
			}

			return ret;
		}

		int ConfigParser::Parser(TiXmlElement* pItem, TiXmlDocument& doc)
		{
			int ret = -1;

			if (pItem)
			{
				ChannelDef stChannel;

				ret = pItem->QueryIntAttribute("type", (int*)(&stChannel._type));

				if (ret != TIXML_SUCCESS)
				{
					gErrorCode = ERRO_CONFIG_TINYXML;
					SetTinyXMLError(doc.ErrorDesc());
					return -1;
				}

				ret = pItem->QueryIntAttribute("connector_id", &stChannel._channel_id._connector_id);

				if (ret != TIXML_SUCCESS)
				{
					gErrorCode = ERRO_CONFIG_TINYXML;
					SetTinyXMLError(doc.ErrorDesc());
					return -1;
				}

				ret = pItem->QueryIntAttribute("acceptor_id", &stChannel._channel_id._accpetor_id);

				if (ret != TIXML_SUCCESS)
				{
					gErrorCode = ERRO_CONFIG_TINYXML;
					SetTinyXMLError(doc.ErrorDesc());
					return -1;
				}

				ret = pItem->QueryIntAttribute("bufsize", &stChannel._bufsize);

				if (ret != TIXML_SUCCESS)
				{
					gErrorCode = ERRO_CONFIG_TINYXML;
					SetTinyXMLError(doc.ErrorDesc());
					return -1;
				}

				const char* url = pItem->Attribute("url");

				if (!url || strlen(url) > MAX_URL_SIZE)
				{
					gErrorCode = ERRO_CONFIG_TINYXML;
					SetTinyXMLError("url attribude has null value\n");
					return -1;
				}

				strncpy(stChannel._url, url, MAX_URL_SIZE);
				stChannel._url[MAX_URL_SIZE - 1] ='\0';

				_channels.push_back(stChannel);
				return 0;
			}

			return ret;
		}
	}
}
