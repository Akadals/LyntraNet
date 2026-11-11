#define LT_USE_CONSOLE_CAPTURE

#include <iostream>
#include <LT/LyntraNet.h>
#pragma comment(lib,"ws2_32.lib")

using namespace LT;

int main()
{
	LyntraServer server(IOBackendType::IOCP);
	Listener<ProtocolType::TCP> listener(IPEndPoint(IPAddress::AnyIPv4(), 8000));

	server << listener;


	server.Start();
	std::cout << "Successfully started\n";

	while (true) Sleep(1);
}