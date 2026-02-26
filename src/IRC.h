#ifndef __M_IRC_H
#define __M_IRC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "safema.h"
#include "color8.h"
#include "mkeys.h"
#include "minput.h"

#define IRC_SUCCESS 0
#define IRC_TERMINATE_PROMPT 1
#define IRC_CLOSE_PROGRAM 2

typedef struct {
	InputBuffer *input;
	char *name;
	char *password;
	char *id;
	char *prompt;
	int has_prompt;
	size_t prompt_size;
} IRC_USER;

void key_typed_callback(void *_c);

IRC_USER *IRCnewUser();

int IRCupdateUser(IRC_USER *U);

void IRCfree(IRC_USER *U);

#endif // __M_IRC_H