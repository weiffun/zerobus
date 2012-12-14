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
		 exit(1);
	}

	//init channels
	ret = ZBus::Instance().InitChannels(20001);

	if (ret != 0)
	{
		printf("init channel failed\n");
		print_error();
		exit(1);
	}

	Channel* pchannel = ZBus::Instance().GetChannel(20001, 10001);

	if (!pchannel)
	{
		printf("get channel failed\n");
		print_error();
		 exit(1);
	}

	void* socket = pchannel->GetRawSocket();

	if (!socket)
	{
		printf("local peer nulll\n");
		return -1;
	}

	zmq_pollitem_t items [] = { { socket, 0, ZMQ_POLLIN, 0 } };

	int i = 10;
	while (i) {
		Poll(items, 1, 1000);

		char str[12] = "hello world";
		pchannel->Send(str, sizeof(str));

		if (items [0].revents & ZMQ_POLLIN) {
		
			char data[12] = {0};
			int len = 0;
			int iRet = pchannel->Recv((void *)data, len);

			if (len > 0)
			{
				printf("server rep %d: iRet %d, %s\n", i, iRet, data);
			}
		}

		i--;
	}

	return 0;
}
