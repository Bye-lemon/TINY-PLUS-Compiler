/*****************************************************************************
*  TINY-PLUS-Compiler Token Scaner                                           *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     scan.c                                                          *
*  @brief    TPC编译器的词法分析器                                             *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#include "globals.h"
#include "utils.h"
#include "scan.h"

/**
* @brief 词法分析器全局变量定义
* @{
*/
#define BUFFER 256

/**
 * @brief 定义有穷状态自动机的状态
 */
typedef enum {
    START,      /*!< 开始状态 */  /**< 状态机的开始状态 */
    DONE,       /*!< 结束状态 */  /**< 状态机的结束状态 */
    INID,       /*!< 标识符状态 */  /**< 检测标识符（字符序列串） */
    INNUM,      /*!< 数字状态 */  /**< 检测常数（数字序列串） */
    INLT,       /*!< 小于号状态 */  /**< 判断{<, <=, <>} */
    INGT,       /*!< 大于号状态 */  /**< 判断{>, >=} */
    INASSIGN,   /*!< 赋值状态 */  /**< 判断是否为:= */
    INCOMMENT   /*!< 注释状态 */  /**< 处理源代码中的注释 */
} State;

char tokenString[MAX_TOKEN_SIZE + 1];

static char lineBuffer[BUFFER];
static int thisPosition = 0;
static int lineLens = 0;
static int EOF_FLAG = FALSE;
/** @} */

/**
 * @brief 获取当前行下一个字符
 * @return 下一个待检测字符
 */
static int getNext(void) {
    if (thisPosition >= lineLens) {
        thisLine++;
        if (fgets(lineBuffer, BUFFER - 1, source)) {
            fprintf(oscan, "%4d: %s", thisLine, lineBuffer);
            lineLens = strlen(lineBuffer);
            thisPosition = 0;
            return lineBuffer[thisPosition++];
        } else {
            EOF_FLAG = TRUE;
            return EOF;
        }
    } else
        return lineBuffer[thisPosition++];
}

/**
 * @brief 将当前指针减一，用来返回{<=, >=, <>, :=}的报错信息
 */
static void goBack(void) {
    if (!EOF_FLAG)
        thisPosition--;
}

/**
 * @brief 文法保留字表
 */
static struct {
    char *str;
    Token token;
} reservedWordList[RESERVED_WORD_NUM] = {
        {"if",     IF},
        {"then",   THEN},
        {"else",   ELSE},
        {"end",    END},
        {"repeat", REPEAT},
        {"until",  UNTIL},
        {"read",   READ},
        {"write",  WRITE}
};

/**
 * @brief 查找一个串属不属于保留字，属于返回保留字Token，不属于标记为ID（标识符）
 * @param s -> 待检查字符串
 * @return Token类型
 */
static Token reservedLookup(char *s) {
    int i;

    for (i = 0; i < RESERVED_WORD_NUM; i++) {
        if (!strcmp(s, reservedWordList[i].str))
            return reservedWordList[i].token;
    }

    return ID;
}

/**
 * @brief 词法分析器主函数，每次生成一个Token供语法分析器调用
 * @return 待处理的Token
 */
Token getToken(void) {
    int tokenStringIndex = 0;
    Token thisToken;
    State thisState = START;
    int SAVE_FLAG;

    while (thisState != DONE) {
        int ch = getNext();
        SAVE_FLAG = TRUE;

        switch (thisState) {
            case START:
                if (isdigit(ch))
                    thisState = INNUM;
                else if (isalpha(ch))
                    thisState = INID;
                else if (ch == '<')
                    thisState = INLT;
                else if (ch == '>')
                    thisState = INGT;
                else if (ch == ':')
                    thisState = INASSIGN;
                else if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
                    SAVE_FLAG = FALSE;
                else if (ch == '{') {
                    thisState = INCOMMENT;
                    SAVE_FLAG = FALSE;
                } else {
                    thisState = DONE;
                    switch (ch) {
                        case EOF:
                            SAVE_FLAG = FALSE;
                            thisToken = ENDFILE;
                            break;
                        case '=':
                            thisToken = EQ;
                            break;
                        case '+':
                            thisToken = PLUS;
                            break;
                        case '-':
                            thisToken = MINUS;
                            break;
                        case '*':
                            thisToken = TIMES;
                            break;
                        case '/':
                            thisToken = OVER;
                            break;
                        case '(':
                            thisToken = LPAREN;
                            break;
                        case ')':
                            thisToken = RPAREN;
                            break;
                        case ';':
                            thisToken = SEMI;
                            break;
                        default:
                            thisToken = ERROR;
                            break;
                    }
                }
                break;
            case INNUM:
                if (!isdigit(ch)) {
                    goBack();
                    SAVE_FLAG = FALSE;
                    thisState = DONE;
                    thisToken = NUM;
                }
                break;
            case INID:
                if (!isalpha(ch)) {
                    goBack();
                    SAVE_FLAG = FALSE;
                    thisState = DONE;
                    thisToken = ID;
                }
                break;
            case INLT:
                thisState = DONE;
                if (ch == ' ')
                    thisToken = LT;
                else if (ch == '=')
                    thisToken = LTEQ;
                else if (ch == '>')
                    thisToken = NOTEQ;
                else {
                    goBack();
                    SAVE_FLAG = FALSE;
                    thisToken = ERROR;
                }
                break;
            case INGT:
                thisState = DONE;
                if (ch == ' ')
                    thisToken = GT;
                else if (ch == '=')
                    thisToken = GTEQ;
                else {
                    goBack();
                    SAVE_FLAG = FALSE;
                    thisToken = ERROR;
                };
                break;
            case INASSIGN:
                thisState = DONE;
                if (ch == '=')
                    thisToken = ASSIGN;
                else {
                    goBack();
                    SAVE_FLAG = FALSE;
                    thisToken = ERROR;
                }
                break;
            case INCOMMENT:
                SAVE_FLAG = FALSE;
                if (ch == '}')
                    thisState = START;
                else if (ch == EOF) {
                    thisState = DONE;
                    thisToken = ENDFILE;
                }
                break;
        }

        if ((SAVE_FLAG) && (tokenStringIndex <= MAX_TOKEN_SIZE))
            tokenString[tokenStringIndex++] = (char) ch;
        if (thisState == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (thisToken == ID)
                thisToken = reservedLookup(tokenString);
        }
    }
    fprintf(oscan, "\t%d: ", thisLine);
    printToken(thisToken, tokenString, oscan);

    if (thisToken == ERROR)
        scanError++;

    return thisToken;
}
