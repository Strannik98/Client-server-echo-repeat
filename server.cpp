#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment (lib,"Ws2_32.lib")

#include <winsock2.h>
#include <iostream>

using std::cout;
using std::endl;

const int N = 256;

int main() {
int retVal;
  
WSADATA wsaData;
  
WSAStartup(MAKEWORD(2, 2), &wsaData);

SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

if (ListenSocket == INVALID_SOCKET) {
cout << "Unable to make socket\n";
WSACleanup();
system("pause");
return SOCKET_ERROR;
}

SOCKADDR_IN ServerAddress;

ServerAddress.sin_family = PF_INET;

ServerAddress.sin_port = htons(2010);

ServerAddress.sin_addr.s_addr = INADDR_ANY;

retVal = bind(ListenSocket, (LPSOCKADDR)&ServerAddress, sizeof(ServerAddress));

if (retVal == SOCKET_ERROR) {
printf("Unable to bind\n");
WSACleanup();
system("pause");
return SOCKET_ERROR;
}

char host[N];

char HostName[1024];

if (!gethostname(HostName, 1024)) {
  if (LPHOSTENT lphost = gethostbyname(HostName))
  strcpy_s(host, inet_ntoa(*((in_addr*)lphost->h_addr_list[0])));
}

cout << "Server started at " << host << ", port " << htons(ServerAddress.sin_port) << endl;

while (true) {
  retVal = listen(ListenSocket, 10);
  if (retVal == SOCKET_ERROR) {
  cout << "Unable to listen\n";
  WSACleanup();
  system("pause");
  return SOCKET_ERROR;
}

SOCKET ClientSocket;

SOCKADDR_IN connection;

int ConnectionLength = sizeof(connection);

ClientSocket = accept(ListenSocket, (struct sockaddr*)&connection, &ConnectionLength);

if (ClientSocket == INVALID_SOCKET) {
  cout << "Unable to accept\n";
  WSACleanup();
  system("pause");
  return SOCKET_ERROR;
}

cout << "New connection accepted from " << inet_ntoa(connection.sin_addr)

<< ", port " << htons(connection.sin_port) << endl;

char text[N];

retVal = recv(ClientSocket, text, N, 0);

if (retVal == SOCKET_ERROR)
{
  WSACleanup();
  cout << "Unable to recv\n";
  system("pause");
  return SOCKET_ERROR;
}

if (strncmp(text + 1, "exit", text[0]) == 0)
{
  char ShutMessage[] = "Server shutdown";
  retVal = send(ClientSocket, ShutMessage, 256, 0);
  closesocket(ClientSocket);
  break;
}

int i = 1, j = 0, num = 0;

char newText[N];

while (i <= text[0])
{
  if (text[i] == '.')
  {
    newText[j] = '.';
    i++;
    j++;
    num++;
    if(num <= 9) newText[j] = num + 48;
  else
  {
    int num2 = num % 10;
    num /= 10; newText[j] = num + 48;
    j++;
    newText[j] = num2+48;
  }
  j++;
  num = 0;
}

else
{
  newText[j] = text[i];
  num++;
  i++;
  j++;
}

}

j++;

newText[j] = '\0';

cout << "Data received\n";

cout << "Sending response from server\n";

retVal = send(ClientSocket, newText, N, 0);

if (retVal == SOCKET_ERROR)
{
  cout << "Unable to send\n";
  system("pause");
  return SOCKET_ERROR;
}

closesocket(ClientSocket);

cout << "Connection closed\n";

}

closesocket(ListenSocket);

WSACleanup();

return 0;

} 
