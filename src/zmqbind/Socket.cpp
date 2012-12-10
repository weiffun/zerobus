// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/08
// Version: 1.0

#include "Socket.hpp"
#include "Context.hpp"
#include "Message.hpp"

#include <string.h>

namespace zerobus {
	namespace zmqbind {

		Socket::Socket() : _praw_socket(NULL)
		{

		}

		Socket::~Socket()
		{
			if (_praw_socket)
			{
				zmq_close(_praw_socket);
				_praw_socket = NULL;
			}
		}

		int Socket::Init(const Context& ctx, ESOCKET_TYPE type, const char* identity /* = NULL */)
		{
			_praw_socket = zmq_socket(ctx._pcontext, type);

			if (!_praw_socket)
			{
				return -2;
			}

			int size = 0;
			int ret = 0;

			//设置标识
			if (identity && (size = strlen(identity)))
			{
				ret = zmq_setsockopt(_praw_socket, ZMQ_IDENTITY, identity, size);

				if (ret < 0)
				{
					return ret;
				}
			}

			//设置linger
			int linger = ctx.GetLinger();
			ret = zmq_setsockopt (_praw_socket, ZMQ_LINGER, &linger, sizeof(int));

			if (ret < 0)
			{
				return ret;
			}

			int hwm = ctx.GetHighWaterMark();
			//发送阈值
			ret = zmq_setsockopt (_praw_socket, ZMQ_SNDHWM, &hwm, sizeof (int));

			if (ret < 0)
			{
				return ret;
			}

			//接受阈值
			ret = zmq_setsockopt (_praw_socket, ZMQ_RCVHWM, &hwm, sizeof (int));

			if (ret < 0)
			{
				return ret;
			}

			return 0;
		}

		int  Socket::Send(Message* msg, int flags /* = 0 */)
		{
			int nbytes = zmq_msg_send(&(msg->_msg), _praw_socket, flags);

			if (nbytes >= 0) 
			{
				//msg->Close();
				return nbytes;
			}

			if (zmq_errno () == EAGAIN) return 0;

			return nbytes;
		}

		int  Socket::Recv(Message* msg, int flags /* = 0 */)
		{
			int nbytes = zmq_msg_recv(&(msg->_msg), _praw_socket, flags);

			if (nbytes >= 0) return nbytes;

			if (zmq_errno () == EAGAIN) return 0;

			return nbytes;
		}

	} //namespace zmqbind
} //namespace zerobus
