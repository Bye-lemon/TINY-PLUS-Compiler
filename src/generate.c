/*****************************************************************************
*  TINY-PLUS-Compiler Code Generator                                         *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     generate.c                                                      *
*  @brief    TPC编译器的三元式生成器                                            *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#include "globals.h"
#include "utils.h"
#include "generate.h"

/**
* @brief 三元式生成器数据结构和全局变量定义
* @{
*/
/**
 * @brief 三元式数据结构
 */
typedef struct {
    int trueLabel;   /*!< 真出口 */   /**< 储存当条件表达式为真时跳转到的标号 */
    int falseLabel;  /*!< 假出口 */   /**< 储存当条件表达式为假时跳转到的标号 */
    int beginLabel;   /**/
    int nextLabel;
    int isStmt;      /*!< 节点类型 */ /**< 是否为一条语句 */
} CodeStruct;

/** 存储当前的Label标号 */
static int labelNum = 0;
/** 存储当前临时中间变量的序号 */
static int varNum = 1;
/** 存储当前中间代码行号 */
static int thisOutLine = 1;

static char *cGenThreeAddr(TreeNode *tree, CodeStruct *code);

static void cGenStmt(TreeNode *tree, CodeStruct *S);

static char *cGenExp(TreeNode *tree, CodeStruct *S);
/** @} */

/**
 * @brief 新建一个Label标号
 * @return int
 */
static int newLabel() {
    return ++labelNum;
}

/**
 * @brief 语句的语法制导翻译
 * @param tree -> 语法树的一个节点
 * @param S -> 三元式代码的一个节点
 */
static void cGenStmt(TreeNode *tree, CodeStruct *S) {
    TreeNode *p1, *p2, *p3;
    char *str1;
    CodeStruct E, E1, E2;
    switch (tree->kind.stmt) {
        case IfK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];
            E.trueLabel = newLabel();
            if (p3 == NULL) {
                E.falseLabel = S->nextLabel;
                E1.nextLabel = S->nextLabel;
                E.isStmt = 1;
                cGenExp(p1, &E);
                if (p2 != NULL)
                    fprintf(code, "(%-2d)(label, -, -, l%d)\n", thisOutLine++, E.trueLabel);
                cGenThreeAddr(p2, &E1);
            } else {
                E.falseLabel = newLabel();
                E.isStmt = 1;
                E1.nextLabel = S->nextLabel;
                E2.nextLabel = S->nextLabel;
                cGenExp(p1, &E);
                if (p1 != NULL)
                    fprintf(code, "(%-2d)(label, -, -, l%d)\n", thisOutLine++, E.trueLabel);
                cGenThreeAddr(p2, &E1);
                fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, S->nextLabel);
                if (p2 != NULL)
                    fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, E.falseLabel);
                cGenThreeAddr(p3, &E2);
            }
            break;
        case RepeatK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            S->beginLabel = newLabel();
            E1.nextLabel = newLabel();
            E.trueLabel = S->nextLabel;
            E.falseLabel = S->beginLabel;
            E.isStmt = 1;
            if (p1 != NULL)
                fprintf(code, "(%-2d)(label, -, -, l%d)\n", thisOutLine++, S->beginLabel);
            cGenThreeAddr(p1, &E1);
            fprintf(code, "(%-2d)(label, -, -, l%d)\n", thisOutLine++, E1.nextLabel);
            cGenExp(p2, &E);
            break;
        case AssignK:
            str1 = cGenThreeAddr(tree->child[0], S);
            fprintf(code, "(%-2d)(:=   , %s, -, %s)\n", thisOutLine++, str1, tree->attr.name);
            free(str1);
            break;
        case ReadK:
            fprintf(code, "(%-2d)(read , -, -, %s)\n", thisOutLine++, tree->attr.name);
            break;
        case WriteK:
            str1 = cGenThreeAddr(tree->child[0], S);
            fprintf(code, "(%-2d)(write, -, -, %s)\n", thisOutLine++, str1);
            free(str1);
            break;
    }
}

/**
 * @brief 表达式的语法制导翻译
 * @param tree -> 语法树的一个节点
 * @param S -> 三元式代码的一个节点
 * @return char *
 */
static char *cGenExp(TreeNode *tree, CodeStruct *S) {
    TreeNode *p1, *p2;
    char *str1, *str2;
    char *str = malloc(20);
    CodeStruct E1, E2;
    switch (tree->kind.exp) {
        case ConstK :
            sprintf(str, "%d", tree->attr.val);
            break;
        case IdK :
            strcpy(str, tree->attr.name);
            break;
        case OpK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            switch (tree->attr.op) {
                case GT:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->isStmt == 1) {
                        fprintf(code, "(%-2d)(j>   , %s, %s, l%d)\n", thisOutLine++, str1, str2, S->trueLabel);
                        fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, S->falseLabel);
                    } else
                        fprintf(code, "(%-2d)(:=   , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    break;
                case LT:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->isStmt == 1) {
                        fprintf(code, "(%-2d)(j<   , %s, %s, l%d)\n", thisOutLine++, str1, str2, S->trueLabel);
                        fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, S->falseLabel);
                    } else {
                        fprintf(code, "(%-2d)(:=   , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    }
                    break;
                case EQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->isStmt == 1) {
                        fprintf(code, "(%-2d)(j=   , %s, %s, l%d)\n", thisOutLine++, str1, str2, S->trueLabel);
                        fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, S->falseLabel);
                    } else {
                        fprintf(code, "(%-2d)(:=   , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    }
                    break;
                case GTEQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->isStmt == 1) {
                        fprintf(code, "(%-2d)(j>=   , %s, %s, l%d)\n", thisOutLine++, str1, str2, S->trueLabel);
                        fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, S->falseLabel);
                    } else {
                        fprintf(code, "(%-2d)(:=   , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    }
                    break;
                case LTEQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->isStmt == 1) {
                        fprintf(code, "(%-2d)(j<=   , %s, %s, l%d)\n", thisOutLine++, str1, str2, S->trueLabel);
                        fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, S->falseLabel);
                    } else {
                        fprintf(code, "(%-2d)(:=   , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    }
                    break;
                case NOTEQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->isStmt == 1) {
                        fprintf(code, "(%-2d)(j<>   , %s, %s, l%d)\n", thisOutLine++, str1, str2, S->trueLabel);
                        fprintf(code, "(%-2d)(goto , -, -, l%d)\n", thisOutLine++, S->falseLabel);
                    } else {
                        fprintf(code, "(%-2d)(:=   , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    }
                    break;
                case PLUS:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%-2d)(+    , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    break;
                case MINUS:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%-2d)(-    , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    break;
                case TIMES:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%-2d)(*    , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    break;
                case OVER:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%-2d)(/    , %s, %s, t%d)\n", thisOutLine++, str1, str2, varNum ++);
                    break;
            }
            free(str1);
            free(str2);
            sprintf(str, "%s%d", "t", varNum);
            break;
    }

    return str;
}

/**
 * @brief 语法制导翻译控制函数
 * @param tree -> 语法树的一个节点
 * @param codes -> 三元式代码的一个节点
 * @return
 */
char *cGenThreeAddr(TreeNode *tree, CodeStruct *codes) {
    char *str = NULL;
    CodeStruct E, E1;
    if (tree != NULL) {
        switch (tree->nodekind) {
            case StmtK:
                switch (tree->kind.stmt) {
                    case RepeatK:
                    case IfK:
                    case AssignK:
                    case ReadK:
                    case WriteK:
                        E.nextLabel = newLabel();
                        E1.nextLabel = codes->nextLabel;
                        cGenStmt(tree, &E);
                        fprintf(code, "(%-2d)(label, -, -, l%d)\n", thisOutLine++, E.nextLabel);
                        break;
                }
                break;
            case ExpK:
                str = cGenExp(tree, codes);
                break;
            default:
                break;
        }
        cGenThreeAddr(tree->sibling, &E1);
    }

    return str;
}

/**
 * @brief 三元式生成器主函数
 * @param syntaxTree -> 语法树根节点
 */
void genCode(TreeNode *syntaxTree) {
    CodeStruct codes;

    codes.beginLabel = labelNum;
    codes.nextLabel = labelNum;
    codes.trueLabel = labelNum;
    codes.falseLabel = labelNum;
    cGenThreeAddr(syntaxTree, &codes);
}
