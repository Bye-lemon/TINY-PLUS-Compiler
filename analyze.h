//
// Created by dell on 2018/12/9.
//

#ifndef TPC_ANALYZE_H
#define TPC_ANALYZE_H

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *);

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode *);

#endif //TPC_ANALYZE_H
