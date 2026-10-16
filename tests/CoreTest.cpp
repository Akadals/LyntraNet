#define LT_USE_CONSOLE_CAPTURE

#include <iostream>
#include <LT/LyntraNet.h>
#pragma comment(lib,"ws2_32.lib")

using namespace LT;

int main()
{
	LyntraServer server(IOBackendType::IOCP);
	
	server.AddListener<TCP>(IPEndPoint::AnyIPv4(), 9000);
	server.AddListener<UDP>(IPEndPoint::AnyIPv4(), 9001);

	server.Start();
	std::cout << "Successfully started";
}