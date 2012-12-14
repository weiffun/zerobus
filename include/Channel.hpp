// Copyright (c) 2012,  Reve Zhang.
// All rights reserved.
//
// Author: Reve Zhang <weiffun@gmail.com>
// Created: 2012/12/07
// Version: 1.0

#ifndef NETBUS_CHANNEL_H__
#define NETBUS_CHANNEL_H__

#include "CommonDefine.hpp"

#include "zmqbind/zmqbind.hpp"

namespace zerobus {
	namespace framework {

		class IPeer;

		/**
		* Channel is a base class which abstract peers' communication.
		*/
		class Channel 
		{
		public:
			
			/**
			* A constructor.
			*/
			Channel();

			/**
			* A constructor.
			*/
			Channel(int connector, int acceptor, int buf_size,
				const char* url);

			/**
			* A destructor.
			*/
			virtual ~Channel();

			/**
			* Initialize a channel, bind current peer or connect to remote peer. 
			* @param ctx reference to the zeromq context.
			* @param id endpoint peer id of a channel.
			* @param time_out time out(millisecond) of init the channel, -1 wait for ever.
			* @return success return 0, else return -1.
			*/
			virtual int InitChannel(zerobus::zmqbind::Context& ctx, int id, int time_out) = 0;

			/**
			* Delete current channel, then initialize a channel. 
			* @see InitChannel().
			*/
			virtual int ReInitChannel(zerobus::zmqbind::Context& ctx, int id, int time_out);

			/**
			* Set channel info. 
			* @param connector Connector peer id.
			* @param acceptor Connector peer id.
			* @param buf_size  Maxinum number of message 
			  can be processed in the channel's message queue.
		    * @param url Acceptor bind url.
			* @return Success return 0, else return -1.
			*/
			int SetChannel(int connector, int acceptor, int buf_size,
				const char* url);

			/**
			* Send a message on channel, it is asynchronous.
			* @param pmsg The message to send.
			* @param len The message length.
			* @return Success return send num, else return -1.
			*/
			int Send(const void* pmsg, int len);

			/**
			* Send a message to specified channel, it is asynchronous.
			* @param peer Point to the channel.
			* @param pmsg The message to send.
			* @param len The message length.
			* @return Success return send num, else return -1.
			*/
			int SendToChannel(Channel* peer, const void* pmsg, int len);

			/**
			* Recv a message on channel, it is asynchronous.
			* @param pmsg The message buffer to recv.
			* @param len The message buffer's max length, success recv it will be assigned to
				the received bytes num.
			* @return Success return recv num, else return -1.
			*/
			int Recv(void* pmsg, int& len);

			/**
			* Get the raw socket, should only used in Poll. 
			* @return Success return the raw socket, else return NULL.
			*/
			void* GetRawSocket();

			/**
			* Send keep alive heart beat message. 
			* @return Success return 0, else return -1.
			*/
			int KeepAlive();

			/**
			* Check the channel status. 
			* @return Initialize return true, else return false.
			*/
			bool IsInitized();

		protected:
			/**
			* Initialize the local peer socket. 
			* @param ctx reference to the zeromq context.
			* @return Success return 0, else return -1.
			*/
			int InitLocalPeer(zerobus::zmqbind::Context& ctx);

		protected:
			int _connnector_id;  
			int _acceptor_id;
			int _buf_size;
			IPeer* _local_peer;
			char _url[MAX_URL_SIZE];

		private:
			NOTCOPYBLE(Channel)
		};

		/**
		* Channel Factory
		*/
		class ChannelFactory
		{
		public:
			/**
			* Instance a Factory Object. 
			* @return ChannelFactory object reference.
			*/
			static ChannelFactory& Instance();

			/**
			* Get specified type of channel. 
			* @param type Channel type.
			* @see ChannelType.
			* @return Success return a Channel Object pointer, else return NULL.
			*/
			Channel* GetChannel(int type);
			
		private:
			ChannelFactory() { };
			NOTCOPYBLE(ChannelFactory)
		};
	}
}
#endif // NETBUS_CHANNEL_H__
