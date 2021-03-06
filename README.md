# TINY-PLUS Compiler(TPC)

---

[![Build Status](https://travis-ci.org/Bye-lemon/TINY-PLUS-Compiler.svg?branch=master)](https://travis-ci.org/Bye-lemon/TINY-PLUS-Compiler)

## 文件说明

| 文件名/文件夹名 | 文件功能     |
| --------------- | ------------ |
| dist/           | GUI          |
| test/           | 测试文件源码 |
| TPC-GUI/        | GUI源代码    |

## 使用方法

#### 构建TPC可执行文件

```bash
mkdir build
cd build
cmake ..
make
```

#### 编译TNY文件

```bash
./TPC <Filename>
```

#### 编译结果说明

| 后缀名  | 说明           |
| ------- | -------------- |
| .scan   | 词法分析结果   |
| .tree   | 语法分析结果   |
| .symbol | 符号表         |
| .code   | 四元式中间代码 |

## 文法的定义

> 程序 -\> 语句序列<br>
> 语句序列 -\> 语句 ; 语句<br>
> 语句 -\> if 语句 | repeat 语句 | assign 语句 | read 语句 | write 语句<br>
> if 语句 -\> if 表达式 then 语句序列 end | if 表达式 then 语句序列 else 语句序列 end<br>
> repeat 语句 -\> repeat 语句序列 until 表达式<br>
> assign 语句 -\> 标识符 := 表达式<br>
> read 语句 -\> read 标识符<br>
> write 语句 -\> write 表达式<br>
> 表达式 -\> 简单表达式 | 简单表达式 {<, \>, =, <=, \>=, <\>} 简单表达式<br>
> 简单表达式 -\> 加数 | 加数 {+, -} 加数<br>
> 加数 -\> 因数 | 因数 {*, /} 因数<br>
> 因数 -\> ( 表达式 ) | 数字 | 标识符<br>
> 标识符 -\> 字母的串<br>
> 数字 -\> 数字的串<br>

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

