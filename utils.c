/*****************************************************************************
*  TINY-PLUS-Compiler Utilities                                              *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     utils.c                                                         *
*  @brief    TPC编译器的辅助函数                                               *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#include "globals.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"

/**
 * @brief 将词法分析器分析结果输出至文件流
 * @param token -> Token类型
 * @param tokenString -> Token对应的源码中的串
 * @param output -> 目标文件流
 */
void printToken(Token token, const char *tokenString, FILE *output) {
    switch (token) {
        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
            fprintf(output, "%-10s\tReserved Word\n", tokenString);
            break;
        case ASSIGN:
            fprintf(output, ":=        \tAssign\n");
            break;
        case LT:
            fprintf(output, "<         \tOperator\n");
            break;
        case LTEQ:
            fprintf(output, "<=        \tOperator\n");
            break;
        case EQ:
            fprintf(output, "=         \tOperator\n");
            break;
        case GT:
            fprintf(output, ">         \tOperator\n");
            break;
        case GTEQ:
            fprintf(output, ">=        \tOperator\n");
            break;
        case NOTEQ:
            fprintf(output, "<>        \tOperator\n");
            break;
        case LPAREN:
            fprintf(output, "(         \tLeft Parentheses\n");
            break;
        case RPAREN:
            fprintf(output, ")         \tRight Parentheses\n");
            break;
        case SEMI:
            fprintf(output, ";         \tSemicolon\n");
            break;
        case PLUS:
            fprintf(output, "+         \tOperator\n");
            break;
        case MINUS:
            fprintf(output, "-         \tOperator\n");
            break;
        case TIMES:
            fprintf(output, "*         \tOperator\n");
            break;
        case OVER:
            fprintf(output, "/         \tOperator\n");
            break;
        case ENDFILE:
            fprintf(output, "          \tEOF\n");
            break;
        case NUM:
            fprintf(output, "%-10s\tConst Number\n", tokenString);
            break;
        case ID:
            fprintf(output, "%-10s\tIdentification\n", tokenString);
            break;
        case ERROR:
            fprintf(stdout, "Line %d: ERROR: Invaild Symbol \'%s\'\n", thisLine, tokenString);
            break;
    }
}

/**
 * @brief 创建一个空的语句节点
 * @param kind -> 具体的语句类型 {StmtK, ExpK}
 * @return TreeNode
 */
TreeNode *newStmtNode(StmtKind kind) {
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(stdout, "Memory Overflow %d\n", thisLine);
    else {
        for (i = 0; i < MAX_CHILDREN_NUM; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->thisLine = thisLine;
    }
    return t;
}

/**
 * @brief 创建一个空的表达式节点
 * @param kind -> 具体的表达式语句类型 {OpK, ConstK, IdK}
 * @return TreeNode
 */
TreeNode *newExpNode(ExpKind kind) {
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(stdout, "Memory Overflow %d\n", thisLine);
    else {
        for (i = 0; i < MAX_CHILDREN_NUM; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->thisLine = thisLine;
        t->type = Void;
    }
    return t;
}

/**
 * @brief 动态创建一个空间，并复制字符串
 * @param s -> 源字符串
 * @return char *
 */
char *copyString(char *s) {
    int n;
    char *t;
    if (s == NULL) return NULL;
    n = strlen(s) + 1;
    t = (char *) malloc(n);
    if (t == NULL)
        fprintf(stdout, "Memory Overflow %d\n", thisLine);
    else strcpy(t, s);
    return t;
}

/** indentno -> 缩进大小 */
static int indent = 0;

/**
 * @brief 输出缩进内容
 */
static void printIndent(void) {
    int i;
    for (i = 0; i < indent-2; i++)
        fprintf(oparse, " ");
    fprintf(oparse, "┖-");
}

/**
 * @brief 将语法分析结果输出到文件流
 * @param tree -> 语法树的一个节点（根节点）
 */
void printTree(TreeNode *tree) {
    int i;
    indent += 2;
    while (tree != NULL) {
        printIndent();
        if (tree->nodekind == StmtK) {
            switch (tree->kind.stmt) {
                case IfK:
                    fprintf(oparse, "If\n");
                    break;
                case RepeatK:
                    fprintf(oparse, "Repeat\n");
                    break;
                case AssignK:
                    fprintf(oparse, "Assign to: %s\n", tree->attr.name);
                    break;
                case ReadK:
                    fprintf(oparse, "Read: %s\n", tree->attr.name);
                    break;
                case WriteK:
                    fprintf(oparse, "Write\n");
                    break;
                default:
                    fprintf(oparse, "Unknown ExpNode kind\n");
                    break;
            }
        } else if (tree->nodekind == ExpK) {
            switch (tree->kind.exp) {
                case OpK:
                    fprintf(oparse, "Op: ");
                    printToken(tree->attr.op, "\0", oparse);
                    break;
                case ConstK:
                    fprintf(oparse, "Const: %d\n", tree->attr.val);
                    break;
                case IdK:
                    fprintf(oparse, "Id: %s\n", tree->attr.name);
                    break;
                default:
                    fprintf(oparse, "Unknown ExpNode kind\n");
                    break;
            }
        } else fprintf(oparse, "Unknown node kind\n");
        for (i = 0; i < MAX_CHILDREN_NUM; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    indent -= 2;
}

/**
 * @brief 哈希函数
 * @param key -> 待散列字符串
 * @return int 哈希值
 */
int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }

    return temp;
}

/**
 * @brief 将标识符插入符号表
 * @param name -> 标识符名称
 * @param thisLine -> 行标号
 * @param loc -> 为该标识符分配的内存地址
 */
void st_insert(char *name, int thisLine, int loc) {
    int h = hash(name);
    List l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    if (l == NULL) {
        l = (List) malloc(sizeof(struct LinkList));
        l->name = name;
        l->lines = (Link) malloc(sizeof(struct LinkNode));
        l->lines->thisLine = thisLine;
        l->memloc = loc;
        l->lines->next = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    } else {
        Link t = l->lines;
        while (t->next != NULL)
            t = t->next;
        t->next = (Link) malloc(sizeof(struct LinkNode));
        t->next->thisLine = thisLine;
        t->next->next = NULL;
    }
}

/**
 * @brief 查找标识符在符号表中是否已存在
 * @param name -> 标识符名称
 * @return
 *     -<em>-1<em> 查找失败
 *     -<em>int<em> 标识符在符号表中的下标
 */
int st_lookup(char *name) {
    int h = hash(name);
    List l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    if (l == NULL)
        return -1;
    else
        return l->memloc;
}

/**
 * @brief 将符号表格式化输出到文件流
 * @param listing -> 输出文件流
 */
void printSymTab(FILE *listing) {
    int i;
    fprintf(listing, "Variable Name  Address   Line Numbers\n");
    fprintf(listing, "-------------  -------   ------------\n");
    for (i = 0; i < SIZE; ++i) {
        if (hashTable[i] != NULL) {
            List l = hashTable[i];
            while (l != NULL) {
                Link t = l->lines;
                fprintf(listing, "%-14s ", l->name);
                fprintf(listing, "%-7d  ", l->memloc);
                while (t != NULL) {
                    fprintf(listing, "%4d ", t->thisLine);
                    t = t->next;
                }
                fprintf(listing, "\n");
                l = l->next;
            }
        }
    }
}
