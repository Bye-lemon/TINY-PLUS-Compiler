/*****************************************************************************
*  TINY-PLUS-Compiler Semantic Analyzer                                      *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     analyze.c                                                       *
*  @brief    TPC编译器的语义分析器                                             *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#include "globals.h"
#include "utils.h"
#include "analyze.h"

/**
* @brief 语义分析器的全局变量定义
* @{
*/
/** location -> 记录符号表已分配的地址 */
static int location = 0;
/** @} */

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */

/**
 * @brief 遍历语法树的通用模板函数
 * @param t -> 语法树的一个节点
 * @param preProc -> 第一个函数
 * @param postProc -> 第二个函数
 */
static void traverse(TreeNode *t, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *)) {
    if (t != NULL) {
        preProc(t);
        int i;
        for (i = 0; i < MAX_CHILDREN_NUM; i++)
            traverse(t->child[i], preProc, postProc);
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

/**
 * @brief 占位符，空过程
 * @param t -> 语法树的一个节点
 */
static void nullProc(TreeNode *t) {

    return ;
}

/**
 * @brief 过程函数，用来将节点信息插入符号表
 * @param t -> 节点树的一个节点
 */
static void insertNode(TreeNode *t) {
    switch (t->nodekind) {
        case StmtK:
            switch (t->kind.stmt) {
                /**
                 * 语句类if write repeat 不产生新变量， 所以只考虑assign和read
                 * 语句类节点需要保存的是变量的标识符
                 */
                case AssignK:
                case ReadK:
                    if (st_lookup(t->attr.name) == -1)
                        st_insert(t->attr.name, t->thisLine, location++);
                    else
                        st_insert(t->attr.name, t->thisLine, 0);
                    break;
                default:
                    break;
            }
            break;
        case ExpK:
            switch (t->kind.exp) {
                /**
                 * 表达式类op const 不是变量，只有id需要计入符号表
                 */
                case IdK:
                    if (st_lookup(t->attr.name) == -1)
                        st_insert(t->attr.name, t->thisLine, location++);
                    else
                        st_insert(t->attr.name, t->thisLine, 0);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

/**
 * @brief 先序遍历语法树，创建符号表并打印到输出文件中
 * @param syntaxTree -> 语法树的一个节点（根节点）
 */
void buildSymtab(TreeNode *syntaxTree) {
    traverse(syntaxTree, insertNode, nullProc);
    printSymTab(oanalyze);
}

/**
 * @brief 将语义分析错误信息输出到stdout
 * @param t -> 语法树的一个节点
 * @param message -> 错误信息
 */
static void typeError(TreeNode *t, char *message) {
    fprintf(stdout, "Line %d: ERROR: Type Error \'%s\'\n", t->thisLine, message);
    scanError ++;
    Error = TRUE;
}

/**
 * @brief 对语法树的一个节点进行静态类型检查
 * @param t -> 语法树的一个节点
 */
static void checkNode(TreeNode *t) {
    switch (t->nodekind) {
        case ExpK:
            switch (t->kind.exp) {
                case OpK:
                    if ((t->child[0]->type != Integer) || (t->child[1]->type != Integer))
                        typeError(t, "Operator Required Integer");
                    if ((t->attr.op == EQ) || (t->attr.op == LT) || (t->attr.op == GT) || (t->attr.op == LTEQ) || (t->attr.op == NOTEQ))
                        t->type = Boolean;
                        /** 比较表达式节点为布尔型 */
                    else
                        t->type = Integer;
                        /** 算术表达式节点为整型 */
                    break;
                case ConstK:
                case IdK:
                    t->type = Integer;
                    break;
                default:
                    break;
            }
            break;
        case StmtK:
            switch (t->kind.stmt) {
                case IfK:
                    if (t->child[0]->type == Integer)
                        typeError(t->child[0], "If Required Boolean");
                    break;
                case AssignK:
                    if (t->child[0]->type != Integer)
                        typeError(t->child[0], "Assign Required Integer");
                    break;
                case WriteK:
                    if (t->child[0]->type != Integer)
                        typeError(t->child[0], "Write Required Integer");
                    break;
                case RepeatK:
                    if (t->child[1]->type == Integer)
                        typeError(t->child[1], "Repeat Required Boolean");
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

/**
 * @brief 后序遍历语法树，进行静态类型检查
 * @param syntaxTree -> 语法树的一个节点（根节点）
 */
void typeCheck(TreeNode *syntaxTree) {
    traverse(syntaxTree, nullProc, checkNode);
}

/**
 * @brief 语义分析器主函数
 * @param syntaxTree -> 语法树的一个节点（根节点）
 */
void anaylze(TreeNode *syntaxTree) {
    buildSymtab(syntaxTree);
    typeCheck(syntaxTree);
}