// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/07
// Version: 1.0

#ifndef zerobus_ERROR_H__
#define zerobus_ERROR_H__

#include "zmqbind/Common.hpp"

#include <string.h>

namespace zerobus {
	namespace framework {
		enum ERROR_DEFINATION {
			ERRO_MIN = 99999900,
			ERRO_MALLOC_MEM_FAILED = 99999900,
			ERRO_ID_NOT_BELONGTO_CHANNEL = 99999901,
			ERRO_CONFIG_TINYXML = 99999902,
			ERRO_INIT_CHANNEL_INSTER_MAP = 99999903,
			ERRO_CHANNEL_INITED = 99999904,
			ERRO_MAX = 99999999
		};

		struct Error {
			int _code;
			char str[100];
		};

		//error map
		static Error gErrorMap[100] = {
			{ERRO_MALLOC_MEM_FAILED, "failed to malloc/new \n"},
			{ERRO_ID_NOT_BELONGTO_CHANNEL, "id not belong to channel\n"},
			{ERRO_CONFIG_TINYXML, "tinyxml failed \n"},
			{ERRO_INIT_CHANNEL_INSTER_MAP, "init channel failed, same channel had init\n"},
			{ERRO_CHANNEL_INITED, "channel had been initialized\n"},
		};

		//error code
		static int gErrorCode = 0;
		static const int TINYXMLERRSTRSIZE = 1024;
		static char gTinyXMLErrorStr[TINYXMLERRSTRSIZE] = {0};

		inline const char *GetErrorStr(int errcode)
		{
			if (errcode == ERRO_CONFIG_TINYXML)
				return gTinyXMLErrorStr;

			if (errcode >= ERRO_MIN && errcode < ERRO_MAX)
				return gErrorMap[errcode - ERRO_MIN].str;
			
			return zmqbind::GetErrorStr(errcode);
		}

		inline int GetErrorCode ()
		{
			if (gErrorCode != 0)
				return gErrorCode;

			return zmqbind::GetErrorCode();
		}

		inline int SetTinyXMLError(const char *szError)
		{
			int ret = -1;

			if (szError)
			{
				strncpy(gTinyXMLErrorStr, szError, sizeof(gTinyXMLErrorStr));
				gTinyXMLErrorStr[TINYXMLERRSTRSIZE - 1] = '\0';
			}

			return ret;
		}
	}
}
#endif // zerobus_ERROR_H__
