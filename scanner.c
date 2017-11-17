#include "scanner.h"

enum State{
	start = 1,
	id,
	int_,
	double_,
	lesser,
	greater,
	ex_mark,
	string,
	escape_seq,
	ddd,
};

// funkce pro inicializaci Tokenu
int Init_Token(TOKEN *t){
	if((t->data = (char*)malloc(sizeof(INC_SIZE*sizeof(char)))) == NULL){
		return ALLOC_ERROR;
	}

	t->size = 0;
	t->data[t->size] = '\0';
	t->alloc_size = INC_SIZE;
	t->name = INVALIDCHAR;

	return OK;
}

// funkce pro pridani znaku pro dany Token
int Add_Char(TOKEN *t, char c){	
	if(t->size >= t->alloc_size-1){
		t->alloc_size = INC_SIZE + t->size;	
		if((t->data = (char*)realloc(t->data, t->alloc_size*sizeof(char))) == NULL){
			return ALLOC_ERROR;
		}	
	}

	// do dat ohledne Tokenu, se prida znak a zmeni se velikost dat + ukoncovaci znak
	t->data[t->size] = c;
	t->size++;
	t->data[t->size] = '\0';

	return OK;
}

// funkce vymaze data o Tokenu
void Clear_Token(TOKEN *t){
	t->size = 0;
   	t->data[0] = '\0';
   	t->name = INVALIDCHAR;
}

int KeywordCheck(char *string){
	if(strcasecmp("as",string) == 0) return AS;
	else if(strcasecmp("asc",string) == 0) return ASC;
	else if(strcasecmp("declare",string) == 0) return DECLARE;
	else if(strcasecmp("dim",string) == 0) return DIM;
	else if(strcasecmp("do",string) == 0) return DO;
	else if(strcasecmp("double",string) == 0) return DOUBLE;
	else if(strcasecmp("else",string) == 0) return ELSE;
	else if(strcasecmp("end",string) == 0) return END;
	else if(strcasecmp("function",string) == 0) return FUNCTION;
	else if(strcasecmp("if",string) == 0) return IF;
	else if(strcasecmp("input",string) == 0) return INPUT;
	else if(strcasecmp("integer",string) == 0) return INTEGER;
	else if(strcasecmp("lenght",string) == 0) return LENGTH;
	else if(strcasecmp("loop",string) == 0) return LOOP;
	else if(strcasecmp("print",string) == 0) return PRINT;	
	else if(strcasecmp("return",string) == 0) return RETURN;
	else if(strcasecmp("scope",string) == 0) return SCOPE;
	else if(strcasecmp("string",string) == 0) return STRING;
	else if(strcasecmp("substr",string) == 0) return SUBSTR;
	else if(strcasecmp("then",string) == 0) return THEN;
	else if(strcasecmp("while",string) == 0) return WHILE;
	else if(strcasecmp("and",string) == 0) return AND;
	else if(strcasecmp("boolean",string) == 0) return BOOLEAN_;
	else if(strcasecmp("continue",string) == 0) return CONTINUE;
	else if(strcasecmp("elseif",string) == 0) return ELSEIF;
	else if(strcasecmp("exit",string) == 0) return EXIT;
	else if(strcasecmp("false",string) == 0) return FALSE_;
	else if(strcasecmp("for",string) == 0) return FOR;
	else if(strcasecmp("next",string) == 0) return NEXT;
	else if(strcasecmp("not",string) == 0) return NOT;
	else if(strcasecmp("or",string) == 0) return OR;
	else if(strcasecmp("shared",string) == 0) return SHARED;
	else if(strcasecmp("static",string) == 0) return STATIC;
	else if(strcasecmp("true",string) == 0) return TRUE_;
	else 
		return 0;
}

int Get_Token(FILE *f,TOKEN *t){
	int c,nextc,pom,next_num = false,int_exp = false,count = 0;
	int state = start;

	Clear_Token(t);

	while((c = fgetc(f))){
		switch(state){
			case start: // odstrani whitespace
						if(isspace(c)){
							if(c == '\n'){
								if((pom = Add_Char(t,'E')) == ALLOC_ERROR){
									return ALLOC_ERROR;
								}
								if((pom = Add_Char(t,'O')) == ALLOC_ERROR){
									return ALLOC_ERROR;
								}
								if((pom = Add_Char(t,'L')) == ALLOC_ERROR){
									return ALLOC_ERROR;
								}
								t->name = EOL_;
								return OK;
							}
							state = start;
						}
						// odstrani jednoradkovy komentar
						else if(c == 39){
							// odebrani komentaru po konec radku nebo po EOF
							while (c != EOF && c != '\n') c=fgetc(f);
							// vraceni nechteneho znaku
							ungetc(c,f);
							state = start;
						}
						// odstraneni slozeneho komentare
						else if(c == '/'){
							nextc = fgetc(f);
							if(nextc == 39){
								nextc = fgetc(f);
								// odebrani komentare po konec radku nebo po EOF
								while (nextc != EOF){ 
									c = nextc;
									nextc = fgetc(f);
									if(c == 39){
										if(nextc == '/'){
											break;
										}
									}
									if(nextc == EOF){
										return LEX_A_ERROR;
									}
								}
							}
							else{
								// vraceni nechteneho znaku
								ungetc(nextc,f);
							}

						}
						// identifikator
						else if(isalpha(c) || c == '_'){							
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
							state = id;
						}
						// cislo
						else if(isdigit(c)){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
							state = int_;
						}
						// retezec
						else if(c == '!'){
							state = ex_mark;
						}
						// rovno, prirovnani, tri rovnitka
						else if(c == '='){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
							t->name = EQUAL;
							return OK;
						}
						// mensi nebo mensi rovno nez
						else if(c == '<'){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
						 	state = lesser;
						}
						// vetsi, vetsi ronvo nez
						else if(c == '>'){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
							state = greater;
						}
						else{
							switch (c){
								case '-' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = MINUS;
											return OK;
											break;

								case '+' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = PLUS;
											return OK;
											break;

								case '*' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = TIMES;
											return OK;
											break;

								case '/' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = DIVISION;
											return OK;
											break;

								case 92: if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = DIVISION_INT;
											return OK;
											break;

								case '(' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = LEFTPAREN;
											return OK;
											break;

								case ')' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = RIGHTPAREN;
											return OK;
											break;

								case '{' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = LEFTBRACE;
											return OK;
											break;

								case '}' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = RIGHTBRACE;
											return OK;
											break;

								case ';' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = SEMICOLON;
											return OK;
											break;

								case '.' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = DOT;
											return OK;
											break;

								case ',' : if((pom = Add_Char(t,c)) == ALLOC_ERROR){
												return ALLOC_ERROR;
											}
											t->name = COMMA;
											return OK;
											break;

								default: 
										t->name = INVALIDCHAR;
										return LEX_A_ERROR;
										break;
							}
						}break;
			// stav identifikator
			case id: 
						if((isdigit(c) || isalpha(c) || c == '_') && c != EOF){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
									return ALLOC_ERROR;
								}
								state = id;
						}
						else{
							if(c != EOF){
							ungetc(c,f);
							}
							if((pom = KeywordCheck(t->data)) != 0){
								t->name = pom;
								if(c == EOF) return EOF;
								return OK;
							}
							else{
								t->name = ID;
								if(c == EOF) return EOF;
								else return OK;
							}
							state = start;
						}break;
			// stav int
			case int_:
						if(next_num){
								if(isdigit(c)){
									if((pom = Add_Char(t,c)) == ALLOC_ERROR){
											return ALLOC_ERROR;
										}
									next_num = false;
								}
								else{
									ungetc(c,f);
									return LEX_A_ERROR;
								}
						}
						else{
							if(isdigit(c)){
								if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
							}
							else if(c == '.' && int_exp == false){
								if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
								state = double_;
								next_num = true;
							}
							else if(c == 'E' || c == 'e'){
								int_exp = true;
								if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
								c = fgetc(f);
								if(c == '+' || c == '-' || isdigit(c)){
									if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
									if(c == '+' || c == '-'){
										next_num = true;
									}
									state = int_exp;
								}
								else{
									ungetc(c,f);
									return LEX_A_ERROR;
								}
							}
							else{
								ungetc(c,f);
								t->name = INT_NUM;
								return OK;
							}
						}break;
			// stav double
			case double_:
						if(next_num){
							if(isdigit(c)){
								if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
								next_num = false;
							}
							else{
								ungetc(c,f);
								return LEX_A_ERROR;
							}
						}
						else{
							if(isdigit(c)){
								if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
							}
							else if(c == 'E' || c == 'e'){
								if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
								c = fgetc(f);
								if(c == '+' || c == '-' || isdigit(c)){
									if((pom = Add_Char(t,c)) == ALLOC_ERROR){
										return ALLOC_ERROR;
									}
									if(c == '+' || c == '-'){
										next_num = true;
									}
								}
								else{
									ungetc(c,f);
									return LEX_A_ERROR;
								}
							}
							else{
								ungetc(c,f);
								t->name = DOUBLE_NUM;
								return OK;
							}
						}break;
			// stav lesser
			case lesser:
						if(c == '='){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
							t->name = LESSEREQUAL;	
							return OK;
						}
						else if(c == '>'){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
							t->name = NOTEQUAL;
							return OK;
						}
						else{
							ungetc(c,f); 
							t->name = LESSER;
							return OK;
						}break;

			case greater:
						if(c == '='){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}
							t->name = GREATEREQUAL;	
							return OK;
						}
						else{
							ungetc(c,f); 
							t->name = GREATER;
							return OK;
						}break;

			case ex_mark: 
						if(c == '"'){
							state = string;
						}
						else{
							ungetc(c,f);
							return LEX_A_ERROR;
						}break;

			case string: 
						if(c == '"'){
							t->name = STR;
							return OK;
						}
						else{
							if(c > 31){
								if((pom = Add_Char(t,c)) == ALLOC_ERROR){
									return ALLOC_ERROR;
								}
							}
							else if(c == '\\'){
								state = escape_seq;
							}
						}break;

			case escape_seq:
						if(c == '"'){
							if((pom = Add_Char(t,'\"')) == ALLOC_ERROR){
								return ALLOC_ERROR;
								}
							state = string;
						}
						else if(c == 'n'){
							if((pom = Add_Char(t,'\n')) == ALLOC_ERROR){
								return ALLOC_ERROR;
								}
							state = string;
						}
						else if(c == 't'){
							if((pom = Add_Char(t,'\t')) == ALLOC_ERROR){
								return ALLOC_ERROR;
								}
							state = string;
						}
						else if(c == '\\'){
							if((pom = Add_Char(t,'\\')) == ALLOC_ERROR){
								return ALLOC_ERROR;
								}
							state = string;
						}
						else if(isdigit(c)){
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
								}
							state = ddd;	
						}break;

			case ddd:
						if(isdigit(c)){
							count++;
							if((pom = Add_Char(t,c)) == ALLOC_ERROR){
								return ALLOC_ERROR;
							}								
							if(count == 2){
								state = string;
							}
						}
						else{
							ungetc(c,f);
							return LEX_A_ERROR;
						}break;
		}
	}	
	return EOF;
}
