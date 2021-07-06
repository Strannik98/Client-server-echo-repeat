#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib,"Ws2_32.lib")

#include <stdio.h>

#include <winsock2.h>

#include <string>

#include <iostream>

using std::cout;

using std::cin;

using std::endl;

using std::string;

const int N = 256;

int main()

{

int retVal;

WSADATA wsaData;

WSAStartup(MAKEWORD(2, 2), (LPWSADATA)&wsaData);

LPHOSTENT hostEnt;

hostEnt = gethostbyname("");

if (!hostEnt) {

cout << "Unable to collect gethostbyname\n";

WSACleanup();

system("pause");

return SOCKET_ERROR;

}

SOCKET ClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

if (ClientSocket == SOCKET_ERROR) {

cout << "Unable to create socket\n";

WSACleanup();

system("pause");

return SOCKET_ERROR;

}

cout << "ip = 192.168.240.1\n";

string ip = "192.168.240.1";

SOCKADDR_IN ServerInfo;

ServerInfo.sin_family = PF_INET;

ServerInfo.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

ServerInfo.sin_port = htons(2010);

retVal = connect(ClientSocket, (LPSOCKADDR)&ServerInfo, sizeof(ServerInfo));

if (retVal == SOCKET_ERROR) {

cout << "Unable to connect\n";

WSACleanup();

system("pause");

return SOCKET_ERROR;

}

cout << "Connection made sucessfully\n";

cout << "Enter the text or 'exit' to shutdown server\n";

int i=1;

char text[N];

scanf("%c", &text[1]);

for (i = 2; text[i-1] != '\n'; i++) scanf("%c", &text[i]);

text[0] = i-2;

cout << "Sending request from client\n";

retVal = send(ClientSocket, text, strlen(text), 0);

if (retVal == SOCKET_ERROR) {

cout << "Unable to send\n";

WSACleanup();

system("pause");

return SOCKET_ERROR;

}

char szResponse[N];

retVal = recv(ClientSocket, szResponse, N, 0);

if (retVal == SOCKET_ERROR) {

cout << "Unable to recv\n";

WSACleanup();

system("pause");

return SOCKET_ERROR;

}

cout << szResponse << endl;

closesocket(ClientSocket);

WSACleanup();

system("pause");

return 0;

}
