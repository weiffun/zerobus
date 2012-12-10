
#include "Channel.hpp"
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
	Context ctx;
	int ret = ctx.Init(0, 10);

	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	Dealer2DealerChannel channel;

	channel.SetChannel(2001, 10001, 1000, "tcp://*:5561");
	ret = channel.InitChannel(ctx, 10001, 10000);

	if (ret != 0)
	{
		printf("bind failed\n");
		print_error();
		return -1;
	}

	void* socket = channel.GetRawSocket();

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
			ret = channel.Recv(data, len);

			if (len > 0)
			{
				printf("recive client  %s\n",  data);
				channel.Send(data, len);
			}
		}
	}

	return 0;
}
