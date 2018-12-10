//
// Created by Li Yingping on 2018/12/10.
//

#include "globals.h"
#include "utils.h"
#include "generate.h"

/* tmpOffset is the memory offset for temps
   It is decremented each time a temp is
   stored, and incremeted when loaded again
*/
static int tmpOffset = 0;

/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree);

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode *tree) {
    TreeNode *p1, *p2, *p3;
    int savedLoc1, savedLoc2, currentLoc;
    int loc;
    switch (tree->kind.stmt) {
        case IfK :
            genComment("-> if");
            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];
            /* generate code for test expression */
            cGen(p1);
            savedLoc1 = genSkip(1);
            genComment("if: jump to else belongs here");
            /* recurse on then part */
            cGen(p2);
            savedLoc2 = genSkip(1);
            genComment("if: jump to end belongs here");
            currentLoc = genSkip(0);
            genBackup(savedLoc1);
            genRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
            genRestore();
            /* recurse on else part */
            cGen(p3);
            currentLoc = genSkip(0);
            genBackup(savedLoc2);
            genRM_Abs("LDA", pc, currentLoc, "jmp to end");
            genRestore();
            genComment("<- if");
            break; /* if_k */

        case RepeatK:
            genComment("-> repeat");
            p1 = tree->child[0];
            p2 = tree->child[1];
            savedLoc1 = genSkip(0);
            genComment("repeat: jump after body comes back here");
            /* generate code for body */
            cGen(p1);
            /* generate code for test */
            cGen(p2);
            genRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
            genComment("<- repeat");
            break; /* repeat */

        case AssignK:
            genComment("-> assign");
            /* generate code for rhs */
            cGen(tree->child[0]);
            /* now store value */
            loc = st_lookup(tree->attr.name);
            genRM("ST", ac, loc, gp, "assign: store value");
            genComment("<- assign");
            break; /* assign_k */

        case ReadK:
            genRO("IN", ac, 0, 0, "read integer value");
            loc = st_lookup(tree->attr.name);
            genRM("ST", ac, loc, gp, "read: store value");
            break;
        case WriteK:
            /* generate code for expression to write */
            cGen(tree->child[0]);
            /* now output it */
            genRO("OUT", ac, 0, 0, "write ac");
            break;
        default:
            break;
    }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode *tree) {
    int loc;
    TreeNode *p1, *p2;
    switch (tree->kind.exp) {

        case ConstK :
            genComment("-> Const");
            /* gen code to load integer constant using LDC */
            genRM("LDC", ac, tree->attr.val, 0, "load const");
            genComment("<- Const");
            break; /* ConstK */

        case IdK :
            genComment("-> Id");
            loc = st_lookup(tree->attr.name);
            genRM("LD", ac, loc, gp, "load id value");
            genComment("<- Id");
            break; /* IdK */

        case OpK :
            genComment("-> Op");
            p1 = tree->child[0];
            p2 = tree->child[1];
            /* gen code for ac = left arg */
            cGen(p1);
            /* gen code to push left operand */
            genRM("ST", ac, tmpOffset--, mp, "op: push left");
            /* gen code for ac = right operand */
            cGen(p2);
            /* now load left operand */
            genRM("LD", ac1, ++tmpOffset, mp, "op: load left");
            switch (tree->attr.op) {
                case PLUS :
                    genRO("ADD", ac, ac1, ac, "op +");
                    break;
                case MINUS :
                    genRO("SUB", ac, ac1, ac, "op -");
                    break;
                case TIMES :
                    genRO("MUL", ac, ac1, ac, "op *");
                    break;
                case OVER :
                    genRO("DIV", ac, ac1, ac, "op /");
                    break;
                case LT :
                    genRO("SUB", ac, ac1, ac, "op <");
                    genRM("JLT", ac, 2, pc, "br if true");
                    genRM("LDC", ac, 0, ac, "false case");
                    genRM("LDA", pc, 1, pc, "unconditional jmp");
                    genRM("LDC", ac, 1, ac, "true case");
                    break;
                case EQ :
                    genRO("SUB", ac, ac1, ac, "op ==");
                    genRM("JEQ", ac, 2, pc, "br if true");
                    genRM("LDC", ac, 0, ac, "false case");
                    genRM("LDA", pc, 1, pc, "unconditional jmp");
                    genRM("LDC", ac, 1, ac, "true case");
                    break;
                default:
                    genComment("BUG: Unknown operator");
                    break;
            } /* case op */
            genComment("<- Op");
            break; /* OpK */

        default:
            break;
    }
} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen(TreeNode *tree) {
    if (tree != NULL) {
        switch (tree->nodekind) {
            case StmtK:
                genStmt(tree);
                break;
            case ExpK:
                genExp(tree);
                break;
            default:
                break;
        }
        cGen(tree->sibling);
    }
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void genCode(TreeNode *syntaxTree) {
    genComment("TINY Compilation to TM Code");
    /* generate standard prelude */
    genComment("Standard prelude:");
    genRM("LD", mp, 0, ac, "load maxaddress from location 0");
    genRM("ST", ac, 0, ac, "clear location 0");
    genComment("End of standard prelude.");
    /* generate code for TINY program */
    cGen(syntaxTree);
    /* finish */
    genComment("End of execution.");
    genRO("HALT", 0, 0, 0, "");
}