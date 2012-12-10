// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/10
// Version: 1.0

#ifndef zerobus_COMMONDEFINE_H__
#define zerobus_COMMONDEFINE_H__


#define NOTCOPYBLE(TYPE) \
	TYPE(const TYPE&);\
	void operator=(const TYPE&);\

namespace zerobus {
	namespace framework {
		const int MAX_URL_SIZE = 80;

		enum ChannelType {
			CHANNEL_DEALER2DEALER = 0,
		};

		struct ChannelID
		{
			int  _connector_id;
			int  _accpetor_id;

			bool operator <(const ChannelID &c) const
			{  
				if(_connector_id < c._connector_id)  
					return true;  

				if (_accpetor_id < c._accpetor_id)  
					return true;

				return false;
			} 
		};

		struct ChannelDef
		{
			char _type;
			int  _bufsize;
			ChannelID _channel_id;
			char _url[MAX_URL_SIZE];
		};

	}
}

#endif // zerobus_COMMONDEFINE_H__
