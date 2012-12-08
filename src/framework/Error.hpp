// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/07
// Version: 1.0

#ifndef NETBUS_ERROR_H__
#define NETBUS_ERROR_H__

#include "../zmqbind/Common.hpp"

namespace netbus {
	namespace framework {
		enum ERROR_DEFINATION {
			ERRO_MIN = 99999900,
			ERRO_MALLOC_MEM_FAILED = 99999900,
			ERRO_ID_NOT_BELONGTO_CHANNEL = 99999901,
			ERRO_MAX = 99999999
		};

		struct Error {
			int _code;
			char str[100];
		};

		//´íÎóÓ³Éä±í
		static Error gErrorMap[100] = {
			{ERRO_MALLOC_MEM_FAILED, "failed to malloc/new \n"},
			{ERRO_ID_NOT_BELONGTO_CHANNEL, "id not belong to channel\n"}
		};

		//´íÎóÂë
		static int gErrorCode = 0;

		inline const char *GetErrorStr(int errcode)
		{
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
	}
}
#endif // NETBUS_ERROR_H__
