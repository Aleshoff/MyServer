#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

#pragma warning(disable: 4996)


class MyServer
{
private:
	int p;
	int g;
	int b;
	int key;
	std::vector<std::string> text;
	std::string alphabet;

	void calculateSecretKey(int openKey)
	{
		long long temp = pow(openKey, b);
		key = temp % p;
		std::cout << key << std::endl;

	}

	std::string  getOpenKey()
	{
		long long temp = pow(g, b);
		int openKey = temp % p;
		return std::to_string(openKey);
	}

	std::vector<std::string> uncrypt(std::string txt)
	{
		std::vector<std::string> vect;
		std::string textUncrypted = "";

		for (int i = 0; i < txt.size(); ++i)
		{
			
			if (txt[i] == ' ')
			{
				textUncrypted += ' ';
				continue;
			}
			if (txt[i] == '\n')
			{
				vect.push_back(textUncrypted);
				textUncrypted = "";
				continue;
			}
			int index = alphabet.find(txt[i]);
			if (index + key < alphabet.size())
				textUncrypted += alphabet[index + key];
			else
				textUncrypted += alphabet[(key + index) % alphabet.size()];
		}
		
		return vect;
	}

public:
	MyServer()
	{
		this->p = 33;
		this->g = 5;
		srand(time(NULL));		
		this->b = rand() % 10;
		this->alphabet = { "abcdefghijklmnopqrstuvwxyz,. " };
	}

	void start()
	{
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
		bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
		listen(sListen, SOMAXCONN);

		SOCKET newConnection;
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			std::string ok = getOpenKey();
			char buffer[256];
			strcpy(buffer, ok.c_str());
			send(newConnection, buffer, sizeof(buffer), NULL);
			char msg[256];
			recv(newConnection, msg, sizeof(msg), NULL);
			std::cout << msg << std::endl;
			calculateSecretKey(std::stoi(msg));
			char msg2[16384];
			recv(newConnection, msg2, sizeof(msg2), NULL);
			std::cout << "\nMesajul primit:\n\n" << msg2 << std::endl;
			text = uncrypt(msg2);
		}
		system("pause");
	}
	void print()
	{
		std::cout << "\nMesajul decriptat:\n" << std::endl;
		for (int i = 0; i < text.size(); ++i)
		{
			std::cout << text[i] << std::endl;
		}
	}
	
};

void main(int argc, char* argv[]) {

	MyServer server;
	server.start();
	server.print();
}