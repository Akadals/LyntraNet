#define LT_USE_CONSOLE_CAPTURE

#include <iostream>
#include <LT/LyntraNet.h>
#pragma comment(lib,"ws2_32.lib")

using namespace LT;

int main()
{
	LyntraServer server(IOBackendType::IOCP);
	
	server.AddListener<ProtocolType::TCP>(IPAddress::AnyIPv4(), 9000);

	server.Start();
	std::cout << "Successfully started\n";

	while (true) Sleep(1);
}