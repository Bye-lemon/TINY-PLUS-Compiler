# TINY-PLUS Compiler(TPC)
---
[![Build Status](https://travis-ci.org/Bye-lemon/TINY-PLUS-Complier.svg?branch=master)](https://travis-ci.org/Bye-lemon/TINY-PLUS-Complier)


## 使用方法

## 文法的定义

> 程序 -\> 语句序列
> 语句序列 -\> 语句 ; 语句
> 语句 -\> if 语句 | repeat 语句 | assign 语句 | read 语句 | write 语句
> if 语句 -\> if 表达式 then 语句序列 end | if 表达式 then 语句序列 else 语句序列 end
> repeat 语句 -\> repeat 语句序列 until 表达式
> assign 语句 -\> 标识符 := 表达式
> read 语句 -\> read 标识符
> write 语句 -\> write 表达式
> 表达式 -\> 简单表达式 | 简单表达式 {<, \>, =, <=, \>=, <\>} 简单表达式
> 简单表达式 -\> 加数 | 加数 {+, -} 加数
> 加数 -\> 因数 | 因数 {*, /} 因数
> 因数 -\> ( 表达式 ) | 数字 | 标识符
> 标识符 -\> 字母的串
> 数字 -\> 数字的串

## Token 列表

| 保留字 | Token  |
| ------ | ------ |
| if     | IF     |
| then   | THEN   |
| else   | ELSE   |
| repeat | REPEAT |
| until  | UNTIL  |
| read   | READ   |
| write  | WRITE  |
| end    | END    |

| 运算符 | Token  |
| ------ | ------ |
| +      | PLUS   |
| -      | MINUS  |
| *      | TIMES  |
| /      | OVER   |
| =      | EQ     |
| <>     | NOTEQ  |
| <      | LT     |
| \>      | GT     |
| <=     | LTEQ   |
| \>=    | GTEQ   |
| (      | LPAREN |
| )      | RPAREN |
| ;      | SEMI   |
| :=     | ASSIGN |

| 标识符 | Token |
| ------ | ----- |
| 数字   | NUM   |
| 标识符 | ID    |

