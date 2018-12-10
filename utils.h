/*****************************************************************************
*  TINY-PLUS-Compiler Utilities                                              *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     utils.h                                                         *
*  @brief    TPC编译器的辅助函数头文件                                          *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#ifndef TPC_UTILS_H
#define TPC_UTILS_H

void printToken(Token, const char *, FILE *);

TreeNode *newStmtNode(StmtKind);

TreeNode *newExpNode(ExpKind);

char *copyString(char *);

void printTree(TreeNode *);

int hash(char *);

void st_insert(char *name, int lineno, int loc);

int st_lookup(char *name);

void printSymTab(FILE *listing);

/**
 * @brief: Middle Code Generate
 */
/* pc = program counter  */
#define  pc 7

/* mp = "memory pointer" points
 * to top of memory (for temp storage)
 */
#define  mp 6

/* gp = "global pointer" points
 * to bottom of memory for (global)
 * variable storage
 */
#define gp 5

/* accumulator */
#define  ac 0

/* 2nd accumulator */
#define  ac1 1
void genComment(const char *);
void genRO(const char *op, int r, int s, int t, const char *c);
void genRM(const char *op, int r, int d, int s, const char *c);
int genSkip(int n);
void genBackup(int loc);
void genRestore(void);
void genRM_Abs(const char *op, int r, int a, const char *c);

#endif //TPC_UTILS_H
