//
// Created by 王震 on 2016/11/7.
//

#ifndef COMPILER_ANALYSIS_H
#define COMPILER_ANALYSIS_H

#include <string>
#include <vector>
#include <string.h>
#include <stdbool.h>
#include<stdio.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

typedef enum {
    noneTyp,//NULL
    customObj,//自定义类型
    Const,//常量
    Var,//变量
    Func,//函数
    Array,//数组型
    Int,//整型
    Char,//字符型
    String,//字符串型
    Void,//void
    aNum,//一个数字
    aChar,//一个字符
    aArray,//一个数组
    aString,//一个字符串
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
    Plus,//+
    sub,//-
    mul,//*
    Div,///
    Printf,//printf
    Scanf,//scanf
    semicolon,//分号
    comma,//逗号
    colon,//冒号
    lDoubleQuo,//左双引号
    RDoubleQuo,//右双引号
    lSingleQuo,//左单引号
    rSingleQuo,//右单引号
    singleQuo,//单引号
    doubleQuo,//双引号
    space,//空格
    enter,//回车换行
    Return
} ReservedWord;

typedef enum {
    qNewIntCons,
    qNewCharCons,
    qNewIntVar,
    qNewCharVar,
    qNewIntArray,
    qNewCharArray,
    qLabel,
    qFuncLabel,
    qFuncEndLabel,
    qCallFunc,//调用函数
    qPlus,//加
    qSub,//减
    qMul,//乘
    qDiv,//除
    qJg,//a > b 跳转
    qJl,//a < b 跳转
    qJe,//a == b 跳转
    qJne,//a != b 跳转
    qJge,//a >= b 跳转
    qJle,//a <= b 跳转
    qJ,//无条件跳转
    qJFunc,
    qGetReturnInt,//从v0取函数返回值
    qGetReturnChar,//从v0取函数返回值
    qPutReturnInt,//向v0放返回值
    qPutReturnChar,//向v0放返回值
    qGetArrayValue,//取数组值
    qGetArrayIntValue,//取整型数组值
    qGetArrayCharValue,//取字符型数组值
    qAssignInt,//赋值
    qAssignIntArray,//赋值
    qAssignChar,//赋值
    qAssignCharArray,//赋值
    qReverse,//a取反
    qPassParam,//传参
    qPassIntParam,//传int参
    qPassCharParam,//传int参
    qGetIntParam,//取int参
    qGetCharParam,//取char参
    qReturn,//返回
    qScanf,//输入
    qScanfInt,//输入
    qScanfChar,//输入
    qPrintf,//输出
    qPrintfString,//输出
    qPrintfInt,//输出
    qSaveAddr,//存入内存
    qLoadAddr,//加载至寄存器
} QuadCodeInstr;

//间隔符
typedef struct {
    char symbol;
    ReservedWord reservedWord;
} Interval;

//关键字
typedef struct {
    char* symbol;
    ReservedWord reservedWord;
    char* rwToString;
} Keyword;

//符号表项
typedef struct {
    string name;//token名
    ReservedWord obj;//种类（常量，变量，函数，字符串）
    ReservedWord type;//类型（整型，字符型, Void）
    bool isArray = false;//是否是数组
    int intValue = 0;//如果是整型，记录int值
    char charValue = '\0';//如果是字符型，记录char值
    int refToOtherTable;//连接其他表的索引
    int address;//所处内存的地址
    ReservedWord returnType;//函数返回值类型
    int paramAddr;//如果是函数名则记录在函数参数表的起始地址
    int paramAccount;//如果是函数名则记录参数表参数的个数
    int localVarAccount;//记录函数局部变量的个数
    char content[1024];//如果是字符串则记录内容，其他为’\0’
    int arraySize = 1;//如果是数组则记录数组大小，其他为1
} TokenTableItem;

//代码表项
typedef struct {
    string name;//指令名
    string operand1;//第一个操作数
    string operand2;//第二个操作数
    string operand3;//第三个操作数
} CodeTableItem;

//四元式表项
typedef struct {
    QuadCodeInstr quadCodeInstr;//四元式指令
    string operand1;//操作数1
    string operand2;//操作数2
    string result;//结果
} QuadCodeTableItem;

//函数参数表项
typedef struct {
    string name;//参数名
    ReservedWord type;//参数类型
    ReservedWord obj;//记录是否为常量
    int intValue;//如果是常量声明则记录声明的int值
    char charValue;//如果是常量声明则记录声明的字符值
    bool isArray;//记录是否是数组
    int arraySize;//如果是数组则记录数组的大小
    string addr;//相对于基地址的地址
} FuncParamTableItem;

string getStrQCode(QuadCodeInstr instr) ;
void init() ;
void printTokenTable() ;
void printFuncParamTable() ;
bool isinterval(char c) ;
bool isKeyword(char* s) ;
bool isNumber(char* s) ;
bool isANumber(string s) ;
bool isChar(char* s) ;
string itoa(int i) ;
string ctoa(char a) ;
string getNextLabel() ;
string getNextTmpVar() ;
void printAllQCode() ;
string getFuncLabel(string funcName) ;
char getNextChar() ;
void pushBackChar() ;
void clearSymbol() ;
void symbolAppend(char c) ;
char* getNextSymbol() ;
void getNextSymbolAndType() ;
char* getTypeToString(char* symbol) ;
void output(char* s) ;
void printAllSymbol() ;
void jumpToNextLine() ;
int findTokenInTable(string symbol) ;
bool findFuncParamTable(TokenTableItem* tti, char* symbol) ;
vector<string> findInAllTable(string symbol) ;
void dealOverallConst() ;
void dealInnerConst(TokenTableItem *ptr);
void dealOverallVar(ReservedWord recordType) ;
void dealInnerVar(TokenTableItem *tti, ReservedWord param);
void dealAssign() ;
void dealPassParam(int index) ;
string dealCallFunc(int index) ;
string dealFactor() ;
string dealTerm() ;
string dealExpression() ;
void dealCondition(string label, string op);
void dealForStep() ;
void dealFor() ;
void dealDoWhile() ;
void dealIf() ;
void dealStatement() ;
void dealStatements() ;
void dealParamlist(TokenTableItem *tti) ;
void dealFunc(TokenTableItem *tti) ;
void grammaticalAnalysis() ;
void emitQCode(QuadCodeInstr quadCodeInstr, string operand1, string operand2, string result) ;

#endif //COMPILER_ANALYSIS_H
