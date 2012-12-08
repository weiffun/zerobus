// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/08
// Version: 1.0

#ifndef NETBUS_COMMON_H__
#define NETBUS_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <zmq.h>

#ifdef __cplusplus
}
#endif

namespace netbus {
	namespace zmqbind {

		inline const char *GetErrorStr(int errcode)
		{
			return zmq_strerror(errcode);
		}

		inline int GetErrorCode ()
		{
			return zmq_errno();
		}

		inline int Poll (zmq_pollitem_t *items, int nitems, long timeout = -1)
		{
			if (!items) return -2;

			int rc = zmq_poll (items, nitems, timeout);

			return rc;
		}

		inline void Version (int *major, int *minor, int *patch)
		{
			zmq_version (major, minor, patch);
		}

	} //namespace zmqbind
} //namespace netbus

#endif // NETBUS_COMMON_H__
