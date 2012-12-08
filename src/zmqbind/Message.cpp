// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/08
// Version: 1.0

#include "Message.hpp"

namespace netbus {
	namespace zmqbind {
		Message::Message() : _is_inited(false)
		{

		}

		Message::~Message()
		{
			Close();
		}

		int Message::Init()
		{
			if (_is_inited)
			{
				return -2;
			}

			int iRet = zmq_msg_init(&_msg);

			if (iRet == 0)
			{
				_is_inited = true;
			}

			return iRet;
		}

		int Message::Init(int size)
		{
			if (_is_inited)
			{
				return -2;
			}

			int iRet = zmq_msg_init_size(&_msg, size);

			if (iRet == 0)
			{
				_is_inited = true;
			}

			return iRet;
		}

		int Message::Init(void *data, size_t size, zmq_free_fn *ffn, void *hint)
		{
			if (_is_inited || !data || !ffn)
			{
				return -2;
			}

			int iRet = zmq_msg_init_data(&_msg, data, size, ffn, hint);

			if (iRet == 0)
			{
				_is_inited = true;
			}

			return iRet;
		}

		int Message::ReBuild()
		{
			if (!_is_inited)
			{
				return Init();
			}

			int iRet = Close();

			if (iRet != 0)
			{
				return iRet;
			}

			return Init();
		}

		int Message::ReBuild(size_t size)
		{
			if (!_is_inited)
			{
				return Init(size);
			}

			int iRet = Close();

			if (iRet != 0)
			{
				return iRet;
			}

			return Init(size);
		}

		int Message::ReBuild(void *data, size_t size, zmq_free_fn *ffn, void *hint)
		{
			if (!_is_inited)
			{
				return Init(data, size, ffn, hint);
			}

			int iRet = Close();

			if (iRet != 0)
			{
				return iRet;
			}

			return Init(data, size, ffn, hint);
		}

		int Message::Move (Message *msg)
		{	
			int rc = zmq_msg_move (&_msg, &(msg->_msg));

			if (rc == 0)
			{
				_is_inited = true;
			}

			return rc;
		}

		int Message::Copy (Message *msg)
		{
			int rc = zmq_msg_copy(&_msg, &(msg->_msg));

			if (rc == 0)
			{
				_is_inited = true;
			}

			return rc;
		}

	} //namespace zmqbind
} //namespace netbus

