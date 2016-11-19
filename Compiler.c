//
// Created by 王震 on 2016/11/7.
//

#include <printf.h>
#include <string.h>
#include <stdbool.h>
#include "Compiler.h"

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
char a[];


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
    intervalsLength = 20;

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
    keywords[20].reservedWord = div;
    keywords[20].rwToString = "div";
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
    symbolType = NULL;
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
 * @return 如果找到返回类型，否则返回用户自定义类型
 */
char* getTypeToString(char* symbol) {
    if(symbolType != NULL)
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

void printAllSymbol() {
    while(true) {
        char* s = getNextSymbol();
        if (s != NULL) {
            cout(getTypeToString(symbol));
        }
        else
            return;
    }
}

int main() {
    init();
    in = fopen("/Users/billy/Documents/Github/C/Compiler/14061131_test.txt", "r");
    printf("***********start***********\n");
    printAllSymbol();
    printf("************end************\n");
    return 0;
}