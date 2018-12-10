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

#endif //TPC_UTILS_H
