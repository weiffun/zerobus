/*
    Copyright (c) 2009-2011 250bpm s.r.o.
    Copyright (c) 2011 Botond Ballo
    Copyright (c) 2007-2009 iMatix Corporation

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/08
// Version: 1.0

#ifndef NETBUS_MESSAGE_H__
#define NETBUS_MESSAGE_H__

#include "Common.hpp"

namespace netbus {
	namespace zmqbind {

		//zeromq c++绑定，消息操作类
		class Message
		{
			friend class Socket;

		public:
			Message();
			~Message();

		public:
			int Init();

			int Init(int size);

			int Init(void *data, size_t size, zmq_free_fn *ffn, void *hint);

			int ReBuild();

			int ReBuild(size_t size);

			int ReBuild(void *data, size_t size, zmq_free_fn *ffn, void *hint);

			int Move (Message *msg);

			int Copy (Message *msg);

			inline void *Data ()
			{
				if (!_is_inited) return NULL;

				return zmq_msg_data (&_msg);
			}

			inline const void* Data () const
			{
				if (!_is_inited) return NULL;

				return zmq_msg_data (const_cast<zmq_msg_t*>(&_msg));
			}

			inline size_t Size () const
			{
				if (!_is_inited) return 0;

				return zmq_msg_size (const_cast<zmq_msg_t*>(&_msg));
			}

			inline int Close()
			{
				if (!_is_inited) return 0;

				int iRet = zmq_msg_close(&_msg);

				if (iRet == 0) _is_inited = false;

				return iRet;
			}

			inline bool IsInit() { return _is_inited; }

		private:
			zmq_msg_t	_msg;
			bool		_is_inited;

		private:
			Message(const Message&);
			void operator=(const Message&);
		};

	} //namespace zmqbind
} //namespace netbus

#endif // NETBUS_MESSAGE_H__

