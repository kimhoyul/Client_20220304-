#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket; //ip와 port를 프로그램에서 사용하기 위해서 매칭
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerAddr;
	memset(&ServerAddr, 0, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(50000);

	string Command;
	string Filename;
	cout << "Command ? " << endl;
	cin >> Command;

	cout << "Filename ? " << endl;
	cin >> Filename;

	string Packet = Command + " " + Filename + "\n";

	connect(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

	int SendLength = send(ServerSocket, Packet.c_str(), Packet.length(), 0);

	FILE* fp = fopen(Filename.c_str(), "rb");
	char Buffer[2049] = { 0, };
	int ReadLength = fread(Buffer, sizeof(char), 2048, fp);
	while (ReadLength != 0)
	{
		send(ServerSocket, Buffer, ReadLength, 0);
		ReadLength = fread(Buffer, sizeof(char), 2048, fp);
	}

	fclose(fp);
	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}