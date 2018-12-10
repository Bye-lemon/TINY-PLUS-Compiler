/*
 * Created by dell on 2018/11/10.
 */

#ifndef TPC_GLOBALS_H
#define TPC_GLOBALS_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

/*
 * Data Structure; Const Parameter
 * Usage:          Boolean value definition
 * Used By:        Global
 */
#define TRUE 1
#define FALSE 0

/* Scanner Definition */
/*
 * Data Structure; Enum Array
 * Usage:          Define token type
 * Variable:       Token -> List of all tokens
 * Used By:        Scanner
 */
typedef enum {
    ENDFILE, ERROR, ID, NUM,
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    PLUS, MINUS, TIMES, OVER, EQ, NOTEQ, LT, LTEQ, GT, GTEQ, LPAREN, RPAREN, SEMI, ASSIGN
} Token;

/*
 * Data Structure; Const Parameter
 * Usage:          Number of reserved word
 * Addition:       IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE
 * Used By:        Scanner
 */
#define RESERVED_WORD_NUM 8
/* End Scanner Definition */

/* Main Function Definition */
/*
 * Data Structure; Const Parameter
 * Usage:          Global line number
 * Variable:       thisLine -> Global line number
 * Used By:        Global
 */
extern int thisLine;

/*
 * Data Structure; File
 * Usage:          File I/O
 * Variable:       source   -> TINY-PLUS language source file
 *                 oscan    -> Result of scanner
 *                 oparse   -> Result of syntax parser
 *                 oanalyze -> Result of semantics analyzer
 *                 code     -> Middle Code
 * Used By:        Global
 */
extern FILE *source;
extern FILE *oscan;
extern FILE *oparse;
extern FILE *oanalyze;
extern FILE *code;

/*
 * Data Structure; Const Parameter
 * Usage:          Record compile errors
 * Variable:       Error     -> Flag variable
 *                 scanError -> Count number of Errors
 * Used By:        Global
 */
extern int Error;
extern int scanError;
/* End Main Function Definition */

/* Parser Definition */
/*
 * Data Structure; Enum Array
 * Usage:          Define kind of statement
 * Variable:       NodeKind -> Divided statement to logic control statement and experiment
 *                 StmtKind -> Define logic control statement, including IfK RepeatK AssignK ReadK WriteK
 *                 ExpKind  -> Define experiment, including OpK ConstK IdK
 *                 ExpType  -> Define kind of an experiment's  result
 * Used By:        Global Definition
 */
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

/*
 * Data Structure; Const Parameter
 * Usage:          Define child node max number
 * Addition:       All operators have at most 2 numbers to operate, so a node at most has 3 child, 1 opK and 2 ConstK or IdK
 * Used By:        Global Definition
 */
#define MAX_CHILDREN_NUM 3

/*
 * Data Structure; Tree
 * Usage:          Record result of syntax analyze
 * Variable:       child    -> Link to child node
 *                 sibling  -> Link to sibling node (Statement after ';')
 *                 thisLine -> Record line number now
 *                 nodeKind -> Record which kind of statement this node belongs to
 *                 kind     -> Record node kind detail
 *                 attr     -> Record values, including label of operator, value of integer and name of identification
 *                 type     -> Marked node type to check semantics simply
 * Used By:        Syntax Parser & Semantics Analyzer
 */
typedef struct treeNode {
    struct treeNode *child[MAX_CHILDREN_NUM];
    struct treeNode *sibling;
    int thisLine;
    NodeKind nodekind;
    union {
        StmtKind stmt;
        ExpKind exp;
    } kind;
    union {
        Token op;
        int val;
        char *name;
    } attr;
    ExpType type;
} TreeNode;
/* End Parser Definition */

/* Symbol Table Definition */
/*
 * Data Structure: Const Parameter
 * Usage:          Parameter of Hash Function, Also Used to Define A Hash Table
 * Variable:       SIZE  -> Length of Hash Table
 *                 SHIFT -> Shift Length
 * Used By:        Symbol Table (utils.c)
 */
#define SIZE 211
#define SHIFT 4

/*
 * Data Structure: LinkList
 * Usage:          Record variable usage in which line of the source file
 * Variable:       thisLine -> Line number
 *                 next     -> Link to next record
 * Used By:        Symbol Table
 */
typedef struct LinkNode {
    int thisLine;
    struct LinkNode *next;
} *Link;

/*
 * Data Structure; LinkList
 * Usage:          Record all information of a variable
 * Variable:       name   -> Variable name
 *                 lines  -> Record lines where this variable occur
 *                 memloc -> Stack Address Locations
 *                 next   -> Link to next Variable
 * Used By:        Symbol Table
 */
typedef struct LinkList {
    char *name;
    Link lines;
    int memloc; /* memory location for variable */
    struct LinkList *next;
} *List;

/*
 * Data Structure; Array (Hash Table)
 * Usage:          Symbol Table
 * Variable:       hashTable -> Store symbols
 * Used By:        Symbol Table & Semantics Analyzer
 */
static List hashTable[SIZE];
/* End Symbol Table Definition */

#endif //TPC_GLOBALS_H
