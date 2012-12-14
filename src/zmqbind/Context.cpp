// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/08
// Version: 1.0

#include "Context.hpp"
#include "Socket.hpp"

namespace zerobus {
	namespace zmqbind {

		Context::Context() : _pcontext(NULL), 
			_linger(0), _high_water_mark(1000)
		{

		}

		Context::~Context()
		{
			if (_pcontext)
			{
				std::map<Socket*, bool>::iterator it = _socket_map.begin();
				for(; it != _socket_map.end(); ++it)
				{
					if (it->second)
					{
						delete it->first;
						it->second = false;
					}
				}

				zmq_ctx_destroy(_pcontext);
				_pcontext = NULL;
			}
		}

		int Context::Init(uint32_t linger, uint32_t high_water_mark)
		{
#if (ZMQ_VERSION >= ZMQ_MAKE_VERSION(3, 2, 0))
			_linger = linger;
			_high_water_mark = high_water_mark;

			_pcontext = zmq_ctx_new();

			if (!_pcontext)
			{
				return -2;
			}

			return 0;
#else
			return -2;
#endif
		}
	
		Socket* Context::CreateSocket(ESOCKET_TYPE type)
		{
			Socket* psocket = new Socket();

			if (!psocket)
			{
				return NULL;
			}

			int ret = psocket->Init(*this, type);

			if (ret != 0)
			{
				return NULL;
			}

			_socket_map[psocket] = true;
			return psocket;
		}

		void Context::DestroySocket(Socket* socket)
		{
			if (socket && _socket_map[socket])
			{
				_socket_map.erase(socket);
				delete socket;
			}
		}

	} //namespace zmqbind
} //namespace zerobus
