#include "Reactor.hpp"

#include <cstdlib>

namespace zerobus 
{
	namespace framework 
	{
		Reactor::Reactor() : _isInit(false), _size(0)
		{

		}

		Reactor::~Reactor()
		{
			for (int i = 0; i < _size; i++)
			{
				CallBackMap::iterator it = _callBackMap.find(i);
				if (it != _callBackMap.end())
					delete it->second;
			}

			free(_zmq_pollitem_cache);
		}

		int Reactor::Initialize(int maxEventNum)
		{
			if (_isInit)
				return -1;

			if(maxEventNum <= 0)
				return -2;

			_zmq_pollitem_cache = (zmq_pollitem_t*)malloc(maxEventNum * sizeof(zmq_pollitem_t));
			if (_zmq_pollitem_cache == NULL)
				return -3;

			_maxEventNum = maxEventNum;
			_isInit = true;
		}

		int Reactor::RegisterChannel(Channel* channel, IOHandler* handler)
		{
			if (!_isInit || channel == NULL)
				return -1;

			if (_size >= _maxEventNum)
				return -2;

			event_item* callback = new event_item();
			callback->_channel = channel;
			callback->_handler = handler;
			callback->_itemIndex = _size;

			_zmq_pollitem_cache[_size].socket = channel->GetRawSocket();
			_zmq_pollitem_cache[_size].fd = 0;
			_zmq_pollitem_cache[_size].events = ZMQ_POLLIN;
			_zmq_pollitem_cache[_size].revents = 0;

			_callBackMap.insert(std::pair<int, event_item*>(_size, callback));
			_size++;

			return 0;
		}

		int Reactor::UnRegisterChannel(const Channel* channel)
		{
			if (!_isInit || channel == NULL)
				return -1;
			
			event_item* callback = NULL;
			for (CallBackMap::iterator it = _callBackMap.begin(); it != _callBackMap.end(); ++it)
			{
				if (it->second->_channel == channel)
				{
					callback = it->second;
					_callBackMap.erase(it);
				}
			}

			if (callback == NULL)
				return -2;

			if (callback->_itemIndex < 0 || callback->_itemIndex > _size)
			{
				delete callback;
				return 0;
			}
		
			for (int i = callback->_itemIndex + 1; i < _size; i++)
			{
				_zmq_pollitem_cache[i - 1] = _zmq_pollitem_cache[i];
			}

			return 0;
		}

		int Reactor::EventLoop(int timeOut)
		{
			if (!_isInit)
				return -1;

			zerobus::zmqbind::Poll(_zmq_pollitem_cache, _size, timeOut);

			for (int i = 0; i < _size; i++)
			{
				if (_zmq_pollitem_cache[i].revents & ZMQ_POLLIN) {
					CallBackMap::iterator it = _callBackMap.find(i);
					
					//call back
					if (it != _callBackMap.end() && it->second != NULL)
					{
						event_item* callback = it->second;
						callback->_handler->OnReceiveMessage(callback->_channel);
					}
				}
			}
			
			return 0;
		}
	}
}

