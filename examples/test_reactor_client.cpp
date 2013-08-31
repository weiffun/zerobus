#include "Channel.hpp"
#include "ZBus.hpp"
#include "Error.hpp"
#include "Reactor.hpp"

#include <cstdio>
#include <cstring>

using namespace zerobus::zmqbind;
using namespace zerobus::framework;

void print_error()
{
	int err = zerobus::framework::GetErrorCode();
	const char* pstr = zerobus::framework::GetErrorStr(err);
	printf("error code: %d, error: %s\n", err, pstr);
}

class ReactorHandler: public IOHandler
{
public:
	int OnReceiveMessage(Channel* channel)
	{
		memset(_data, 0, sizeof(_data));

		if (channel != NULL)
		{
			int len = sizeof(_data);
			int ret = channel->Recv(_data, len);
			if (ret > 0)
			{
				printf("server resp : iRet %d, %s\n", ret, _data);
			}
			return 0;
		}

		return -1;
	}

private:
	char _data[1024*1024];
};

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

	Reactor reactor;
	ReactorHandler handler;

	reactor.Initialize(1);
	ret = reactor.RegisterChannel(pchannel, &handler);

	if (ret < 0)
	{
		printf("register channel failed\n");
		return -1;
	}

	int i = 10;
	while (i) {
		reactor.EventLoop(1000);

		char str[12] = "hello world";
		pchannel->Send(str, sizeof(str));

		i--;
	}

	return 0;
}
