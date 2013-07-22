#include "Reactor.hpp"

namespace zerobus 
{
	namespace framework 
	{
		Reactor::Reactor() : _isInit(false), _size(0)
		{

		}

		Reactor::~Reactor()
		{

		}

		int Reactor::Initialize(int maxEventNum)
		{
			if (_isInit)
				return -1;

			if(maxEventNum <= 0)
				return -2;

			_zmq_pollitem_cache = malloc(maxEventNum * sizeof(zmq_pollitem_t));
			if (_zmq_pollitem_cache == NULL)
				return -3;

			_maxEventNum = maxEventNum;
			_isInit = true;
		}

		int Reactor::RegisterChannel(const Channel* channel, const IOHandler *handler)
		{
			if (_size >= _maxEventNum)
				return -1;

			event_item* callback = new event_item();
			callback->_channel = channel;
			callback->_handler = handler;
			zmq_pollitem_t* item = &_zmq_pollitem_cache[_size];
			item->socket = channel->GetRawSocket();
			item->fd = 0;
			item->events = ZMQ_POLLIN;
			item->revents = 0;
			map.insert(item, callback);
			_size++;
			return 0;
		}
	}
}