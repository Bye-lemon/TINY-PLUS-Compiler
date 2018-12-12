/*****************************************************************************
*  TINY-PLUS-Compiler Main Function                                          *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     main.c                                                          *
*  @brief    TPC编译器入口程序                                                 *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#include "globals.h"
#include "utils.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "generate.h"

/** 全局变量，存储当前行号 */
int thisLine = 0;
/** 全局变量，统计错误数量 */
int scanError = 0;
/** 源文件流 */
FILE *source;
/** 词法分析结果输出文件流 */
FILE *oscan;
/** 语法分析结果输出文件流 */
FILE *oparse;
/** 语义分析结果输出文件流 */
FILE *oanalyze;
/** 中间代码输出文件流 */
FILE *code;
/** 标志变量，存储出错信息 */
int Error = FALSE;

/**
 * @brief TPC编译器入口函数
 * @param argc 期待值为2
 * @param argv 源文件路径
 * @return
 *     @retval 0 程序顺利结束
 */
int main(int argc, char *argv[]) {
    char srcName[100];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <FILENAME>", argv[0]);
        exit(1);
    }
    strcpy(srcName, argv[1]);
    source = fopen(srcName, "r");
    if (source == NULL) {
        fprintf(stderr, "File %s don't exist", srcName);
        exit(1);
    }
    fprintf(stdout, "\nTINY COMPILATION: %s\n", srcName);

    char *codefile;
    int fnlen = strcspn(srcName, ".");
    codefile = (char *) calloc(fnlen + 4, sizeof(char));
    strncpy(codefile, srcName, fnlen);
    oscan = fopen(strcat(codefile, ".scan"), "w");
    codefile = (char *) calloc(fnlen + 4, sizeof(char));
    strncpy(codefile, srcName, fnlen);
    oparse = fopen(strcat(codefile, ".tree"), "w");
    codefile = (char *) calloc(fnlen + 4, sizeof(char));
    strncpy(codefile, srcName, fnlen);
    oanalyze = fopen(strcat(codefile, ".symbol"), "w");
    codefile = (char *) calloc(fnlen + 4, sizeof(char));
    strncpy(codefile, srcName, fnlen);
    code = fopen(strcat(codefile, ".code"), "w");
    if (oscan == NULL || oparse == NULL || oanalyze == NULL || code == NULL) {
        printf("Unable to open %s\n", codefile);
        exit(1);
    }
    fprintf(oscan, "Line\tContent   \tToken\n");
    fprintf(oscan, "----\t----------\t-----\n\n");
    TreeNode *syntaxTree = parse();
    fprintf(oparse, "Main Function\n");
    printTree(syntaxTree);
    if (!Error) {
        anaylze(syntaxTree);
    }
    if (!Error) {
        genCode(syntaxTree);
    }
    fclose(oscan);
    fclose(oparse);
    fclose(oanalyze);
    fclose(code);
    fclose(source);

    fprintf(stdout, "Scan Complete. %d Error(s)\n.", scanError);

    return 0;
}
