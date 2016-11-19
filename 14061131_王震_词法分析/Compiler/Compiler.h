//
// Created by 王震 on 2016/11/7.
//

#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

typedef enum {
    noneTyp,//NULL
    customObj,//自定义类型
    aNum,//数字
    aChar,//字符
    aString,//字符串
    Int,//整型
    Char,//字符型
    Void,//void
    lBrace,//左花括号
    rBrace,//右花括号
    lBracket,//左中括号
    rBracket,//右中括号
    lParent,//左小括号
    rParent,//右小括号
    For,//for
    While,//while
    Do,//do
    If,//if
    Else,//else
    big,//>
    small,//<
    assign,//=
    bigAndEql,//>=
    smallAndEql,//<=
    notEql,//!=
    eql,//==
    plus,//+
    sub,//-
    mul,//*
    div,///
    Printf,//printf
    Scanf,//scanf
    semicolon,//分号
    comma,//逗号
    colon,//冒号
    Const,//const
    lDoubleQuo,//左双引号
    RDoubleQuo,//右双引号
    lSingleQuo,//左单引号
    rSingleQuo,//右单引号
    singleQuo,//单引号
    doubleQuo,//双引号
    space,//空格
    enter//回车换行
} ReservedWord;

typedef struct {
    char symbol;
    ReservedWord reservedWord;
} Interval;

typedef struct {
    char* symbol;
    ReservedWord reservedWord;
    char* rwToString;
} Keyword;

Interval intervals[100];
Keyword keywords[100];

#endif //COMPILER_COMPILER_H
