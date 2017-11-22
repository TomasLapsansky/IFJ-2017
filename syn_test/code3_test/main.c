#include "../../scanner.h"
#include "../../parser.h"


FILE* f;
TOKEN token;
int error;		//error code
int line;		//line number

int main(void) {
	Init_Token(&token);
	
	f = fopen("../../code_test/code3.txt","r");
	if (f == NULL){
		printf("Opening file code3 - ERROR \n");
		return 1;
	}
	
	int finish = parser();
	
	if(finish == OK) {
		printf("CODE3: OK\n");
	} else if(finish == SYN_A_ERROR) {
		printf("CODE3: SYN_A_ERROR line = %d token = %d\n", line, token.name);
	} else if(finish == LEX_A_ERROR) {
		printf("CODE3: LEX_A_ERROR line = %d token = %d\n", line, token.name);
	}
	
	fclose(f);
	
	Clear_Token(&token);
	return 0;
}
