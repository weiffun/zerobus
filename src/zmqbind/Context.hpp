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

#ifndef zerobus_CONTEXT_H__
#define zerobus_CONTEXT_H__

#include "Common.hpp"

namespace zerobus {
	namespace zmqbind {
		//zeromq c++绑定，上下文类，去掉异常处理
		//添加linger和阈值处理,处理信号和Ctrl + C。
		class Context
		{
			friend class Socket;

		public:
			Context();
			~Context();

		public:
			int Init(uint32_t linger, uint32_t high_water_mark);

			inline uint32_t GetLinger() const { return _linger; }

			inline void SetLinger(uint32_t linger) { _linger = linger; }

			inline uint32_t GetHighWaterMark() const { return _high_water_mark; }

			inline void SetHighWaterMark(uint32_t high_water_mark) 
			{ 
				_high_water_mark = high_water_mark;
			}

		private:
			void*		_pcontext; //zeromq context
			uint32_t    _linger;     //Linger timeout, socket关闭时，等待时间
			uint32_t	_high_water_mark; //消息队列最大阈值

		private:
			Context(const Context&);
			Context& operator=(const Context&);
		};

	} //namespace zmqbind
} //namespace zerobus

#endif // zerobus_CONTEXT_H__
