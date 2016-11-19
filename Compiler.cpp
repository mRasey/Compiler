//
// Created by 王震 on 2016/11/7.
//

#include <string.h>
#include <stdbool.h>
#include<stdio.h>
#include <cstdlib>
#include "Compiler.h"

void printTokenTable() ;
void printFuncParamTable() ;
void init() ;
bool isinterval(char c) ;
bool isKeyword(char* s) ;
bool isNumber(char* s) ;
bool isChar(char* s) ;
char getNextChar() ;
void pushBackChar() ;
void clearSymbol() ;
void symbolAppend(char c) ;
char* getNextSymbol() ;
void getNextSymbolAndType() ;
char* getTypeToString(char* symbol) ;
void cout(char* s) ;
void printAllSymbol() ;
void jumpToNextLine() ;
int findTokenInTable(char* symbol) ;
bool findFuncParamTable(TokenTableItem* tti, char* symbol) ;
void dealOverallConst() ;
void dealInnerConst(TokenTableItem *ptr);
void dealOverallVar(ReservedWord recordType) ;
void dealInnerVar(TokenTableItem *tti, ReservedWord param);
void dealAssign() ;
int dealCallFunc() ;
int dealFactor() ;
void dealTerm() ;
int dealExpression() ;
int dealCondition() ;
void dealForStep() ;
void dealFor() ;
void dealDoWhile() ;
void dealIf() ;
void dealStatement() ;
void dealStatements() ;
void dealParamlist(TokenTableItem *tti) ;
void dealFunc(TokenTableItem *tti) ;
void grammaticalAnalysis() ;

/**
 * 打印出符号表
 */
void printTokenTable() {
    for(int i = 0; i < 10; i++) {
        TokenTableItem *tti = &tokenTable[i];
        if(tti->obj == Func)
            printf("%s %d\n", tti->name, tti->paramAccount);
    }
}

/**
 * 打印函数参数表
 */
void printFuncParamTable() {
    for(int i = 0; i < funcParamTablePointer; i++) {
        FuncParamTableItem *fpti = &funcParamTable[i];
        printf("%s\n", fpti->name);
    }
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
    keywords[17].reservedWord = plus;
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
    keywordsLength = 31;
}

/**
 * 判断是否是一个间隔符
 * @param c 要判断的字符
 * @return 如果是间隔符返回真，否则返回false
 */
bool isinterval(char c) {
    for(int i = 0; i < intervalsLength; i++) {
        if(intervals[i].symbol == c)
            return true;
    }
    return false;
}

/**
 * 判断是否是一个关键字
 * @param s 要判断的字符
 * @return 如果是关键字返回true，否则返回false
 */
bool isKeyword(char* s) {
    for(int i = 0; i < keywordsLength; i++) {
        if(strcpy(keywords[i].symbol, s) == 0)
            return true;
    }
    return false;
}

/**
 * 判断一个字符串是否是数字
 * @param s 要判断的字符
 * @return 如果是数字返回true，否则返回false
 */
bool isNumber(char* s) {
    for(int i = 0; i < strlen(s); i++) {
        if(s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

/**
 * 判断字符串s是否是一个合法字符
 * @param s 要判断的字符串
 * @return 如果是单个合法字符返回true，否则返回false
 */
bool isChar(char* s) {
    if(strlen(s) != 1)
        return false;
    return s[0] == '+'
           || s[0] == '-'
           || s[0] == '_'
           || ('0' <= s[0] && s[0] <= '9')
           || ('A' <= s[0] && s[0] <= 'z');
}

/**
 * 从当前缓冲行中获取下一个字符
 * @return 下一个字符
 */
char getNextChar() {
    return readIn[lineNowPos++];
}

/**
 * 向缓冲区中放回一个字符
 */
void pushBackChar() {
    lineNowPos--;
}

/**
 * 清空symbol的内容同时将位置指针置为0
 */
void clearSymbol() {
    for( ; symPos >= 0; symPos--)
        symbol[symPos] = '\0';
    symPos = 0;
}

/**
 * 将字符加在当前token的后面
 * @param c 要添加的字符
 */
void symbolAppend(char c) {
    symbol[symPos++] = c;
}

/**
 * 获取下一个token
 * @return 返回下一个找到的token
 */
char* getNextSymbol() {
    clearSymbol();//清空symbol
    symbolType = noneTyp;
    symbolTypeToString = "";
    bool firstScan = true;
    if(lineNowPos == lineLength) { //如果当前行已经读取完，读取下一行进缓冲区
        if(!feof(in)) {
            fgets(readIn, 1024, in);
        } else {
//            printf("end of the file\n");  //如果下一行为EOF，则返回null
            return NULL;
        }
        lineLength = (int) strlen(readIn);
        lineNowPos = 0;
    }
    while(1) {
        char temp = getNextChar();//获取下一个字符
        if(isinterval(temp)) {  //如果是一个间隔符
            if(firstScan == false) {
                pushBackChar();
                return symbol;
            }
            firstScan = false;
            if(temp == '!' || temp == '>' || temp == '<' || temp == '=') {
                symbolAppend(temp);
                temp = getNextChar();
                if(temp == '=') {   //表明是>=或<=或!=或==
                    symbolAppend(temp);
                    return symbol;
                }
                else {
                    pushBackChar();  //将读取的字符放回缓冲区，只返回>或<或!或=
                    return symbol;
                }
            }
            else if(temp == '{' || temp == '}' || temp == '[' || temp == ']' || temp == '(' || temp == ')'
                    || temp == ',' || temp == ':' || temp == ';') {
                symbolAppend(temp);
                return symbol;
            }
            else if(temp == '\'' || temp == '\"') {   //如果是一个字符或字符串声明
                if(temp == '\'') {
                    symbolType = aChar;
                    symbolTypeToString = "aChar";
                }
                else {
                    symbolType = aString;
                    symbolTypeToString = "aString";
                }
                char oldTemp = temp;
                do {
                    temp = getNextChar();
                    if(temp == oldTemp || lineNowPos == lineLength)  //读到另一个匹配的引号或者一行读取完毕，跳出循环
                        break;
                    symbolAppend(temp);
                }while(1);
            }
            else if (temp == '+' || temp == '-' || temp == '*' || temp == '/')  {
                symbolAppend(temp);
                return symbol;
            }
            else {
                if(strcmp(symbol, "") == 0)  //如果发现当前的symbol为空，则返回寻找到的下一个symbol
                    return getNextSymbol();
                return symbol;
            }
        }
        else {  //如果不是间隔符，直接添加进symbol
            symbolAppend(temp);
            firstScan = false;
        }
    }
}

/**
 * 从关键字表中找到相应的类型
 * @param symbol 要寻找的关键字
 * @return 如果找到则返回类型，否则返回用户自定义类型
 */
void getNextSymbolAndType() {
    getNextSymbol();//获取下一个token
    if(symbolType != noneTyp) {
        symbolType = symbolType;
        return;
    }
    if(isNumber(symbol)) {
        symbolType = aNum;
        return;
    }
    for(int i = 0; i < keywordsLength; i++) {
        if(strcmp(keywords[i].symbol, symbol) == 0) {
            symbolType = keywords[i].reservedWord;
            return;
        }
    }
    symbolType = customObj;
}

/**
 * 从关键字表中找到相应的类型
 * @param symbol 要寻找的关键字
 * @return 如果找到返回类型，否则返回用户自定义类型
 */
char* getTypeToString(char* symbol) {
    if(symbolType != noneTyp)
        return symbolTypeToString;
    if(isNumber(symbol))
        return "aNum";
    for(int i = 0; i < keywordsLength; i++) {
        if(strcmp(keywords[i].symbol, symbol) == 0)
            return keywords[i].rwToString;
    }
    return "customObj";
}

/**
 * 按类型输出结果
 * @param s token的类型toString
 */
void cout(char* s) {
    printf("%s: %s\n", s, symbol);
}

/**
 * 输出所有的symbol
 * @return
 */
void printAllSymbol() {
    int i = 1;
    while(true) {
        if (!feof(in)) {
            getNextSymbol();
            printf("%d ", i++);
            cout(getTypeToString(symbol));
        }
        else
            return;
    }
}

/**
 * 跳过当前代码行(以;作为行结束标志),返回下一行的第一个symbol
 */
void jumpToNextLine() {
    getNextSymbolAndType();
    while(symbolType != semicolon)
        getNextSymbolAndType();
    getNextSymbolAndType();
}

/**
 * 从符号表中找到相应的符号的下标
 * @param symbol 要寻找的符号
 * @return 找到的token的下标，如果没有找到返回0
 */
int findTokenInTable(char* symbol) {
    for(int i = tokenTablePointer; i > 0; i--) {
        TokenTableItem tti = tokenTable[i];
        if(strcmp(tti.name, symbol) == 0)
            return i;
    }
    return 0;
}

/**
 * 从函数参数表中找到相应的局部变量
 * @param symbol 要寻找的token
 * @return 如果能找到返回true，否则返回false
 */
bool findFuncParamTable(TokenTableItem* tti, char* symbol) {
    int startIndex = tti->paramAddr;//找到在函数表中的指针
    int paramCount = tti->paramAccount;//找到参数的数目
    int localVarCount = tti->localVarAccount;//找到局部变量的数目
    for(int i = startIndex; i < startIndex + paramCount + localVarCount; i++) {
        if(strcmp(funcParamTable[i].name, symbol) == 0)
            return true;
    }
    return false;
}

/**
 * 处理全局常量声明
 */
void dealOverallConst() {
    printf("this is a overall const declare\t\t\t%s\n", readIn);
    while(symbolType != semicolon) {
        TokenTableItem *tti = &tokenTable[tokenTablePointer++];
        getNextSymbolAndType();
        if(symbolType == Int) {
            do {
                tti->type = Int;//记录类型
                tti->obj = Const;//记录种类
                getNextSymbol();//读取常量名
                strcpy(tti->name, symbol);//记录变量名
                //todo 重定义的容错处理
                getNextSymbol();//读取=号
                //todo 容错
                getNextSymbol();//读取常量值
                if (isNumber(symbol))
                    tti->intValue = atoi(symbol);//记录常量值
                else {
                    printf("err: not a number");
                    tti->intValue = 0;//否则报错并赋值为0
                }
                getNextSymbolAndType();
            } while(symbolType == comma && (tti = &tokenTable[tokenTablePointer++]));//如果是逗号则继续读取
            //todo 非逗号的容错处理
        }
        else if(symbolType == Char) {
            do {
                tti->type = Char;//记录类型
                tti->obj = Const;//记录种类
                getNextSymbol();//读取常量名
                strcpy(tti->name, symbol);//记录常量名
                //todo 重定义的容错处理
                getNextSymbol();//读取=号
                //todo 容错
                getNextSymbol();//读取常量值
                if (isChar(symbol))
                    tti->charValue = symbol[0];//将字符串转为单个字符
                else {
                    printf("err: not a char");
                    //todo 只取第一个合法字符
                    tti->charValue = symbol[0];//否则报错并只取第一个字符
                }
                getNextSymbolAndType();
            } while(symbolType == comma && (tti = &tokenTable[tokenTablePointer++]));//如果是逗号则继续读取
            //todo 非逗号的容错处理
        }
        else {
            printf("err type in overall\n");
            jumpToNextLine();
            break;
        }
    }
}

/**
 * 处理函数内常量声明
 */
void dealInnerConst(TokenTableItem *tti) {
    printf("this is a inner const declare\t\t\t%s\n", readIn);
    while(symbolType != semicolon) {
        FuncParamTableItem *fpti = &funcParamTable[funcParamTablePointer++];
        getNextSymbolAndType();
        if(symbolType == Int) {
            do {
                getNextSymbolAndType();
                if(!findFuncParamTable(tti, symbol)) { //如果在函数参数表中没有找到同名的变量则将其加入表中
                    fpti->type = Int;//记录类型
                    fpti->obj = Const;//记录种类
//                    getNextSymbolAndType();//读取常量名
                    strcpy(fpti->name, symbol);//记录变量名
                    getNextSymbolAndType();//读取=号
                    //todo 容错
                    getNextSymbol();//读取常量值
                    if (isNumber(symbol))
                        fpti->intValue = atoi(symbol);//记录常量值
                    else {
                        printf("err: not a number");
                        fpti->intValue = 0;//否则报错并赋值为0
                    }
                    getNextSymbolAndType();
                    tti->localVarAccount++;//将局部变量的计数加一
                }
                else {
                    //todo 重定义常量的容错处理
                    printf("multi declare in inner const\n");
                    getNextSymbolAndType();//=
                    getNextSymbolAndType();//value
                    getNextSymbolAndType();//, or ;
                    funcParamTablePointer--;//指针退回之前的位置
                }
            } while(symbolType == comma && (fpti = &funcParamTable[funcParamTablePointer++]));//如果是逗号则继续读取
            //todo 非逗号的容错处理
        }
        else if(symbolType == Char) {
            do {
                getNextSymbolAndType();
                if(!findFuncParamTable(tti, symbol)) { //如果在函数参数表中没有找到同名的变量则将其加入表中
                    fpti->type = Char;//记录类型
                    fpti->obj = Const;//记录种类
//                    getNextSymbolAndType();//读取常量名
                    strcpy(fpti->name, symbol);//记录常量名
                    getNextSymbolAndType();//读取=号
                    //todo 容错
                    getNextSymbol();//读取常量值
                    if (isChar(symbol))
                        fpti->charValue = symbol[0];//将字符串转为单个字符
                    else {
                        printf("err: not a char");
                        //todo 只取第一个合法字符
                        fpti->charValue = symbol[0];//否则报错并只取第一个字符
                    }
                    getNextSymbolAndType();
                    tti->localVarAccount++;//将局部变量的计数加一
                }
                else {
                    //todo 常量重定义的容错处理
                    printf("multi declare in inner const\n");
                    getNextSymbolAndType();//=
                    getNextSymbolAndType();//value
                    getNextSymbolAndType();//, or ;
                    funcParamTablePointer--;//指针退回之前的位置
                }
            } while(symbolType == comma && (fpti = &funcParamTable[funcParamTablePointer++]));//如果是逗号则继续读取
            //todo 非逗号的容错处理
        }
        else {
            printf("err type in inner\n");
            jumpToNextLine();
            break;
        }
    }
}

/**
 * 处理全局变量声明
 */
void dealOverallVar(ReservedWord recordType) {
    printf("this is a overall variable declare\t\t\t%s\n", readIn);
    while(symbolType != semicolon) {
        TokenTableItem *tti = &tokenTable[tokenTablePointer++];//获取下一个表项
        tti->type = recordType;
        tti->obj = Var;//记录种类
        getNextSymbolAndType();
        if(symbolType == customObj) {
            strcpy(tti->name, symbol);
            //todo 重定义的容错处理
            getNextSymbolAndType();
            if(symbolType == comma) //如果是逗号表示当前一个变量声明完毕，继续当前行的分析
                continue;
            else if(symbolType == semicolon) //如果是分号表示当前行分析完毕，跳出循环
                break;
            else if(symbolType == lBracket) { //如果是左中括号表示是一个数组
                tti->isArray = true;
                getNextSymbolAndType();
                if(isNumber(symbol)) { //如果数组下标是合法数字
                    tti->arraySize = atoi(symbol); //记录数组的大小
                    getNextSymbolAndType();
                    if(symbolType == rBracket) {
                        getNextSymbolAndType();
                        if(symbolType == comma) //如果是逗号则继续分析
                            continue;
                        else if(symbolType == semicolon) //如果是分号则结束当前行的分析
                            break;
                        else {
                            //todo 未出现合法分隔符的容错处理
                        }
                    }
                    else {
                        //todo 数组缺少右中括号的容错处理
                    }
                }
                else {
                    //todo 数组下标非数组的容错处理
                }
            }
            else {
                //todo 出现非分隔符的容错处理
            }
        }
        else {
            //todo 出现保留字的容错处理
        }
    }
}

/**
 * 处理函数内变量声明
 */
void dealInnerVar(TokenTableItem *tti, ReservedWord recordType) {
    printf("this is a inner variable declare\t\t\t%s\n", readIn);
    while(symbolType != semicolon) {
        FuncParamTableItem *fpti = &funcParamTable[funcParamTablePointer++];//获取下一个表项
        fpti->type = recordType;//类型记录为传进来的类型
        fpti->obj = Var;//记录种类
        getNextSymbolAndType();
        if(symbolType == customObj) {
            if(!findFuncParamTable(tti, symbol)) {
                tti->localVarAccount++;//将记录的函数局部变量值加一
                strcpy(fpti->name, symbol);
                //todo 重定义的容错处理
                getNextSymbolAndType();
                if (symbolType == comma) //如果是逗号表示当前一个变量声明完毕，继续当前行的分析
                    continue;
                else if (symbolType == semicolon) //如果是分号表示当前行分析完毕，跳出循环
                    break;
                else if (symbolType == lBracket) { //如果是左中括号表示是一个数组
                    fpti->isArray = true;
                    getNextSymbolAndType();
                    if (isNumber(symbol)) { //如果数组下标是合法数字
                        fpti->arraySize = atoi(symbol); //记录数组的大小
                        getNextSymbolAndType();
                        if (symbolType == rBracket) {
                            getNextSymbolAndType();
                            if (symbolType == comma) //如果是逗号则继续分析
                                continue;
                            else if (symbolType == semicolon) //如果是分号则结束当前行的分析
                                break;
                            else {
                                //todo 未出现合法分隔符的容错处理
                            }
                        } else {
                            //todo 数组缺少右中括号的容错处理
                        }
                    } else {
                        //todo 数组下标非数组的容错处理
                    }
                } else {
                    //todo 出现非分隔符的容错处理
                }
            }
            else {
                //todo 变量重定义的容错处理
                printf("multi declare in inner var\n");
                getNextSymbolAndType();//,
                funcParamTablePointer--;//退回之前的位置
                continue;
            }
        }
        else {
            //todo 出现保留字的容错处理
        }
    }
}

/**
 * 处理赋值语句
 * @return
 */
void dealAssign() {
    printf("this is a assign statement\t\t\t%s\n", readIn);
//    while(symbolType != semicolon) {
//        getNextSymbolAndType();
//    }
    getNextSymbolAndType();
    int index = findTokenInTable(symbol);
    if(index != 0) {
        getNextSymbolAndType();
        if(symbolType == assign) {
            dealExpression();
        }
        else {
            //todo 缺少赋值号的容错处理
        }
    }
    else {
        //todo 使用未定义的变量的容错处理
    }
}

/**
 * 处理返回int的函数调用
 */
int dealCallFunc() {
    return 0;
}

/**
 * 处理因子
 * @return 返回因子的值
 */
int dealFactor() {
    printf("this is a factor\t\t\t%s\n", readIn);
//    getNextSymbolAndType();
    int index = findTokenInTable(symbol);
    if(index != 0) { //如果是一个标识符
        if(tokenTable[index].isArray) { //如果是数组型
            getNextSymbolAndType();//[
            getNextSymbolAndType();//
            if(isNumber(symbol)) {
                //todo 计算数组下标相应的值
            }
            else {
                dealExpression();
            }
        }
        else if(tokenTable[index].obj == Func) { //如果是一个函数
            if(tokenTable[index].returnType == Int) { //return int
                int result = dealCallFunc();
            }
            else if(tokenTable[index].returnType == Char) { //return char
                char result = (char) dealCallFunc();
            }
            else { //return void
                //todo 使用了无返回值函数的容错处理
            }
        }
        else { //非数组及函数的标识符
            if(tokenTable[index].type == Int) //如果是int型变量返回int值
                return tokenTable[index].intValue;
            else if(tokenTable[index].type == Char){ //如果是char型返回char值
                return tokenTable[index].charValue;
            }
            else {
                printf("unexpected error in dealFactor\n");
            }
        }
    }
    else if(isNumber(symbol)) { //如果是一个数字

    }
    else if(isChar(symbol)) { //如果是一个字符

    }
    else if(symbolType == lParent) { //如果是一个右括号
        dealExpression();
    }
    else {
        //todo 不合法的因子的容错处理
    }
    return 0;
}

/**
 * 处理项
 */
void dealTerm() {
    printf("this is a term\t\t\t%s\n", readIn);
    dealFactor();//处理第一个因子
    while(true) {
        getNextSymbolAndType();
        if(symbolType == mul || symbolType == Div) {
            dealFactor();//处理接下来的一个因子
        }
        else
            break;
    }
}

/**
 * 处理表达式
 */
int dealExpression() {
    printf("this is a expression\t\t\t%s\n", readIn);
    getNextSymbolAndType();
    if(symbolType == plus || symbolType == sub) {
        getNextSymbolAndType();
        dealTerm();
    }
    else {
        dealTerm();
    }
    while(true) {
//        getNextSymbolAndType();
        if(symbolType == plus || symbolType == sub) {
            getNextSymbolAndType();
            dealTerm();
        }
        else if(symbolType == rParent || symbolType == semicolon) //读到分号或者右括号表示表达式结束
            break;
        else if(symbolType == big || symbolType == small || symbolType == bigAndEql
                || symbolType == smallAndEql || symbolType == eql || symbolType == notEql) {
            break;
        }
        else {
            getTypeToString(symbol);
            printf("unexpected error in dealExpression\n");
        }
    }
    return 0;
}

/**
 * 处理条件语句，读到)或;为止
 * @return 返回条件语句的结果，如果为假返回0，否则返回1
 */
int dealCondition() {
    printf("this is a condition\t\t\t%s\n", readIn);
    int op1, op2;
    op1 = dealExpression();//处理第一个表达式并获取结果
//    getNextSymbolAndType();
    if(symbolType == big || symbolType == small || symbolType == bigAndEql
            || symbolType == smallAndEql || symbolType == eql || symbolType == notEql) {
        op2 = dealExpression();//处理第二个表达式并获取结果
        if((symbolType == big && op1 > op2)
                || (symbolType == small && op1 < op2)
                || (symbolType == bigAndEql && op1 >= op2)
                || (symbolType == smallAndEql && op1 <= op2)
                || (symbolType == eql && op1 == op2)
                || (symbolType == notEql && op1 != op2)) {
            return 1;
        }
        else
            return 0;
    }
    else if(symbolType == rParent) { //如果读取到右括号表明条件只有一个表达式
        return op1;
    }
    else
        return 0;
}

/**
 * 处理for循环步长语句
 */
void dealForStep() {
    printf("this is a for step\t\t\t%s\n", readIn);
    getNextSymbolAndType();//结果标识符
    if(tokenTable[findTokenInTable(symbol)].obj == Var) { //如果能找到并且是一个变量
        getNextSymbolAndType();//=号
        if(symbolType == assign) {
            getNextSymbolAndType();//运算标识符
            if(tokenTable[findTokenInTable(symbol)].type == Int
                    || tokenTable[findTokenInTable(symbol)].obj == Char) {
                getNextSymbolAndType();//加法运算符
                if(symbolType == plus || symbolType == sub) {
                    getNextSymbolAndType();//步长
                    if(isNumber(symbol)) {
                        //todo 生成四元式
                    }
                    else {
                        //todo 步长不是无符号整数的容错处理
                    }
                }
                else {
                    //todo 缺少运算符的容错处理
                }
            }
            else {
                //todo 左操作数不含有有效值得容错处理
            }
        }
        else {
            //todo 缺少赋值号的容错处理
        }
    }
    else {
        //todo 找不到变量或者给常量赋值的容错处理
    }
}

/**
 * 处理for循环
 */
void dealFor() {
    printf("this is a for loop\t\t\t%s\n", readIn);
    getNextSymbolAndType();
    if(symbolType == lParent) {
        dealAssign();//处理赋值语句
        dealCondition();//处理条件语句
        dealForStep();//处理for循环跳转步数语句
    }
    else {
        //todo for循环头部容错处理
    }
    getNextSymbolAndType(); //{
    if(symbolType == lBrace) {
        dealStatements();
    }
    else if(symbolType == semicolon){ //如果是分号表示for循环有一个空循环体，则结束当前行处理
        return;
    }
    else {
        //todo for循环头部后接非法符号的容错处理
    }
}

/**
 * 处理do...while...循环
 */
void dealDoWhile() {
    printf("this is a do...while... loop\t\t\t%s\n", readIn);
    getNextSymbolAndType();
    if(symbolType == lBrace) { //如果是左花括号，进入语句列处理
        dealStatements();
    }
    else if(symbolType == While) {
        //todo 缺少循环实体的容错处理
    }
    else { //否则进入语句处理
        dealStatement();
    }
    getNextSymbolAndType();
    if(symbolType == While) {
        getNextSymbolAndType();
        if(symbolType == lParent)
            dealCondition();
        else {
            //todo while后缺少左括号的容错处理
        }
    }
}

/**
 * 处理if条件语句
 */
void dealIf() {
    printf("this is a if...else... struct\t\t\t%s\n", readIn);
    getNextSymbolAndType();//(
    if(symbolType == lParent) { //如果是左括号进入条件处理语句
        dealCondition();
        getNextSymbolAndType();//{
        if(symbolType == lBrace) { //如果读到花括号进入语句列处理
            dealStatements();
        }
        else if(symbolType == Else) {
            //todo if块为空的容错处理
        }
        else {
            dealStatement(); //进入语句处理
        }
        getNextSymbolAndType();//else 或者下一个语句的开始
        if(symbolType == Else) { //如果有else块则开始处理else的语句
            getNextSymbolAndType();
            dealStatements();
        }
        else {
            //todo 读到下一句的句首
        }
    }
    else {
        //todo if后缺少左括号的容错处理
    }
}

/**
 * 处理语句
 */
void dealStatement() {
    //已经获取首个token，不需要重新获取
    printf("this is a statement\t\t\t%s\n", readIn);
    //todo 语句处理
    while(symbolType != semicolon) {
        getNextSymbolAndType();
    }
}

/**
 * 处理语句列
 */
void dealStatements() {
    printf("this is a list of statement\t\t\t%s\n", readIn);
    while(true) {
        getNextSymbolAndType();
        if (symbolType == If) { //进入if...else...循环
            dealIf();
        }
        else if (symbolType == For) { //进入for循环
            dealFor();
        }
        else if (symbolType == Do) { //进入do...while...循环
            dealDoWhile();
        }
        else if(symbolType == Else) {
            //todo 没有if匹配的else的容错处理
        }
        else if(symbolType == rBrace) { //如果读到}则跳出
            break;
        }
        else if(symbolType == lBrace) {
            dealStatements();
        }
        else {
            dealStatement();
        }
    }
}

/**
 * 处理函数参数
 */
void dealParamlist(TokenTableItem *tti) {
    printf("this is a paramlist\t\t\t%s\n", readIn);
    getNextSymbolAndType();
    if(symbolType == rParent) { //如果是空表示没有参数
        tti->paramAccount = 0;
        return;
    }
    else if(symbolType == Int || symbolType == Char){
        tti->paramAddr = funcParamTablePointer;
        while(true) {
            if(symbolType == Int || symbolType == Char) {
                tti->paramAccount++;
                FuncParamTableItem *fpt = &funcParamTable[funcParamTablePointer++];
                fpt->type = symbolType;
                getNextSymbolAndType();
                if (symbolType == customObj) {
                    strcpy(fpt->name, symbol);//记录参数名
                    getNextSymbolAndType();
                    if (symbolType == comma) {
                        getNextSymbolAndType();
                        continue;
                    }
                    else if (symbolType == rParent) {
                        break;
                    }
                    else {
                        //todo 参数表出现非法参数的容错处理
                    }
                }
                else {
                    funcParamTablePointer--;//从参数表中退出未成功识别的参数
                    tti->paramAccount--;
                    //todo 参数表用户自定义类型为保留字的容错处理
                }
            }
        }
    }
    else {
        //todo 参数表格式错误的容错处理
    }
}

/**
 * 处理函数
 */
void dealFunc(TokenTableItem *tti) {
    printf("this is a function\t\t\t%s\n", readIn);
    dealParamlist(tti);
    getNextSymbolAndType();//{
    if(symbolType == lBrace) {
        getNextSymbolAndType();
        while (symbolType == Const) {
            dealInnerConst(tti);
            getNextSymbolAndType();
        }
        while (symbolType == Int || symbolType == Char) {
            dealInnerVar(tti, symbolType);
            getNextSymbolAndType();
        }
        dealStatements();
    }
    else {
        //todo 函数定义缺少花括号的容错处理
    }
}


/**
 * 语法分析程序
 */
void grammaticalAnalysis() {
    while(true) {
        getNextSymbolAndType();
        if (lineNowPos != lineLength || !feof(in)) { //如果没有读到文件末尾就一直往下读
            if(symbolType == Const)
                dealOverallConst();
            else if(symbolType == Int || symbolType == Char) {
                ReservedWord recordType = symbolType;
                TokenTableItem *tti = &tokenTable[tokenTablePointer++];//获取下一个表项
                tti->type = symbolType;//保存类型
                getNextSymbol();//获取下一个标识符
                strcpy(tti->name, symbol);//保存名称
                //todo 重定义容错处理
                getNextSymbolAndType();//获取标识符类型
                if(symbolType == lParent) { //如果是(表示是一个函数
                    tti->obj = Func;
                    dealFunc(tti);
                }
                else if(symbolType == lBracket) { //如果是[表明是一个声明数组变量语句
                    tti->isArray = true;
                    getNextSymbol();//获取下一个token
                    if(isNumber(symbol)) {
                        tti->arraySize = atoi(symbol);
                        //todo 数组声明过大的容错处理
                        getNextSymbolAndType();
                        if(symbolType == rBracket) {
                            getNextSymbolAndType();
                            if(symbolType == semicolon)
                                continue;//继续下一行代码的处理
                            else if(symbolType == comma)
                                dealOverallVar(recordType);//继续处理当前行的变量声明
                            else {
                                //todo 非逗号且非分号的容错处理
                            }
                        }
                        else {
                            //todo 数组缺少右中括号的容错处理
                        }
                    }
                    else {
                        //todo 数组声明的下标不是数组的容错处理
                    }
                }
                else if(symbolType == comma) { //如果是逗号表示一个声明变量语句
                    tti->obj = Var;
                    dealOverallVar(recordType);//继续处理逗号后面的变量声明
                }
                else if(symbolType == semicolon) { //如果是分号表示一个声明变量语句
                    tti->obj = Var;
                }
                //todo 容错处理
            }
            else if(symbolType == Void) {
                TokenTableItem *tti = &tokenTable[tokenTablePointer++];
                tti->type = Void;
                tti->obj = Func;
                getNextSymbolAndType();
                if(symbolType == customObj) {
                    strcpy(tti->name, symbol);
                    getNextSymbolAndType();
                    if(symbolType == lParent) { //如果是左括号则开始处理函数参数表
                        dealFunc(tti);
                    }
                    else {
                        //todo 函数声明缺少左括号的错误处理
                    }
                }
                else {
                    //todo 缺少函数名的容错处理
                }
            }
            else {
                //todo 非声明语句的容错处理
            }
        }
        else
            return;
    }
}

/**
 * 主函数
 * @return 0
 */
int main() {
    init();
//    in = fopen("/Users/billy/Documents/Github/C/Compiler/testCode.txt", "r");
    printf("***********************输出说明***********************\n");
    printf("每次进入一个处理函数就将缓冲区的一整行输出，所以会有重复的情况\n");
    printf("*****************************************************\n");
    in = fopen("/Users/billy/Documents/Github/C/Compiler/14061131_test.txt", "r");
    grammaticalAnalysis();
//    printTokenTable();
//    printFuncParamTable();
//    printf("***********start***********\n");
//    printAllSymbol();
//    printf("************end************\n");
    return 0;
}
