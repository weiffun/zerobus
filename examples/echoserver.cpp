#include "zmqbind.hpp"

#include <stdio.h>
#include <string.h>

using namespace zerobus::zmqbind;

void print_error()
{
	int err = GetErrorCode();
	printf("error code: %d, error: %s\n", err, GetErrorStr(err));
}

int main()
{
	Context ctx;
	int ret = ctx.Init(0, 1000);

	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	Socket socket;
	ret = socket.Init(ctx, SOCKET_TYPE_ROUTER);
	
	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	ret = socket.Bind("tcp://*:5561");
	if (ret != 0)
	{
		printf("ret %d\n", ret);
		print_error();
		return -1;
	}

	zmq_pollitem_t items [] = { { socket.GetRawSocket(), 0, ZMQ_POLLIN, 0 } };

	while (1) {
		Poll(items, 1, 1);

		if (items [0].revents & ZMQ_POLLIN) {
			Message addr;
			addr.Init();

// 			int iRet = socket.Recv(&addr);
// 
// 			if (iRet >= 0)
// 			{
// 				printf("client addr: %s, iRet %d\n", addr.Data(), iRet);
// 
// 				Message recv;
// 				recv.Init();
// 
// 				int iRet = socket.Recv(&recv);
// 
// 				if (iRet >= 0)
// 				{
// 					printf("client data: %s, iRet %d\n", recv.Data(), iRet);
// 					// 				Message msg;
// 					// 				char str[7] = "client";
// 					// 				msg.Init(6);
// 					// 
// 					// 				memcpy(msg.Data(), str, sizeof(str));
// 					// 
// 					socket.Send(&addr, ZMQ_SNDMORE);
// 					socket.Send(&recv);
// 				}
// 			}
		}
	}

	return 0;
}
