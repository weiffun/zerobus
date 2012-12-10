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

#ifndef zerobus_SOCKET_H__
#define zerobus_SOCKET_H__

#include "Common.hpp"

namespace zerobus {
	namespace zmqbind {
		class Context;
		class Message;

		//socket 类型
		enum ESOCKET_TYPE
		{
			SOCKET_TYPE_REQ = ZMQ_REQ,
			SOCKET_TYPE_RSP = ZMQ_REP,
			SOCKET_TYPE_DEALER = ZMQ_DEALER,
			SOCKET_TYPE_ROUTER = ZMQ_ROUTER,
			SOCKET_TYPE_PUB = ZMQ_PUB,
			SOCKET_TYPE_SUB = ZMQ_SUB,
			SOCKET_TYPE_PUSH = ZMQ_PUSH,
			SOCKET_TYPE_POLL = ZMQ_PULL
		};

		//zeromq c++绑定套接字类，去掉异常处理
		//封装zeromq socket操作
		class Socket
		{
		public:
			Socket();
			~Socket();

		public:
			int Init(const Context& ctx, ESOCKET_TYPE type, const char* identity = NULL);

			inline int Bind(const char* endpoint) 
			{ 
				return zmq_bind(_praw_socket, endpoint);
			}

			inline int Connect(const char* endpoint) 
			{ 
				return zmq_connect(_praw_socket, endpoint);
			}

			//发送消息后， msg重置为未初始化
			int Send(Message* msg, int flags = 0);

			int Recv(Message* msg, int flags = 0);

			inline int GetSocketOpt(int option_name, void *option_value, size_t *len)
			{
				return zmq_getsockopt(_praw_socket, option_name, option_value, len);
			}

			inline int SetSocketOpt(int option_name, const void *option_value, size_t len)
			{
				return zmq_setsockopt(_praw_socket, option_name, option_value, len);
			}

			inline void* GetRawSocket() { return _praw_socket; }

			inline int Close()
			{
				if (_praw_socket) return -2;

				int ret = zmq_close(_praw_socket);

				if (ret == 0) _praw_socket = NULL;

				return ret;
			}

			inline int GetSocketType()
			{
				int type = -1;
				size_t len = sizeof(int);

				int ret = GetSocketOpt(ZMQ_TYPE, &type, &len);

				if (ret == 0)
				{
					return type;
				}

				return ret;
			}

		private:
			void*	_praw_socket;

		private:
			Socket(const Socket&);
			void operator=(const Socket&);
		};	

	} //namespace zmqbind
} //namespace zerobus

#endif // zerobus_SOCKET_H__
