//
// Created by dell on 2018/11/10.
//
#include "globals.h"
#include "utils.h"
#include "scan.h"

#define BUFFER 256 // 每行最大字符数

typedef enum {
    START, DONE, INID, INNUM, INLT, INGT, INASSIGN, INCOMMENT
} State;

char tokenString[MAX_TOKEN_SIZE + 1];

static char lineBuffer[BUFFER];
static int thisPosition = 0;
static int lineLens = 0;
static int EOF_FLAG = FALSE;

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

static void goBack(void) {
    if (!EOF_FLAG)
        thisPosition--;
}

// 保留字表
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

static Token reservedLookup(char *s) {
    int i;

    for (i = 0; i < RESERVED_WORD_NUM; i++) {
        if (!strcmp(s, reservedWordList[i].str))
            return reservedWordList[i].token;
    }

    return ID;
}

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
            tokenString[tokenStringIndex++] = (char)ch;
        if (thisState == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (thisToken == ID)
                thisToken = reservedLookup(tokenString);
        }
    }
    fprintf(oscan, "\t%d: ", thisLine);
    printToken(thisToken, tokenString);

    if (thisToken == ERROR)
        scanError ++;

    return thisToken;
}
