#include "minput.h"

void setKeyCallback(InputBuffer *I, callback func){
	I->func = func;
}

InputBuffer *createMinput(size_t size){
	InputBuffer *I = SMsafeCalloc(1, sizeof(InputBuffer));
	I->capacity = size;
	I->cursor = 0;
	I->length = 0;
	I->special_key = 0;

	I->data = SMsafeCalloc(1, I->capacity);

	I->func = NULL;

	return I;
}

void _callMinputCallback(callback func, void *_c){
	if(func != NULL){
		func(_c);
	}
}

void updateMinput(InputBuffer *I){
	I->enter = 0;
	int key = KgetKey();
	if (!key) return;

	if(I->special_key){
		
		if(key == 91){
			I->special_key = 91;
			return;
		}else if(I->special_key == 91){
			if(key == 68){
				I->special_key = 0;
				if(I->cursor > 0){
					I->cursor--;
					char _c = '\x1B';
					_callMinputCallback(I->func, (void*)&_c);
					_c = '[';
					_callMinputCallback(I->func, (void*)&_c);
					_c = 'D';
					_callMinputCallback(I->func, (void*)&_c);
				}
				return;
			}else if(key == 67){
				I->special_key = 0;
				if(I->cursor < I->length){
					I->cursor++;
					char _c = '\x1B';
					_callMinputCallback(I->func, (void*)&_c);
					_c = '[';
					_callMinputCallback(I->func, (void*)&_c);
					_c = 'C';
					_callMinputCallback(I->func, (void*)&_c);
				}
				return;
			}else if(key == 65){
				I->special_key = 0;
				//char _c = '\x1B';
				//_callMinputCallback(I->func, (void*)&_c);
				//_c = '[';
				//_callMinputCallback(I->func, (void*)&_c);
				//_c = 'A';
				//_callMinputCallback(I->func, (void*)&_c);
				return;
			}
			I->special_key = 0;
		}
		I->special_key = 0;
	}

	if (key == 127 || key == 8) {
		if (I->cursor > 0 && I->length > 0) {
			I->cursor--;
			I->length--;
			char _c = '\b';
			_callMinputCallback(I->func, (void*)&_c);
			_c = ' ';
			_callMinputCallback(I->func, (void*)&_c);
			_c = '\b';
			_callMinputCallback(I->func, (void*)&_c);
		}
		return;
	}

	if (key == 27) {
		I->special_key = 27;
		return;
	}

	if(key == '\n'){
		I->enter = 1;
	}

	if (key > 31 && key <= 126) {
		if(I->length >= I->capacity){
			I->capacity*=2;
			I->data = SMsafeRealloc(I->data, I->capacity);
		}

		I->data[I->cursor] = key;
		
		if(I->cursor >= I->length){
			I->length++;
			I->cursor = I->length;
		}else{
			I->cursor++;
		}
		
		I->data[I->cursor] = 0;

		char _c = key;
		_callMinputCallback(I->func, (void*)&_c);
		fflush(stdout);
	}
}

void flushMinput(InputBuffer *I){
	I->length = 0;
	I->cursor = 0;
	I->data[0] = 0;
}

void freeMinput(InputBuffer *I){
	free(I->data);
	free(I);
}