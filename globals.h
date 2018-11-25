//
// Created by dell on 2018/11/10.
//

#ifndef TPC_GLOBALS_H
#define TPC_GLOBALS_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#define TRUE 1
#define FALSE 0

typedef enum {
    ENDFILE, ERROR, ID, NUM,
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    PLUS, MINUS, TIMES, OVER, EQ, NOTEQ, LT, LTEQ, GT, GTEQ, LPAREN, RPAREN, SEMI, ASSIGN
} Token;
#define RESERVED_WORD_NUM 8

extern int thisLine;
extern FILE *source;
extern FILE *oscan;

extern int scanError;

#endif //TPC_GLOBALS_H
