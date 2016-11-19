//
// Created by 王震 on 2016/11/7.
//

#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

//using namespace std;

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
    plus,//+
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
    enter//回车换行
} ReservedWord;

typedef enum {
    qAdd,//加
    qSub,//减
    qMul,//乘
    qDiv,//除
    qJa,//a > b 跳转
    qJb,//a < b 跳转
    qJe,//a == b 跳转
    qJne,//a != b 跳转
    qJbe,//a >= b 跳转
    qJle,//a <= b 跳转
    qReverse,//a取反
    qAssign,//赋值
    qCall,//调用函数
    qPrintf,//输出
    qScanf,//输入
    qJ,//无条件跳转
} QuadCodeInstr;

typedef struct {
    char symbol;
    ReservedWord reservedWord;
} Interval;

typedef struct {
    char* symbol;
    ReservedWord reservedWord;
    char* rwToString;
} Keyword;

//符号表项
typedef struct {
    char name[1024];//token名
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
    char name[1024];//指令名
    char operand1[1024];//第一个操作数
    char operand2[1024];//第二个操作数
    char operand3[1024];//第三个操作数
} CodeTableItem;

//四元式表项
typedef struct {
    QuadCodeInstr quadCodeInstr;//四元式指令
    char operand1[1024];//操作数1
    char operand2[1024];//操作数2
    char result[1024];//结果
} QuadCodeTableItem;

//函数参数表项
typedef struct {
    char name[1024];//参数名
    ReservedWord type;//参数类型
    ReservedWord obj;//记录是否为常量
    int intValue;//如果是常量声明则记录声明的int值
    char charValue;//如果是常量声明则记录声明的字符值
    bool isArray;//记录是否是数组
    int arraySize;//如果是数组则记录数组的大小
} FuncParamTableItem;


Interval intervals[100];//间隔符表
Keyword keywords[100];//关键字表
TokenTableItem tokenTable[1024];//符号表
int tokenTablePointer = 1;//符号表指针，从1开始，0表示未找到
CodeTableItem codeTable[1024];//代码表
int codeTablePointer;//代码表指针
QuadCodeTableItem quadCodeTable[1024];//四元式表
int quadCodeTablePointer;//四元式表指针
FuncParamTableItem funcParamTable[1024];//函数表
int funcParamTablePointer;//函数表指针

ReservedWord reservedWord;//当前处理的保留字
FILE* in;
char readIn[1024];
int lineNowPos = 0;
int lineLength;
char nowChar;
char symbol[1024];
ReservedWord symbolType;
char* symbolTypeToString;
int symPos = 0;
int intervalsLength;
int keywordsLength;


#endif //COMPILER_COMPILER_H
