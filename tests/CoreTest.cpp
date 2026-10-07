#include <iostream>
#include <LT/LyntraNet.h>

using namespace LT;

int main()
{
	LT::LyntraServer server;

	server.AddListener<TCP>(IPAddress::AnyPort());

	server.Start();
	server.Wait();
}