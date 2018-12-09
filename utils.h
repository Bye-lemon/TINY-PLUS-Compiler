//
// Created by dell on 2018/11/10.
//

#ifndef TPC_UTILS_H
#define TPC_UTILS_H

void printToken(Token, const char *, FILE *);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind);

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode *newExpNode(ExpKind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char *copyString(char *);

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode *);

// Symbol Table Definition
int hash(char *);
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert(char *name, int lineno, int loc);

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char *name);
void printSymTab(FILE *listing);

#endif //TPC_UTILS_H
