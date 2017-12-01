/**
 * @Name IFJ17 Syntakticka a sematicka analyza
 * @author Tomas Lapsansky (xlapsa00)
 * @file parser.h
 * @date 15.11.2017
 */

#include "scanner.h"
#include "structs.h"
#include "symtable.h"
#include "psa.h" 
#include "tstack.h"

int declare(TOKEN idToken);

//KA pre syntakticku analyzu
int parser(void);

int p_start(void);
int p_scope(void);
int p_define(void);
int p_body(int return_type);

int p_type(void);

int p_parameter(tRetData *funcData, int *pocet_parametrov);
int p_nextparameter(tRetData *funcData, int *pocet_parametrov);

int p_declare_parameter(char* funcName);
int p_declare_nextparameter(char* funcName);

int p_vparameter(tRetData *funcData, int *pocet_parametrov);
int p_vnextparameter(tRetData *funcData, int *pocet_parametrov);

int p_prikaz(int return_type);
int p_priradenie(int type);
int p_print(void);
int p_nextprint(void);
