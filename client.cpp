#include <cstdlib>
#include <cstring>
#include <sys/poll.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main(){ 
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		std::cerr << "Failed to create socket" << endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addr;
	addr.sin_port = htons(8080);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;

	if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0){
		cerr << "Failed to connect to server" << endl;
		close(sock);
	}

		struct pollfd fds[2] = {
	{
		0,
		POLLIN,
		0
	},
	{
		sock,
		POLLIN,
		0
	}
	};


	for (;;) {
			
		char buffer[256] = { 0 };
		poll(fds, 2, -1);


		if (fds[0].revents & POLLIN) {
			memset(buffer, 0,sizeof(buffer));
			int bytes = read(0, buffer, 255);
			buffer[bytes - 1] = '\0';
			send(sock, buffer, bytes, 0);
			
		} else if (fds[1].revents & POLLIN) {
			memset(buffer, 0,sizeof(buffer));
			if (recv(sock, buffer, 255, 0) == 0){
				close(sock);
				return 0;
			}
			cout << buffer << endl;
		} 
	}
	
	close(sock);
	return 0;
}
