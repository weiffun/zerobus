#include "zmqbind.hpp"

#include <stdio.h>
#include <string.h>

using namespace zerobus::zmqbind;

void print_error()
{
	int err = GetErrorCode();
	const char* pstr = zerobus::zmqbind::GetErrorStr(err);
	printf("error code: %d, error: %s\n", err, pstr);
}

int main()
{
	Context ctx;
	int ret = ctx.Init(0, 1);

	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	Socket socket;
	ret = socket.Init(ctx, SOCKET_TYPE_DEALER, "client");
	
	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	ret = socket.Connect("tcp://localhost:5561");
	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	zmq_pollitem_t items [] = { { socket.GetRawSocket(), 0, ZMQ_POLLIN, 0 } };

	int i = 10;
	while (i) {
		Poll(items, 1, 1000);

		Message msg;
		char str[12] = "hello world";
		msg.Init(12);

		memcpy(msg.Data(), str, sizeof(str));

		socket.Send(&msg, ZMQ_DONTWAIT);

		if (items [0].revents & ZMQ_POLLIN) {
			Message recv;
			recv.Init();

			int iRet = socket.Recv(&recv);

			if (iRet >= 0)
			{
// 				char str[12] = {0};
// 				memcpy(str, recv.Data(), iRet);
// 				str[12] = '\0';
				printf("server rep %d: iRet %d, %s\n", i, iRet, recv.Data());
			}
		}

		i--;
	}

	return 0;
}
