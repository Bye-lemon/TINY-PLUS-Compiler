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

typedef struct {
    int LTrue, LFalse;
    int LBegin, LNext;
    int is_stmt;
} CodeStruct;

/* tmpOffset is the memory offset for temps
It is decremented each time a temp is
stored, and incremeted when loaded again
*/
static int tmpOffset = 0;

/* prototype for internal recursive code generator */
static char *cGenThreeAddr(TreeNode *tree, CodeStruct *code);

static int LabelIndex = 0;
static int TempIndex = 1;
static int CodeLine = 1;

/* Procedure genStmt generates code at a statement node */
static void cGenStmt(TreeNode *tree, CodeStruct *S);

static char *cGenExp(TreeNode *tree, CodeStruct *S);

static int NewLabel() {
    return ++LabelIndex;
}

static void cGenStmt(TreeNode *tree, CodeStruct *S) {
    TreeNode *p1, *p2, *p3;
    char *str1;
    CodeStruct E, E1, E2;
    switch (tree->kind.stmt) {
        case IfK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];

            E.LTrue = NewLabel();
            if (p3 == NULL) {
                E.LFalse = S->LNext;
                E1.LNext = S->LNext;
                E.is_stmt = 1;
                cGenExp(p1, &E);
                if (p2 != NULL)
                    fprintf(code, "(%d)Label L%d\n", CodeLine++, E.LTrue);
                cGenThreeAddr(p2, &E1);
            } else {
                E.LFalse = NewLabel();
                E.is_stmt = 1;
                E1.LNext = S->LNext;
                E2.LNext = S->LNext;
                cGenExp(p1, &E);
                if (p1 != NULL)
                    fprintf(code, "(%d)Label L%d\n", CodeLine++, E.LTrue);
                cGenThreeAddr(p2, &E1);
                fprintf(code, "(%d)goto L%d\n", CodeLine++, S->LNext);
                if (p2 != NULL)
                    fprintf(code, "(%d)Label L%d\n", CodeLine++, E.LFalse);
                cGenThreeAddr(p3, &E2);
            }
            break;
        case RepeatK:

            p1 = tree->child[0];
            p2 = tree->child[1];

            S->LBegin = NewLabel();
            E1.LNext = NewLabel();
            E.LTrue = S->LNext;
            E.LFalse = S->LBegin;
            E.is_stmt = 1;
            if (p1 != NULL)
                fprintf(code, "(%d)Label L%d\n", CodeLine++, S->LBegin);
            cGenThreeAddr(p1, &E1);
            fprintf(code, "(%d)Label L%d\n", CodeLine++, E1.LNext);
            cGenExp(p2, &E);
            break;            /* repeat */
        case AssignK:
            str1 = cGenThreeAddr(tree->child[0], S);
            fprintf(code, "(%d)%s:=%s\n", CodeLine++, tree->attr.name, str1);
            free(str1);
            break;            /* assign_k */
        case ReadK:
            fprintf(code, "(%d)read %s\n", CodeLine++, tree->attr.name);
            break;
        case WriteK:
            /* generate code for expression to write */
            str1 = cGenThreeAddr(tree->child[0], S);
            fprintf(code, "(%d)write %s\n", CodeLine++, str1);
            free(str1);
            break;
    }

}

static char *cGenExp(TreeNode *tree, CodeStruct *S) {
    TreeNode *p1, *p2;
    char *str1, *str2;
    char *str = malloc(20);
    CodeStruct E1, E2;
    switch (tree->kind.exp) {
        case ConstK :
            sprintf(str, "%d", tree->attr.val);
            break;            /* ConstK */
        case IdK :
            strcpy(str, tree->attr.name);
            break;                             /* IdK */
        case OpK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            switch (tree->attr.op) {
                case GT:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->is_stmt == 1) {
                        fprintf(code, "(%d)if %s > %s goto L%d\n", CodeLine++, str1, str2, S->LTrue);
                        fprintf(code, "(%d)goto L%d\n", CodeLine++, S->LFalse);
                    } else
                        fprintf(code, "(%d)t%d := %s > %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case LT:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->is_stmt == 1) {
                        fprintf(code, "(%d)if %s < %s goto L%d\n", CodeLine++, str1, str2, S->LTrue);
                        fprintf(code, "(%d)goto L%d\n", CodeLine++, S->LFalse);
                    } else {
                        fprintf(code, "(%d)t%d := %s < %s\n", CodeLine++, TempIndex++, str1, str2);
                    }
                    break;
                case EQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->is_stmt == 1) {
                        fprintf(code, "(%d)if %s := %s goto L%d\n", CodeLine++, str1, str2, S->LTrue);
                        fprintf(code, "(%d)goto L%d\n", CodeLine++, S->LFalse);
                    } else
                        fprintf(code, "(%d)t%d := %s =  %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case GTEQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->is_stmt == 1) {
                        fprintf(code, "(%d)if %s >= %s goto L%d\n", CodeLine++, str1, str2, S->LTrue);
                        fprintf(code, "(%d)goto L%d\n", CodeLine++, S->LFalse);
                    } else
                        fprintf(code, "(%d)t%d := %s >= %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case LTEQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->is_stmt == 1) {
                        fprintf(code, "(%d)if %s <= %s goto L%d\n", CodeLine++, str1, str2, S->LTrue);
                        fprintf(code, "(%d)goto L%d\n", CodeLine++, S->LFalse);
                    } else
                        fprintf(code, "(%d)t%d :=%s <= %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case NOTEQ:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    if (S->is_stmt == 1) {
                        fprintf(code, "(%d)if %s != %s goto L%d\n", CodeLine++, str1, str2, S->LTrue);
                        fprintf(code, "(%d)goto L%d\n", CodeLine++, S->LFalse);
                    } else
                        fprintf(code, "(%d)t%d :=%s <= %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case PLUS:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%d)t%d = %s + %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case MINUS:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%d)t%d = %s - %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case TIMES:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%d)t%d = %s * %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
                case OVER:
                    str1 = cGenExp(p1, &E1);
                    str2 = cGenExp(p2, &E2);
                    fprintf(code, "(%d)t%d = %s / %s\n", CodeLine++, TempIndex++, str1, str2);
                    break;
            }
            free(str1);
            free(str2);
            sprintf(str, "%s%d", "t", TempIndex);
            break;
    }
    return str;
}

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
                        E.LNext = NewLabel();
                        E1.LNext = codes->LNext;
                        cGenStmt(tree, &E);
//				if(tree->kind.stmt!=ReadK && tree->kind.stmt!=AssignK && tree->kind.stmt!=WriteK)
                        fprintf(code, "(%d)Label L%d\n", CodeLine++, E.LNext);
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

void genCode(TreeNode *syntaxTree) {
    CodeStruct codes;
    codes.LBegin = LabelIndex;
    codes.LNext = LabelIndex;
    codes.LTrue = LabelIndex;
    codes.LFalse = LabelIndex;
    cGenThreeAddr(syntaxTree, &codes);
}
