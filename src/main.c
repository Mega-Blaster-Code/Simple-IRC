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
		int result = IRCupdateUser(me);
		if(me->has_prompt){
			if(me->connected){
				printf("\n<%s>:%s\n", me->name, me->prompt);
				fflush(stdout);
			}
		}
		if(result == IRC_CLOSE_PROGRAM){
			return 0;
		}else if(result == IRC_TERMINATE_PROMPT){
			break;
		}else if (result == IRC_CUSTOM_COMMAND) {
			if (me->prompt_pointer < 5) {
				IRCinvalidInput(me);
				continue;
			}

			if (strncmp(me->prompt, "/:ip ", 5) == 0) {

				size_t ip_len = me->prompt_pointer - 5;

				if (ip_len == 0 || ip_len >= 16) {
					IRCinvalidInput(me);
					continue;
				}

				memcpy(me->ip, me->prompt + 5, ip_len);
				me->ip[ip_len] = 0;
				fcolor8(stdout, 150, 255, 255);
				printf("\nIP ");
				fcolor8(stdout, 50, 255, 255);
				printf("\"%s\"\n", me->ip);
				rcolor8(stdout);
				continue;
			}

			if (strncmp(me->prompt, "/:break", 7) == 0) {
				break;
			}

			IRCinvalidInput(me);
		}
	}

	printf("\n");

    KdisableRawMode();

	int port = 9091;

	int socketfd = 0;
	int newc_result = newClient(&socketfd);
	if(newc_result < 0){
		return newc_result;
	}

	MclientConnect(socketfd, me->ip, port);

	me->connected = 1;

	Mmessage msg = {
		.data_len = 16
	};

	int result = MreceiveAll(socketfd, &msg);

	if(result){
		return result;
	}

	printf("received: %zu = \"%s\"\n", msg.data_len, msg.data);

	Mmessage my_name = {
		.data = me->name,
		.data_len = strlen(me->name)
	};

	Mmessage my_password = {
		.data = me->password,
		.data_len = strlen(me->password)
	};

	Msend(socketfd, &my_name);
	Msend(socketfd, &my_password);

	IRCfree(me);

    return 0;
}
