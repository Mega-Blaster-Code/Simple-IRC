#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "color8.h"
#include "safema.h"
#include "filereader.h"
#include "Msocket.h"
#include "terminal.h"
#include "mkeys.h"
#include "minput.h"
#include "IRC.h"

int main() {
	KenableRawMode();

    printf("Press keys (q to quit)\n");

	IRC_USER *me = IRCnewUser();

	while (1) {
		if(IRCupdateUser(me)){
			break;
		}
	}

	printf("\nuser_name = \"%s\"\n", me->name);
	printf("user_password = \"%s\"\n", me->password);

	while (1) {
		int terminate = IRCupdateUser(me);
		if(me->has_prompt){
			printf("\n<%s>:%s\n", me->name, me->prompt);
			fflush(stdout);
		}
		if(terminate){
			break;
		}
	}

	printf("\n");
	
	IRCfree(me);

    KdisableRawMode();

	return 0;

	const char *ip = "172.28.202.36";
	int port = 9091;

	int socketfd = 0;
	int newc_result = newClient(&socketfd);
	if(newc_result < 0){
		return newc_result;
	}

	MclientConnect(socketfd, ip, port);

	Mmessage msg = {
		.data_len = 16
	};

	int result = MreceiveAll(socketfd, &msg);

	if(result){
		return result;
	}

	printf("received: %zu = \"%s\"\n", msg.data_len, msg.data);
	
	char *message = "This is a message";
	Mmessage my_msg = {
		.data = message,
		.data_len = strlen(message)
	};

	Msend(socketfd, &my_msg);

    return 0;
}
