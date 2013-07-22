// Copyright (c) 2013,  Lotill Co.Ltd.
// All rights reserved.
//
// Author: Reve Zhang 
// Created: 2013/07/13
// Version: 1.0

#ifndef Reactor_h__
#define Reactor_h__

namespace zerobus 
{
	namespace framework
	{
		class IOHandler
		{
		public:
			//handle receive message
			virtual int OnReceiveMessage(const Channel* channel) = 0;
		};

		//event-driven reactor
		class Reactor
		{
		public:
			//A destructor
			Reactor();

			//A destructor.
			~Reactor();

			//initialize reactor, alloc events with the num of maxEventNum
			int Initialize(int maxEventNum);

			/**
			* register a channel to reactor
			*@param channel
			*@param handler
			*@return success return 0, else return -1
			*/
			int RegisterChannel(const Channel* channel, const IOHandler *handler);

			/**
			* unregister a channel
			*@param channel
			*@return success return 0, else return -1
			/
			int UnRegisterChannel(const Channel* channel);
			
			/**
			* reactor loop
			* @param timeOut, The timeout argument specifies the minimum number of milliseconds that poll() 
			* will block0 return immediately, -1 shall block indefinitely until a requested event has occurred
			* @return success return num of events had occurred, -1 error 
			*/
			int EventLoop(int timeOut);

		private:
			//event callback
			struct event_item {
				Channel* _channel;
				IOHandler* _handler; 
			};

		private:
			bool _isInit;
			// real event size
			int _size;
			// max event num;
			int _maxEventNum;
			// zmq cache 
			zmq_pollitem_t* _zmq_pollitem_cache;
			//callback
			std::map<zmq_pollitem_t*, event_item> _callBackMap;

		private:
			NOTCOPYBLE(Reactor)
		};
	}
}
#endif // Reactor_h__
