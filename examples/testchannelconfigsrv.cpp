
#include "Channel.hpp"
#include "ZBus.hpp"
#include "Error.hpp"

#include <stdio.h>
#include <string.h>

using namespace zerobus::zmqbind;
using namespace zerobus::framework;

void print_error()
{
	int err = zerobus::framework::GetErrorCode();
	const char* pstr = zerobus::framework::GetErrorStr(err);
	printf("error code: %d, error: %s\n", err, pstr);
}

int main()
{
	//load config
	int ret = ZBus::Instance().LoadConfig("channel_temp.xml");

	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	//init channels
	ret = ZBus::Instance().InitChannels(10001);

	if (ret != 0)
	{
		printf("init channel failed\n");
		print_error();
		return -1;
	}

	Channel* pchannel = ZBus::Instance().GetChannel(20001, 10001);

	if (!pchannel)
	{
		printf("get channel failed\n");
		print_error();
		return -1;
	}

	void* socket = pchannel->GetRawSocket();

	if (!socket)
	{
		printf("local peer nulll\n");
		return -1;
	}

	zmq_pollitem_t items [] = { { socket, 0, ZMQ_POLLIN, 0 } };

	while (1) {
		Poll(items, 1, 1000);

		if (items [0].revents & ZMQ_POLLIN) {

			char data[12] = {0};
			int len = 0;
			ret = pchannel->Recv(data, len);

			if (len > 0)
			{
				printf("recive client  %s\n",  data);
				pchannel->Send(data, len);
			}
		}
	}

	return 0;
}
