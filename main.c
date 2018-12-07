#include "globals.h"
#include "scan.h"
#include "parse.h"
#include "utils.h"

int thisLine = 0;
int scanError = 0;

FILE *source;
FILE *oscan;
FILE *oparse;

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
    codefile = (char *) calloc(fnlen + 5, sizeof(char));
    strncpy(codefile, srcName, fnlen);
    oparse = fopen(strcat(codefile, ".parse"), "w");
    if (oscan == NULL || oparse == NULL) {
        printf("Unable to open %s\n", codefile);
        exit(1);
    }
    // while (getToken() != ENDFILE);
    TreeNode *syntaxTree = parse();
    printTree(syntaxTree);
    fclose(oscan);
    fclose(oparse);
    fclose(source);

    fprintf(stdout, "Scan Complete. %d Error(s)\n.", scanError);
    return 0;
}
