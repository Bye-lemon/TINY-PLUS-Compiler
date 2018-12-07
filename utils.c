//
// Created by dell on 2018/11/10.
//
#include "globals.h"
#include "utils.h"

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
            fprintf(output, "reserved word: %s\n", tokenString);
            break;
        case ASSIGN:
            fprintf(output, ":=\n");
            break;
        case LT:
            fprintf(output, "<\n");
            break;
        case LTEQ:
            fprintf(output, "<=\n");
            break;
        case EQ:
            fprintf(output, "=\n");
            break;
        case GT:
            fprintf(output, ">\n");
            break;
        case GTEQ:
            fprintf(output, ">=\n");
            break;
        case NOTEQ:
            fprintf(output, "<>\n");
            break;
        case LPAREN:
            fprintf(output, "(\n");
            break;
        case RPAREN:
            fprintf(output, ")\n");
            break;
        case SEMI:
            fprintf(output, ";\n");
            break;
        case PLUS:
            fprintf(output, "+\n");
            break;
        case MINUS:
            fprintf(output, "-\n");
            break;
        case TIMES:
            fprintf(output, "*\n");
            break;
        case OVER:
            fprintf(output, "/\n");
            break;
        case ENDFILE:
            fprintf(output, "EOF\n");
            break;
        case NUM:
            fprintf(output, "NUM, value= %s\n", tokenString);
            break;
        case ID:
            fprintf(output, "ID, name= %s\n", tokenString);
            break;
        case ERROR:
            fprintf(stdout, "Line %d: ERROR: Invaild Symbol \'%s\'\n", thisLine, tokenString);
            break;
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind kind) {
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(stdout, "Out of memory error at line %d\n", thisLine);
    else {
        for (i = 0; i < MAX_CHILDREN_NUM; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->thisLine = thisLine;
    }
    return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode *newExpNode(ExpKind kind) {
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(stdout, "Out of memory error at line %d\n", thisLine);
    else {
        for (i = 0; i < MAX_CHILDREN_NUM; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->thisLine = thisLine;
        t->type = Void;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char *copyString(char *s) {
    int n;
    char *t;
    if (s == NULL) return NULL;
    n = strlen(s) + 1;
    t = (char *)malloc(n);
    if (t == NULL)
        fprintf(stdout, "Out of memory error at line %d\n", thisLine);
    else strcpy(t, s);
    return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void) {
    int i;
    for (i = 0; i < indentno; i++)
        fprintf(oparse, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode *tree) {
    int i;
    INDENT;
    while (tree != NULL) {
        printSpaces();
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
    UNINDENT;
}