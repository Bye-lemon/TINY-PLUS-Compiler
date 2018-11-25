//
// Created by dell on 2018/11/10.
//
#include "globals.h"
#include "utils.h"

void printToken(Token token, const char *tokenString) {
    switch (token) {
        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
            fprintf(oscan, "reserved word: %s\n", tokenString);
            break;
        case ASSIGN:
            fprintf(oscan, ":=\n");
            break;
        case LT:
            fprintf(oscan, "<\n");
            break;
        case LTEQ:
            fprintf(oscan, "<=\n");
            break;
        case EQ:
            fprintf(oscan, "=\n");
            break;
        case GTEQ:
            fprintf(oscan, ">=\n");
            break;
        case NOTEQ:
            fprintf(oscan, "<>\n");
            break;
        case LPAREN:
            fprintf(oscan, "(\n");
            break;
        case RPAREN:
            fprintf(oscan, ")\n");
            break;
        case SEMI:
            fprintf(oscan, ";\n");
            break;
        case PLUS:
            fprintf(oscan, "+\n");
            break;
        case MINUS:
            fprintf(oscan, "-\n");
            break;
        case TIMES:
            fprintf(oscan, "*\n");
            break;
        case OVER:
            fprintf(oscan, "/\n");
            break;
        case ENDFILE:
            fprintf(oscan, "EOF\n");
            break;
        case NUM:
            fprintf(oscan, "NUM, value= %s\n", tokenString);
            break;
        case ID:
            fprintf(oscan, "ID, name= %s\n", tokenString);
            break;
        case ERROR:
            fprintf(stdout, "Line %d: ERROR: Invaild Symbol \'%s\'\n", thisLine, tokenString);
            break;
    }
}