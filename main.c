#include "globals.h"
#include "scan.h"

int thisLine = 0;
int scanError = 0;

FILE *source;
FILE *oscan;

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
    strcat(codefile, ".scan");
    oscan = fopen(codefile, "w");
    if (oscan == NULL) {
        printf("Unable to open %s\n", codefile);
        exit(1);
    }
    while (getToken() != ENDFILE);
    fclose(oscan);
    fclose(source);

    fprintf(stdout, "Scan Complete. %d Error(s)\n.", scanError);
    return 0;
}
