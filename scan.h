//
// Created by dell on 2018/11/10.
//

#ifndef TPC_SCAN_H
#define TPC_SCAN_H
#define MAX_TOKEN_SIZE 50

extern char tokenString[MAX_TOKEN_SIZE + 1];

Token getToken(void);
#endif //TPC_SCAN_H
