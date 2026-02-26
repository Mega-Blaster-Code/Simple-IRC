#include "Msocket.h"

int McreateServerSocket(int domain, int type, int protocol){
	int fd;
	fd = socket(domain, type, protocol);
	if(fd == -1){
		perror("socket");
		return FAIL_CREATE_SERVER_SOCKET;
	}
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		perror("setsockopt");
		return FAIL_SETSOCKOPT;
	}
	return fd;
}

int McreateServerAddress(const char *ip_address, int port, struct sockaddr_in *address){
	memset(address, 0, sizeof(*address));

	address->sin_family = M_IPV4;
	address->sin_port = htons(port);

	if(inet_pton(M_IPV4, ip_address, &address->sin_addr) != 1){
		perror("inet_pton");
		return FAIL_INET_PTON;
	}
	return 0;
}

int McreateClientSocket(int domain, int type, int protocol, int *fd){
	*fd = socket(domain, type, protocol);
	if(*fd == -1){
		perror("socket");
		return FAIL_CREATE_CLIENT_SOCKET;
	}
	return 0;
}

int Mbind(int fd, struct sockaddr_in *addr){
	if(bind(fd, (struct sockaddr*) addr, sizeof(*addr)) == -1){
		return -1;
	}
	return 0;
}

int newServer(const char *ip, int port){
	int sockfd = McreateServerSocket(M_IPV4, M_TCP, 0);

	struct sockaddr_in address;

	int result = McreateServerAddress(ip, port, &address);

	if(result != 0) {
		return result;
	}

	if(Mbind(sockfd, &address)) {
		perror("bind");
		exit(-7);
	}

	if(listen(sockfd, 10) == -1) {
		perror("listen");
		exit(-8);
	}

	return sockfd;
}

int newClient(int *socketfd){
	int result = McreateClientSocket(M_IPV4, M_TCP, 0, socketfd);

	return result;
}

int MclientConnect(int socketfd, const char *ip_address, int port){
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));

	server_address.sin_family = M_IPV4;
	server_address.sin_port = htons(port);

	int pton_result = inet_pton(M_IPV4, ip_address, &server_address.sin_addr);
	if(pton_result < 0){
		perror("inet_pton");
		return pton_result;
	}else if(pton_result == 0){
		perror("inet_pton");
	}

	if(connect(socketfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
		perror("connect");
		close(socketfd);
		exit(-10);
	}

	printf("Connected to server! %s:%d\n", ip_address, port);

	return 0;
}

Mclient MgetServerNewClient(int fd){
	struct sockaddr_in sa;
	socklen_t sal = sizeof(sa);
	
	int connfd = accept(fd, (struct sockaddr*) &sa, &sal);

	if(connfd == -1){
		perror("accept");
		exit(-11);
	}

	Mclient client_sock;

	client_sock.connfd = connfd;

	client_sock.client_socket = sa;
	client_sock.client_len = sal;

	return client_sock;
}

int Mseek(int fd, int timeout_ms) {
	fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    return select(fd + 1, &readfds, NULL, NULL, &timeout);
}

int Mreceive(int fd, Mmessage *msg){
	msg->data_len = recv(fd, msg->data, msg->data_len, 0);

	if(msg->data_len == 0){
		free(msg->data);
		msg->data = NULL;
		return -1;
	}

	if(msg->data_len < 0){
		perror("recv");
		free(msg->data);
		msg->data = NULL;
		msg->data_len = -1;
		exit(-12);
		return -1;
	}

	msg->data[msg->data_len] = '\0';

	return 0;
}

int MreceiveAll(int fd, Mmessage *msg){
	size_t max_buffer_size = msg->data_len;
	char *buffer = SMsafeMalloc(4);
	size_t pointer = 0;
	size_t size = 4;
	while (1){
		int ready = Mseek(fd, 100);
		if(ready < 0){
			perror("select");
			break;
		}else if(ready == 0){
			break;
		}

		if(pointer >= size){
			size *= 2;
			if(size > max_buffer_size){
				free(buffer);
				return -1;
			}
			buffer = SMsafeRealloc(buffer, size);
		}

		Mmessage temp_msg = {
			.data = buffer + pointer,
			.data_len = size - pointer
		};

		int result = Mreceive(fd, &temp_msg);
		if(result){
			free(buffer);
			return result;
		}

		pointer += temp_msg.data_len;

	}
	
	msg->data = buffer;
	msg->data_len = pointer;

	printf("received: %zu = \"%s\"\n", msg->data_len, msg->data);

	return 0;
}

int Msend(int fd, Mmessage *msg){
    ssize_t total = 0;
    ssize_t sent;

    while (total < msg->data_len) {
        sent = send(fd, msg->data + total, msg->data_len - total, 0);
        if (sent <= 0) {
            perror("send");
            return -1;
        }
        total += sent;
    }

    return 0;
}