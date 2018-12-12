/*****************************************************************************
*  TINY-PLUS-Compiler Syntax Parser                                          *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     parse.c                                                         *
*  @brief    TPC编译器的语法分析器                                             *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#include "globals.h"
#include "utils.h"
#include "scan.h"
#include "parse.h"

/** 待处理的Token */
static Token token;

static TreeNode *stmt_sequence(void);
static TreeNode *statement(void);
static TreeNode *if_stmt(void);
static TreeNode *repeat_stmt(void);
static TreeNode *assign_stmt(void);
static TreeNode *read_stmt(void);
static TreeNode *write_stmt(void);
static TreeNode *complex_exp(void);
static TreeNode *simple_exp(void);
static TreeNode *addend(void);
static TreeNode *factor(void);

/**
 * @brief 将错误信息输出到stdout
 * @param message 错误消息
 */
static void syntaxError(const char *message) {
    fprintf(stdout, "\n>>> ");
    fprintf(stdout, "Line %d: ERROR: Syntax Error \'%s\'\n", thisLine, message);
    scanError++;
    Error = TRUE;
}

/**
 * @brief 获取下一个token，匹配成功返回token，失败抛出错误。
 * @param expected 期待获得的Token值
 */
static void match(Token expected) {
    if (token == expected)
        token = getToken();
    else {
        syntaxError("Unexpected Token ");
        printToken(token, tokenString, stdout);
        fprintf(stdout, "      ");
    }
}

/**
 * @brief 语句序列 -> 语句 ; 语句
 * @return 语法树的一个语句集节点
 */
TreeNode *stmt_sequence(void) {
    TreeNode *t = statement();
    TreeNode *p = t;
    while ((token != ENDFILE) && (token != END) && (token != ELSE) && (token != UNTIL)) {
        TreeNode *q;
        match(SEMI);
        q = statement();
        if (q != NULL) {
            if (t == NULL)
                t = p = q;
            else {
                p->sibling = q;
                p = q;
            }
        }
    }

    return t;
}

/**
 * @brief 语句 -> if 语句 | repeat 语句 | assign 语句 | read 语句 | write 语句
 * @return 语法树的一个语句节点
 */
TreeNode *statement(void) {
    TreeNode *t = NULL;
    switch (token) {
        case IF :
            t = if_stmt();
            break;
        case REPEAT :
            t = repeat_stmt();
            break;
        case ID :
            t = assign_stmt();
            break;
        case READ :
            t = read_stmt();
            break;
        case WRITE :
            t = write_stmt();
            break;
        default :
            syntaxError("Unexpected Token ");
            printToken(token, tokenString, stdout);
            token = getToken();
            break;
    }

    return t;
}

/**
 * @brief if 语句 -> if 表达式 then 语句序列 end | if 表达式 then 语句序列 else 语句序列 end
 * @return 语法树的一个IF语句节点
 */
TreeNode *if_stmt(void) {
    TreeNode *t = newStmtNode(IfK);
    match(IF);
    if (t != NULL)
        t->child[0] = complex_exp();
    match(THEN);
    if (t != NULL)
        t->child[1] = stmt_sequence();
    if (token == ELSE) {
        match(ELSE);
        if (t != NULL)
            t->child[2] = stmt_sequence();
    }
    match(END);

    return t;
}

/**
 * @brief repeat 语句 -> repeat 语句序列 until 表达式
 * @return 语法树的一个REPEAT语句节点
 */
TreeNode *repeat_stmt(void) {
    TreeNode *t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != NULL)
        t->child[0] = stmt_sequence();
    match(UNTIL);
    if (t != NULL)
        t->child[1] = complex_exp();

    return t;
}

/**
 * @brief assign 语句 -> 标识符 := 表达式
 * @return 语法树的一个ASSIGN节点
 */
TreeNode *assign_stmt(void) {
    TreeNode *t = newStmtNode(AssignK);
    if ((t != NULL) && (token == ID))
        t->attr.name = copyString(tokenString);
    match(ID);
    match(ASSIGN);
    if (t != NULL)
        t->child[0] = complex_exp();

    return t;
}

/**
 * @brief read 语句 -> read 标识符
 * @return 语法树的一个READ节点
 */
TreeNode *read_stmt(void) {
    TreeNode *t = newStmtNode(ReadK);
    match(READ);
    if ((t != NULL) && (token == ID))
        t->attr.name = copyString(tokenString);
    match(ID);

    return t;
}

/**
 * @brief write 语句 -> write 表达式
 * @return 语法树的一个WRITE节点
 */
TreeNode *write_stmt(void) {
    TreeNode *t = newStmtNode(WriteK);
    match(WRITE);
    if (t != NULL)
        t->child[0] = complex_exp();

    return t;
}

/**
 * @brief 表达式 -> 简单表达式 | 简单表达式 {<, >, =, <=, >=, <>} 简单表达式
 * @return 语法树的一个表达式节点
 */
TreeNode *complex_exp(void) {
    TreeNode *t = simple_exp();
    if ((token == LT) || (token == GT) || (token == EQ) || (token == LTEQ) || (token == GTEQ) || (token == NOTEQ)) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
        }
        match(token);
        if (t != NULL)
            t->child[1] = simple_exp();
    }

    return t;
}

/**
 * @brief 简单表达式 -> 加数 | 加数 {+, -} 加数
 * @return 语法树的一个简单表达式节点
 */
TreeNode *simple_exp(void) {
    TreeNode *t = addend();
    while ((token == PLUS) || (token == MINUS)) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = addend();
        }
    }

    return t;
}

/**
 * @brief 加数 -> 因数 | 因数 {*, /} 因数
 * @return 语法树的一个加数节点
 */
TreeNode *addend(void) {
    TreeNode *t = factor();
    while ((token == TIMES) || (token == OVER)) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = factor();
        }
    }

    return t;
}

/**
 * @brief 因数 -> ( 表达式 ) | 数字 | 标识符
 * @return 语法树的一个因数节点
 */
TreeNode *factor(void) {
    TreeNode *t = NULL;
    switch (token) {
        case NUM :
            t = newExpNode(ConstK);
            if ((t != NULL) && (token == NUM))
                t->attr.val = atoi(tokenString);
                /** atoi 函数: ASCII To Integer 将字符串转化为整数的库函数 */
            match(NUM);
            break;
        case ID :
            t = newExpNode(IdK);
            if ((t != NULL) && (token == ID))
                t->attr.name = copyString(tokenString);
            match(ID);
            break;
        case LPAREN :
            match(LPAREN);
            t = complex_exp();
            match(RPAREN);
            break;
        default:
            syntaxError("Unexpect Token ");
            printToken(token, tokenString, stdout);
            token = getToken();
            break;
    }
    return t;
}

/**
 * @brief 语法分析器主函数 程序 -> 语句序列
 * @return 语法树的根节点（程序节点）
 * @see scan.h
 */
TreeNode *parse(void) {
    TreeNode *t;
    token = getToken();
    t = stmt_sequence();
    if (token != ENDFILE)
        syntaxError("EOF\n");

    return t;
}
