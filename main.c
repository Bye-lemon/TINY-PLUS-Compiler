#include "globals.h"
#include "utils.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "generate.h"


int thisLine = 0;
int scanError = 0;

FILE *source;
FILE *oscan;
FILE *oparse;
FILE *oanalyze;
FILE *code;

int Error = FALSE;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

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
    // while (getToken() != ENDFILE);
    TreeNode *syntaxTree = parse();
    fprintf(oparse, "Main Function\n");
    printTree(syntaxTree);
    if (!Error) {
        buildSymtab(syntaxTree);
        typeCheck(syntaxTree);
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
