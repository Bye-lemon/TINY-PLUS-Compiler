/*****************************************************************************
*  TINY-PLUS-Compiler Global Header                                          *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     globals.h                                                       *
*  @brief    TPC编译器的全局参数头文件                                          *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#ifndef TPC_GLOBALS_H
#define TPC_GLOBALS_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#define TRUE 1
#define FALSE 0

/**
 * @brief Token列表
 */
typedef enum {
    ENDFILE, ERROR, ID, NUM,
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    PLUS, MINUS, TIMES, OVER, EQ, NOTEQ, LT, LTEQ, GT, GTEQ, LPAREN, RPAREN, SEMI, ASSIGN
} Token;

/**
 * @brief 定义的保留字的个数
 * @details IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE
 */
#define RESERVED_WORD_NUM 8

/** 当前被处理的行号 */
extern int thisLine;

/** 源文件流 */
extern FILE *source;
/** 词法分析结果输出文件流 */
extern FILE *oscan;
/** 语法分析结果输出文件流 */
extern FILE *oparse;
/** 语义分析结果输出文件流 */
extern FILE *oanalyze;
/** 中间代码输出文件流 */
extern FILE *code;
/** 标志变量，是否有扫描错误 */
extern int Error;
/** 编译错误数量 */
extern int scanError;

typedef enum {
    StmtK, ExpK
} NodeKind;
typedef enum {
    IfK, RepeatK, AssignK, ReadK, WriteK
} StmtKind;
typedef enum {
    OpK, ConstK, IdK
} ExpKind;
typedef enum {
    Void, Integer, Boolean
} ExpType;

/**
 * @brief 定义语法树最大子树个数
 * @details 一个节点最多拥有3个子节点，IF语句有一个表达式，一个真出口，一个假出口。
 */
#define MAX_CHILDREN_NUM 3

/**
 * @brief 语法树定义
 */
typedef struct treeNode {
    /** 子节点 */
    struct treeNode *child[MAX_CHILDREN_NUM];
    /** 兄弟节点 */
    struct treeNode *sibling;
    /** 当前行号 */
    int thisLine;
    /** 当前节点类型 @enum NodeKind */
    NodeKind nodekind;

    /**
     * @brief 节点具体类型
     */
    union {
        /** 语句类型 @enum StmtKind */
        StmtKind stmt;
        /** 表达式类型 @enum ExpKind */
        ExpKind exp;
    } kind;

    /**
     * @brief 节点属性信息
     */
    union {
        /** ExpK 类型中的具体表达式信息 */
        Token op;
        /** ConstK 类型中的具体数值信息 */
        int val;
        /** IdK 类型中具体的名称信息 */
        char *name;
    } attr;
    /** 节点静态类型 */
    ExpType type;
} TreeNode;

/** 哈希表长度 */
#define SIZE 512
/** 哈希函数偏移值 */
#define SHIFT 4

/**
 * @brief 符号表中变量被使用的行号表
 */
typedef struct LinkNode {
    int thisLine;  /**< 一条行号记录 */
    struct LinkNode *next;
} *Link;

/**
 * @brief 哈希表中元素的结构定义
 */
typedef struct LinkList {
    char *name;  /**< 变量名 */
    Link lines;  /**< 存储行号信息的链表 @struct LinkNode*/
    int memloc;  /**< 为变量分配的位置 */
    struct LinkList *next;
} *List;

/** 符号哈希表 */
static List hashTable[SIZE];

#endif
