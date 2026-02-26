#include "IRC.h"

void key_typed_callback(void *_c){
	printf("%c", *((char*)_c));
	fflush(stdout);
}

IRC_USER *IRCnewUser(){
	IRC_USER *user = SMsafeMalloc(sizeof(IRC_USER));
	user->id = NULL;
	user->name = NULL;
	user->password = NULL;
	user->input = createMinput(64);
	user->prompt_size = 16;
	user->has_prompt = 0;
	user->prompt = SMsafeCalloc(1, user->prompt_size);
	setKeyCallback(user->input, key_typed_callback);
	return user;
}

int _IRCexecute(IRC_USER *U){
	if(!strcmp(U->prompt, "/exit")){
		return IRC_CLOSE_PROGRAM;
	}
	flushMinput(U->input);
	return IRC_SUCCESS;
}

int IRCupdateUser(IRC_USER *U){
	U->has_prompt = 0;
	updateMinput(U->input);
	int enter = U->input->enter;
	if(enter){
		if(U->input->data[0] == 0 || U->input->data[0] == 32 || U->input->length == 0){
			fcolor8(stdout, 255, 0, 0);
			printf("\nInvalid Input\n");
			rcolor8(stdout);
			flushMinput(U->input);
			return IRC_SUCCESS;
		}

		if(U->name == NULL){
			U->name = SMsafeMalloc(U->input->length + 1);
			memcpy(U->name, U->input->data, U->input->length);
			U->name[U->input->length] = 0;
			flushMinput(U->input);
			printf("\n");
			return IRC_SUCCESS;
		}else if(U->password == NULL){
			U->password = SMsafeMalloc(U->input->length + 1);
			memcpy(U->password, U->input->data, U->input->length);
			U->password[U->input->length] = 0;
			flushMinput(U->input);
			printf("\n");
			return IRC_TERMINATE_PROMPT;
		}else{
			U->has_prompt = 1;
			if (U->input->length + 1 > U->prompt_size) {
				U->prompt_size = U->input->length + 1;
				U->prompt = SMsafeRealloc(U->prompt, U->prompt_size);
			}
			
			memcpy(U->prompt, U->input->data, U->input->length);
			U->prompt[U->input->length] = 0;
			
			if(U->input->data[0] == '/'){
				return _IRCexecute(U);
			}
			flushMinput(U->input);
			return IRC_SUCCESS;
		}
	}
	return 0;
}

void IRCfree(IRC_USER *U){
	if(U->id != NULL){
		free(U->id);
	}
	if(U->name != NULL){
		free(U->name);
	}
	if(U->password != NULL){
		free(U->password);
	}
}