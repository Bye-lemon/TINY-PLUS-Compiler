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

// Scanner Definition
typedef enum {
    ENDFILE, ERROR, ID, NUM,
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    PLUS, MINUS, TIMES, OVER, EQ, NOTEQ, LT, LTEQ, GT, GTEQ, LPAREN, RPAREN, SEMI, ASSIGN
} Token;
#define RESERVED_WORD_NUM 8
// End Scanner Definition

// Main Function Definition
extern int thisLine;
extern FILE *source;
extern FILE *oscan;
extern FILE *oparse;

extern int Error;
extern int scanError;
// End Main Function Definition

// Parser Definition
typedef enum {
    StmtK, ExpK
} NodeKind;
typedef enum {
    IfK, RepeatK, AssignK, ReadK, WriteK
} StmtKind;
typedef enum {
    OpK, ConstK, IdK
} ExpKind;

/* ExpType is used for type checking */
typedef enum {
    Void, Integer, Boolean
} ExpType;

#define MAX_CHILDREN_NUM 3

typedef struct treeNode {
    struct treeNode *child[MAX_CHILDREN_NUM];
    struct treeNode *sibling;
    int thisLine;
    NodeKind nodekind;
    struct {
        StmtKind stmt;
        ExpKind exp;
    } kind;
    struct {
        Token op;
        int val;
        char *name;
    } attr;
    ExpType type; /* for type checking of exps */
} TreeNode;
// End Parser Definition

#endif //TPC_GLOBALS_H
