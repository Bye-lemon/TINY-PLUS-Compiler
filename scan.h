/*****************************************************************************
*  TINY-PLUS-Compiler Token Scaner                                           *
*  Copyright (C) 2018 Yingping Li                                            *
*                                                                            *
*  @file     scan.h                                                          *
*  @brief    TPC编译器的词法分析器头文件                                        *
*  @author   Yingping Li                                                     *
*                                                                            *
*****************************************************************************/

#ifndef TPC_SCAN_H
#define TPC_SCAN_H

#define MAX_TOKEN_SIZE 50

extern char tokenString[MAX_TOKEN_SIZE + 1];

Token getToken(void);

#endif
