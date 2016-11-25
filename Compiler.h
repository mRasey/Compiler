//
// Created by 王震 on 2016/11/7.
//

#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

#include <string>

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
    qGetArrayValue,//取数组值
    qAssignInt,//赋值
    qAssignIntArray,//赋值
    qAssignChar,//赋值
    qAssignCharArray,//赋值
    qReverse,//a取反
    qPassParam,//传参
    qReturn,//返回
    qScanf,//输入
    qPrintf,//输出
    qSaveAddr,//存入内存
    qLoadAddr//加载至寄存器
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
    char name[1024];//指令名
    char operand1[1024];//第一个操作数
    char operand2[1024];//第二个操作数
    char operand3[1024];//第三个操作数
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
QuadCodeTableItem qCodeInstrs[1024];

int qCodePointer = 0;
int labelPointer = 0;
int tmpVarPointer = 0;

string currentDealFunc = "";
//string label1;
//string label2;
//string label3;


string getStrQCode(QuadCodeInstr instr) {
    if(instr == qNewIntCons)
        return "qNewIntCons";
    if(instr == qNewCharCons)
        return "qNewCharCons";
    if(instr == qNewIntVar)
        return "qNewIntVar";
    if(instr == qNewCharVar)
        return "qNewCharVar";
    if(instr == qNewIntArray)
        return "qNewIntArray";
    if(instr == qNewCharArray)
        return "qNewCharArray";
    if(instr == qFuncLabel)
        return "qFuncLabel";
    if(instr == qFuncEndLabel)
        return "qFuncEndLabel";
    if(instr == qLabel)
        return "qLabel";
    if(instr == qCallFunc)
        return "qCallFunc";
    if(instr == qPlus)
        return "qPlus";
    if(instr == qSub)
        return "qSub";
    if(instr == qMul)
        return "qMul";
    if(instr == qDiv)
        return "qDiv";
    if(instr == qJg)
        return "qJg";
    if(instr == qJl)
        return "qJl";
    if(instr == qJe)
        return "qJe";
    if(instr == qJne)
        return "qJne";
    if(instr == qJge)
        return "qJge";
    if(instr == qJle)
        return "qJle";
    if(instr == qJ)
        return "qJ";
    if(instr == qJFunc)
        return "qJFunc";
    if(instr == qGetArrayValue)
        return "qGetArrayValue";
    if(instr == qAssignInt)
        return "qAssignInt";
    if(instr == qAssignChar)
        return "qAssignChar";
    if(instr == qAssignIntArray)
        return "qAssignIntArray";
    if(instr == qAssignCharArray)
        return "qAssignCharArray";
    if(instr == qReverse)
        return "qReverse";
    if(instr == qPassParam)
        return "qPassParam";
    if(instr == qReturn)
        return "qReturn";
    if(instr == qScanf)
        return "qScanf";
    if(instr == qPrintf)
        return "qPrintf";
    if(instr == qSaveAddr)
        return "qSaveAddr";
    if(instr == qLoadAddr)
        return "qLoadAddr";
    return "errType";
}

/**
 * 初始化间隔符
 */
void init() {
    intervals[0].symbol = '+';
    intervals[1].symbol = '-';
    intervals[2].symbol = '*';
    intervals[3].symbol = '/';
    intervals[4].symbol = '>';
    intervals[5].symbol = '<';
    intervals[6].symbol = '=';
    intervals[7].symbol = '{';
    intervals[8].symbol = '}';
    intervals[9].symbol = '[';
    intervals[10].symbol = ']';
    intervals[11].symbol = '(';
    intervals[12].symbol = ')';
    intervals[13].symbol = '\'';
    intervals[14].symbol = '\"';
    intervals[15].symbol = ':';
    intervals[16].symbol = ';';
    intervals[17].symbol = ',';
    intervals[18].symbol = ' ';
    intervals[19].symbol = '\n';
    intervals[20].symbol = '!';
    intervalsLength = 21;

    keywords[0].symbol = "int";
    keywords[0].reservedWord = Int;
    keywords[0].rwToString = "Int";
    keywords[1].symbol = "char";
    keywords[1].reservedWord = Char;
    keywords[1].rwToString = "Char";
    keywords[2].symbol = "for";
    keywords[2].reservedWord = For;
    keywords[2].rwToString = "For";
    keywords[3].symbol = "while";
    keywords[3].reservedWord = While;
    keywords[3].rwToString = "While";
    keywords[4].symbol = "do";
    keywords[4].reservedWord = Do;
    keywords[4].rwToString = "Do";
    keywords[5].symbol = "if";
    keywords[5].reservedWord = If;
    keywords[5].rwToString = "If";
    keywords[6].symbol = "else";
    keywords[6].reservedWord = Else;
    keywords[6].rwToString = "Else";
    keywords[7].symbol = "printf";
    keywords[7].reservedWord = Printf;
    keywords[7].rwToString = "Printf";
    keywords[8].symbol = "scanf";
    keywords[8].reservedWord = Scanf;
    keywords[8].rwToString = "Scanf";
    keywords[9].symbol = "const";
    keywords[9].reservedWord = Const;
    keywords[9].rwToString = "Const";
    keywords[10].symbol = ">";
    keywords[10].reservedWord = big;
    keywords[10].rwToString = "Big";
    keywords[11].symbol = "<";
    keywords[11].reservedWord = small;
    keywords[11].rwToString = "small";
    keywords[12].symbol = ">=";
    keywords[12].reservedWord = bigAndEql;
    keywords[12].rwToString = "bigAndEql";
    keywords[13].symbol = "<=";
    keywords[13].reservedWord = smallAndEql;
    keywords[13].rwToString = "smallAndEql";
    keywords[14].symbol = "==";
    keywords[14].reservedWord = eql;
    keywords[14].rwToString = "eql";
    keywords[15].symbol = "=";
    keywords[15].reservedWord = assign;
    keywords[15].rwToString = "assign";
    keywords[16].symbol = "!=";
    keywords[16].reservedWord = notEql;
    keywords[16].rwToString = "notEql";
    keywords[17].symbol = "+";
    keywords[17].reservedWord = Plus;
    keywords[17].rwToString = "plus";
    keywords[18].symbol = "-";
    keywords[18].reservedWord = sub;
    keywords[18].rwToString = "sub";
    keywords[19].symbol = "*";
    keywords[19].reservedWord = mul;
    keywords[19].rwToString = "mul";
    keywords[20].symbol = "/";
    keywords[20].reservedWord = Div;
    keywords[20].rwToString = "Div";
    keywords[21].symbol = "{";
    keywords[21].reservedWord = lBrace;
    keywords[21].rwToString = "lBrace";
    keywords[22].symbol = "}";
    keywords[22].reservedWord = rBrace;
    keywords[22].rwToString = "rBrace";
    keywords[23].symbol = "[";
    keywords[23].reservedWord = lBracket;
    keywords[23].rwToString = "lBracket";
    keywords[24].symbol = "]";
    keywords[24].reservedWord = rBracket;
    keywords[24].rwToString = "rBracket";
    keywords[25].symbol = "(";
    keywords[25].reservedWord = lParent;
    keywords[25].rwToString = "lParent";
    keywords[26].symbol = ")";
    keywords[26].reservedWord = rParent;
    keywords[26].rwToString = "rParent";
    keywords[27].symbol = ":";
    keywords[27].reservedWord = colon;
    keywords[27].rwToString = "colon";
    keywords[28].symbol = ",";
    keywords[28].reservedWord = comma;
    keywords[28].rwToString = "comma";
    keywords[29].symbol = ";";
    keywords[29].reservedWord = semicolon;
    keywords[29].rwToString = "semicolon";

    keywords[30].symbol = "void";
    keywords[30].reservedWord = Void;
    keywords[30].rwToString = "Void";

    keywords[31].symbol = "return";
    keywords[31].reservedWord = Return;
    keywords[31].rwToString = "return";
    keywordsLength = 32;
}


#endif //COMPILER_COMPILER_H
