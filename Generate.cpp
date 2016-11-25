//
// Created by 王震 on 2016/11/25.
//

#include "Generate.h"
#include "Analysis.h"

extern Interval intervals[100];//间隔符表
extern Keyword keywords[100];//关键字表
extern TokenTableItem tokenTable[1024];//符号表
extern int tokenTablePointer;//符号表指针，从1开始，0表示未找到
extern CodeTableItem codeTable[1024];//代码表
extern int codeTablePointer;//代码表指针
extern QuadCodeTableItem quadCodeTable[1024];//四元式表
extern int quadCodeTablePointer;//四元式表指针
extern FuncParamTableItem funcParamTable[1024];//函数表
extern int funcParamTablePointer;//函数表指针
extern ReservedWord reservedWord;//当前处理的保留字
extern FILE* in;
extern char readIn[1024];
extern int lineNowPos;
extern int lineLength;
extern char symbol[1024];
extern ReservedWord symbolType;
extern char* symbolTypeToString;
extern int symPos;
extern int intervalsLength;
extern int keywordsLength;
extern QuadCodeTableItem qCodeInstrs[1024];//四元式表
extern int qCodePointer;//四元式指针
extern int labelPointer;//标签指针
extern int tmpVarPointer;//临时变量指针
extern string currentDealFunc;//当前处理函数


/**
 * 生成MIPS代码
 */
void generate() {

}
