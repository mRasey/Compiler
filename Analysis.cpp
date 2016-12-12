//
// Created by 王震 on 2016/11/7.
//


#include "Analysis.h"
#include "Generate.h"

Interval intervals[100];//间隔符表
Keyword keywords[100];//关键字表
TokenTableItem tokenTable[1024];//符号表
int tokenTablePointer = 1;//符号表指针，从1开始，0表示未找到
CodeTableItem codeTable[4096];//代码表
int codeTablePointer;//代码表指针
QuadCodeTableItem quadCodeTable[2048];//四元式表
int quadCodeTablePointer;//四元式表指针
FuncParamTableItem funcParamTable[1024];//函数表
int funcParamTablePointer = 0;//函数表指针
ReservedWord reservedWord;//当前处理的保留字
FILE* in;
char readIn[1024];
int lineNowPos = 0;
int lineNumber = 0;
int lineLength;
char symbol[1024];
ReservedWord symbolType;
char* symbolTypeToString;
int symPos = 0;
int intervalsLength;
int keywordsLength;
QuadCodeTableItem qCodeInstrs[1024];//四元式表
int qCodePointer = 0;//四元式指针
int labelPointer = 0;//标签指针
int tmpVarPointer = 0;//临时变量指针
string currentDealFunc = "";//当前处理函数
string printStrings[1024];
int printStringsPointer = 0;

bool isAString = false;
bool isAChar = false;
bool isACharVar = false;
bool isAlreadyRecordParamAddr = false;
int lineNum = 0;
int maxTmpVarIndex = 0;
bool ifCanGenerated = true;//是否可以生成代码
string preLine  = "";
bool hasMainFunc = false;
int braceCount = 0;
bool inDealFunc = false;
bool finishOverallConst = false;
bool finishOverallVar = false;
bool finishInnerConst = false;
bool finishInnerVar = false;

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
    intervals[21].symbol = '\t';
    intervalsLength = 22;

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

/**
 * 四元式指令类型转字符串
 * @param instr
 * @return
 */
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
    if(instr == qGetReturnInt)
        return "qGetReturnInt";
    if(instr == qGetReturnChar)
        return "qGetReturnChar";
    if(instr == qPutReturnInt)
        return "qPutReturnInt";
    if(instr == qPutReturnChar)
        return "qGetReturnChar";
    if(instr == qGetArrayValue)
        return "qGetArrayValue";
    if(instr == qGetArrayIntValue)
        return "qGetArrayIntValue";
    if(instr == qGetArrayCharValue)
        return "qGetArrayCharValue";
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
    if(instr == qPassIntParam)
        return "qPassIntParam";
    if(instr == qPassCharParam)
        return "qPassCharParam";
    if(instr == qGetIntParam)
        return "qGetIntParam";
    if(instr == qGetCharParam)
        return "qGetCharParam";
    if(instr == qReturn)
        return "qReturn";
    if(instr == qScanf)
        return "qScanf";
    if(instr == qScanfInt)
        return "qScanfInt";
    if(instr == qScanfChar)
        return "qScanfChar";
    if(instr == qPrintf)
        return "qPrintf";
    if(instr == qPrintfString)
        return "qPrintfString";
    if(instr == qPrintfInt)
        return "qPrintfInt";
    if(instr == qPrintfChar)
        return "qPrintfChar";
    if(instr == qSaveAddr)
        return "qSaveAddr";
    if(instr == qLoadAddr)
        return "qLoadAddr";
    return "errType";
}

/**
 * 在函数参数表里寻找指定变量
 * @param index 在符号表中的位置
 * @return 寻找的结果
 */
vector<string> findInFuncTable(int index) {
    vector<string> result;
    TokenTableItem tti = tokenTable[index];

    int startIndex = tti.paramAddr;//在函数参数表的起始地址
    int lastParamIndex = startIndex + tti.paramAccount;//参数的最后一个地址（不含）
    int lastLocalVar = lastParamIndex + tti.localVarAccount;//局部变量的最后一个地址（不含）
    for(int i = startIndex; i < lastParamIndex; i++) {
        if(funcParamTable[i].name == symbol) {
            result.push_back("true");
            result.push_back("param");
            result.push_back(itoa(i));
            return result;
        }
    }
    for(int i = lastParamIndex; i < lastLocalVar; i++) {
        if(funcParamTable[i].name == symbol) {
            result.push_back("true");
            result.push_back("localVar");
            result.push_back(itoa(i));
            return result;
        }
    }
    result.push_back("false");
    return result;
}

/**
 * 打印出符号表
 */
void printTokenTable() {
    for(int i = 0; i < 10; i++) {
        TokenTableItem *tti = &tokenTable[i];
//        if(tti->obj == Func)
            printf("%s %d\n", tti->name.c_str(), tti->paramAccount);
    }
}

/**
 * 打印函数参数表
 */
void printFuncParamTable() {
    for(int i = 0; i < funcParamTablePointer; i++) {
        FuncParamTableItem *fpti = &funcParamTable[i];
        printf("%s\n", fpti->name.c_str());
    }
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
        if(keywords[i].symbol == s)
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
    int i;
    if(s[0] == '0' && strlen(s) > 1)
        return false;
    for(i = 0; i < strlen(s); i++) {
        if(s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

bool isANumber(string s) {
    int i = 0;
    if(s.at(0) == '+' || s.at(0) == '-')
        i = 1;
    for(i = i; i < s.length(); i++) {
        if(s.at(i) < '0' || s.at(i) > '9')
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
           || s[0] == '*'
           || s[0] == '/'
           || s[0] == '_'
           || ('0' <= s[0] && s[0] <= '9')
           || ('A' <= s[0] && s[0] <= 'z');
}

/**
 * 数字转字符串
 * @param i 要转换的数字
 * @return 生成的字符串
 */
string itoa(int i) {
    stringstream ss;
    ss << i;
    return ss.str();
}

/**
 * 字符转字符串
 * @param c 要转换的字符
 * @return 生成的字符串
 */
string ctoa(char c) {
    stringstream ss;
    ss << c;
    return ss.str();
}

/**
 * 生成标签
 * @return 生成的标签
 */
string getNextLabel() {
    return "label_" + itoa(labelPointer++);
}

/**
 * 获取下一个临时变量
 * @return
 */
string getNextTmpVar() {
    string newTmpVar = "#" + itoa(tmpVarPointer++);
    TokenTableItem *tti = &tokenTable[std::atoi(findInAllTable(currentDealFunc).at(2).c_str())];
    if(tti->maxTmpVar < tmpVarPointer)
        tti->maxTmpVar = tmpVarPointer;
    return newTmpVar;
}

/**
 * 生成四元式
 * @param qCodeInstr 四元式指令
 * @param operand1 操作数1
 * @param operand2 操作数2
 * @param result 结果
 */
void emitQCode(QuadCodeInstr qCodeInstr, string operand1, string operand2, string result) {
    QuadCodeTableItem *qcti = &qCodeInstrs[qCodePointer++];
    qcti->quadCodeInstr = qCodeInstr;
    qcti->operand1 = operand1;
    qcti->operand2 = operand2;
    qcti->result = result;
}

/**
 * 输出所有四元式
 */
void printAllQCode() {
    for(int i = 0; i < qCodePointer; i++) {
        std::cout << left << setw(20) << getStrQCode(qCodeInstrs[i].quadCodeInstr);
        std::cout << left << setw(15) << qCodeInstrs[i].operand1;
        std::cout << left << setw(15) << qCodeInstrs[i].operand2;
        std::cout << left << setw(15) << qCodeInstrs[i].result << std::endl;
    }
}

string getFuncLabel(string funcName) {
    return funcName + ":";
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
        preLine = readIn;
        if(!feof(in)) {
            memset(readIn, 0, 1024);
            fgets(readIn, 1024, in);
            if(readIn[0] == '\0')
                return NULL;
        } else {
//            printf("end of the file\n");  //如果下一行为EOF，则返回null
            return NULL;
        }
        lineLength = (int) strlen(readIn);
        lineNowPos = 0;
        lineNumber++;
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
                if(temp != oldTemp) {
                    if(oldTemp == '\'')
                        throw 42; //todo 缺少'
                    if(oldTemp == '\"')
                        throw 41; //todo 缺少"
                }
                if(oldTemp == '\'' && symbol[1] != '\0')
                    throw 40; //todo 不合法的字符声明
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
    string preSymbol = symbol;
    if(getNextSymbol() == NULL) { //获取下一个token
        try {
            if(!hasMainFunc)
                throw 32;
            if(preSymbol != "}")
                throw 30; //todo 文件不以花括号结束
            if (ifCanGenerated) {
                printAllQCode();
                printTokenTable();
                printFuncParamTable();
                generateAll(); //生成代码并输出到文件
            } else {
                cout << "\n生成失败" << endl;
            }
        }
        catch (int e) {
            errorHandler(e);
            cout << "\n生成失败" << endl;
        }
        exit(0);
    }
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
            if(symbolType == lBrace)
                braceCount++;
            else if(symbolType == rBrace)
                braceCount--;
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
void output(char* s) {
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
            output(getTypeToString(symbol));
        }
        else
            return;
    }
}

/**
 * 跳过当前代码行(以;作为行结束标志),返回下一行的第一个symbol
 */
void jumpToNext(ReservedWord tokenType) {
    while(symbolType != tokenType) {
        getNextSymbolAndType();
    }
    getNextSymbolAndType();
}

void jumpToNextInterval() {
    while(!isinterval(symbol[0])) {
        getNextSymbolAndType();
    }
}

/**
 * 从符号表中找到相应的符号的下标
 * @param symbol 要寻找的符号
 * @return 找到的token的下标，如果没有找到返回0
 */
int findTokenInTable(string symbol) {
    for(int i = tokenTablePointer; i > 0; i--) {
        TokenTableItem tti = tokenTable[i];
        if(tti.name == symbol)
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
        if(funcParamTable[i].name == symbol)
            return true;
    }
    return false;
}

vector<string> findInAllTable(string symbol) {
    vector<string> result;
    int index = findTokenInTable(currentDealFunc);//获得当前处理的函数在符号表中的位置
    TokenTableItem tti = tokenTable[index];
    int startIndex = tti.paramAddr;
    int paramAccount = tti.paramAccount;
    int localVarAccount = tti.localVarAccount;
    for(int i = startIndex; i < startIndex + paramAccount; i++) {
        FuncParamTableItem fpti = funcParamTable[i];
        if(fpti.name == symbol) {
            result.push_back("true");
            result.push_back("param");
            result.push_back(itoa(i));
            return result;
        }
    }
    for(int i = startIndex + paramAccount; i < startIndex + paramAccount + localVarAccount; i++) {
        FuncParamTableItem fpti = funcParamTable[i];
        if(fpti.name == symbol) {
            result.push_back("true");
            result.push_back("localVar");
            result.push_back(itoa(i));
            return result;
        }
    }
    for(int i = tokenTablePointer; i > 0; i--) {
        tti = tokenTable[i];
        if(tti.name == symbol) {
            result.push_back("true");
            result.push_back("overall");
            result.push_back(itoa(i));
            return result;
        }
    }
    result.push_back("false");
    result.push_back("unexist");
    result.push_back(itoa(0));
    return  result;
}





/**
 * 处理全局常量声明
 */
void dealOverallConst() {
    try {
        if(finishOverallConst)
            throw 34; //todo 非法的全局常量声明位置
        while (symbolType != semicolon) {
            TokenTableItem *tti = &tokenTable[tokenTablePointer++];
            getNextSymbolAndType();
            if (symbolType == Int) {
                do {
                    try {
                        tti->type = Int;//记录类型
                        tti->obj = Const;//记录种类
                        getNextSymbolAndType();//读取常量名
                        if (symbolType != customObj)
                            throw 6; //todo 使用了保留字的容错
                        if(findTokenInTable(symbol) != 0)
                            throw 1; //todo 重定义的容错处理
                        tti->name = symbol;//记录变量名
                        getNextSymbolAndType();//读取=号
                        if (symbolType != assign)
                            throw 2; //todo 不是赋值号的容错
                        getNextSymbolAndType();//读取常量值
                        if (isNumber(symbol))
                            tti->intValue = atoi(symbol);//记录常量值
                        else if (symbolType == sub) {
                            getNextSymbolAndType();
                            if (isNumber(symbol) && atoi(symbol) != 0)
                                tti->intValue = -1 * atoi(symbol);
                            else
                                throw 3; //todo 赋值不是数字的容错处理
                        } else if (symbolType == Plus) {
                            getNextSymbolAndType();
                            if (isNumber(symbol) && atoi(symbol) != 0)
                                tti->intValue = atoi(symbol);
                            else
                                throw 3; //todo 赋值不是数字的容错处理
                        } else {
                            throw 3; //todo 赋值不是数字的容错处理
                            tti->intValue = 0;//否则报错并赋值为0
                        }
                        emitQCode(qNewIntCons, tti->name, itoa(tti->intValue), "");//生成四元式
                        getNextSymbolAndType();
                        if(symbolType != comma && symbolType != semicolon)
                            throw 4; //todo 缺少分号或逗号
                    }
                    catch (int e) {
                        errorHandler(e);
                        jumpToNext(semicolon);
                    }
                } while (symbolType == comma && (tti = &tokenTable[tokenTablePointer++]));//如果是逗号则继续读取
            } else if (symbolType == Char) {
                do {
                    try {
                        tti->type = Char;//记录类型
                        tti->obj = Const;//记录种类
                        getNextSymbolAndType();//读取常量名
                        if (symbolType != customObj)
                            throw 6; //todo 使用了保留字的容错
                        if(findTokenInTable(symbol) != 0)
                            throw 1; //todo 重定义的容错处理
                        tti->name = symbol;
                        getNextSymbolAndType();//读取=号
                        if (symbolType != assign)
                            throw 2; //todo 缺少赋值号的容错
                        getNextSymbolAndType();//读取常量值
                        if (isChar(symbol) && symbolType == aChar)
                            tti->charValue = symbol[0];//将字符串转为单个字符
                        else {
                            throw 3; //todo 非法赋值的容错
                            tti->charValue = symbol[0];//否则报错并只取第一个字符
                        }
                        emitQCode(qNewCharCons, tti->name, ctoa(tti->charValue), "");
                        getNextSymbolAndType();
                    }
                    catch (int e) {
                        errorHandler(e);
                        jumpToNext(semicolon);
                    }
                } while (symbolType == comma && (tti = &tokenTable[tokenTablePointer++]));//如果是逗号则继续读取
            } else {
                throw 5; //todo 常量声明需要定义一个类型的容错
                break;
            }
        }
        if(symbolType != semicolon)
            throw 4; //todo 语句末尾未读到分号的容错
        getNextSymbolAndType();//获取下一行的token
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理函数内常量声明
 */
void dealInnerConst(TokenTableItem *tti) {
//    printf("this is a inner const declare\t\t\t%s\n", readIn);
//    getNextSymbolAndType();
//    if(!isAlreadyRecordParamAddr) {
//        tti->paramAddr = funcParamTablePointer;//如果还没记录记录相应函数在函数参数表起始地址
//    }
    try {
        while (symbolType != semicolon) {
            FuncParamTableItem *fpti = &funcParamTable[funcParamTablePointer++];
            getNextSymbolAndType();
            if (symbolType == Int) {
                do {
                    try {
                        getNextSymbolAndType();
                        if (!findFuncParamTable(tti, symbol)) { //如果在函数参数表中没有找到同名的变量则将其加入表中
                            fpti->type = Int;//记录类型
                            fpti->obj = Const;//记录种类
                            fpti->name = symbol;//记录变量名
                            getNextSymbolAndType();//读取=号
                            if (symbolType != assign)
                                throw 2;//todo 不是赋值号的容错
                            getNextSymbolAndType();//读取常量值
                            if (isNumber(symbol)) {
                                fpti->intValue = atoi(symbol);//记录常量值
                            } else if (symbolType == sub) {
                                getNextSymbolAndType();
                                if (isNumber(symbol) && atoi(symbol) != 0) {
                                    fpti->intValue = -1 * atoi(symbol);
                                } else {
                                    throw 15;//todo 非法数字的容错处理
                                }
                            } else if (symbolType == Plus) {
                                getNextSymbolAndType();
                                if (isNumber(symbol) && atoi(symbol) != 0) {
                                    fpti->intValue = atoi(symbol);
                                } else {
                                    throw 15;//todo 非法数字的容错处理
                                }
                            } else {
                                throw 15;//todo 非法数字的容错处理
                                fpti->intValue = 0;//否则报错并赋值为0
                            }
                            emitQCode(qNewIntCons, fpti->name, itoa(fpti->intValue), "");//生成四元式
                            getNextSymbolAndType();
                            tti->localVarAccount++;//将局部变量的计数加一
                        } else {
                            funcParamTablePointer--;//指针退回之前的位置
                            throw 1;//todo 重定义常量的容错处理
                            getNextSymbolAndType();//=
                            getNextSymbolAndType();//value
                            getNextSymbolAndType();//, or ;
                        }
                    }
                    catch (int e) {
                        errorHandler(e);
                        jumpToNext(semicolon);
                    }
                } while (symbolType == comma && (fpti = &funcParamTable[funcParamTablePointer++]));//如果是逗号则继续读取
            } else if (symbolType == Char) {
                do {
                    try {
                        getNextSymbolAndType();
                        if (!findFuncParamTable(tti, symbol)) { //如果在函数参数表中没有找到同名的变量则将其加入表中
                            fpti->type = Char;//记录类型
                            fpti->obj = Const;//记录种类
//                    getNextSymbolAndType();//读取常量名
                            fpti->name = symbol;//记录常量名
//                    strcpy(fpti->name, symbol);//记录常量名
                            getNextSymbolAndType();//读取=号
                            if (symbolType != assign)
                                throw 2; //todo 不是赋值号的容错
                            getNextSymbol();//读取常量值
                            if (isChar(symbol) && symbolType == aChar) {
                                fpti->charValue = symbol[0];//将字符串转为单个字符

                            } else {
                                throw 16;//todo 只取第一个合法字符
                                fpti->charValue = symbol[0];//否则报错并只取第一个字符
                            }
                            emitQCode(qNewCharCons, fpti->name, ctoa(fpti->charValue), "");//生成四元式
                            getNextSymbolAndType();
                            tti->localVarAccount++;//将局部变量的计数加一
                        } else {
                            funcParamTablePointer--;//指针退回之前的位置
                            throw 1;//todo 常量重定义的容错处理
                            getNextSymbolAndType();//=
                            getNextSymbolAndType();//value
                            getNextSymbolAndType();//, or ;
                        }
                    }
                    catch (int e) {
                        errorHandler(e);
                        jumpToNext(semicolon);
                    }
                } while (symbolType == comma && (fpti = &funcParamTable[funcParamTablePointer++]));//如果是逗号则继续读取
            } else {
                printf("err type in inner\n");
                throw 17;//todo 容错
                break;
            }

            if(symbolType != semicolon) {
                throw 18;
            }
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
    getNextSymbolAndType();//获取下一行的token
}

/**
 * 处理全局变量声明
 */
void dealOverallVar(ReservedWord recordType) {
    try {
        finishOverallConst = true;
        if(finishOverallVar)
            throw 35; //todo 非法的全局变量的声明位置
        while (symbolType != semicolon) {
            TokenTableItem *tti = &tokenTable[tokenTablePointer++];//获取下一个表项
            tti->type = recordType;
            tti->obj = Var;//记录种类
            getNextSymbolAndType();
            if (symbolType == customObj) {
                if(findTokenInTable(symbol) != 0)
                    throw 1; //todo 重定义的容错处理
                tti->name = symbol;
                getNextSymbolAndType();
                if (symbolType == comma) { //如果是逗号表示当前一个变量声明完毕，继续当前行的分析
                    if (recordType == Int)
                        emitQCode(qNewIntVar, tti->name, "", "");//生成代码
                    else
                        emitQCode(qNewCharVar, tti->name, "", "");//生成代码
                    continue;
                } else if (symbolType == semicolon) { //如果是分号表示当前行分析完毕，跳出循环
                    if (recordType == Int)
                        emitQCode(qNewIntVar, tti->name, "", "");//生成代码
                    else
                        emitQCode(qNewCharVar, tti->name, "", "");//生成代码
                    break;
                } else if (symbolType == lBracket) { //如果是左中括号表示是一个数组
                    tti->isArray = true;
                    getNextSymbolAndType();
                    if (isNumber(symbol)) { //如果数组下标是合法数字
                        tti->arraySize = atoi(symbol); //记录数组的大小
                        if (recordType == Int)
                            emitQCode(qNewIntArray, tti->name, itoa(tti->arraySize), "");//生成代码
                        else
                            emitQCode(qNewCharArray, tti->name, itoa(tti->arraySize), "");//生成代码
                        getNextSymbolAndType();
                        if (symbolType == rBracket) {
                            getNextSymbolAndType();
                            if (symbolType == comma) //如果是逗号则继续分析
                                continue;
                            else if (symbolType == semicolon) //如果是分号则结束当前行的分析
                                break;
                            else {
                                throw 4;//todo 未出现合法分隔符的容错处理
                            }
                        } else {
                            throw 13;//todo 数组缺少右中括号的容错处理
                        }
                    } else {
                        throw 10;//todo 数组下标非数字的容错处理
                    }
                } else {
                    throw 14;//todo 出现非分隔符的容错处理
                }
            } else {
                throw 6;//todo 出现保留字的容错处理
            }
        }
        getNextSymbolAndType();//获取下一行的token
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理函数内变量声明
 */
void dealInnerVar(TokenTableItem *tti, ReservedWord recordType) {
    try {
//    if(!isAlreadyRecordParamAddr) { //如果还没记录地址
//        tti->paramAddr = funcParamTablePointer;
//    }
        while (symbolType != semicolon) {
            FuncParamTableItem *fpti = &funcParamTable[funcParamTablePointer++];//获取下一个表项
            fpti->type = recordType;//类型记录为传进来的类型
            fpti->obj = Var;//记录种类
            getNextSymbolAndType();//获得标识符
            if (symbolType == customObj) {
                if (!findFuncParamTable(tti, symbol)) {
                    tti->localVarAccount++;//将记录的函数局部变量值加一
                    fpti->name = symbol;//记录变量名
                    getNextSymbolAndType();
                    if (symbolType == comma) { //如果是逗号表示当前一个变量声明完毕，继续当前行的分析
                        if (recordType == Int)
                            emitQCode(qNewIntVar, fpti->name, "", "");
                        else
                            emitQCode(qNewCharVar, fpti->name, "", "");
                        continue;
                    } else if (symbolType == semicolon) { //如果是分号表示当前行分析完毕，跳出循环
                        if (recordType == Int)
                            emitQCode(qNewIntVar, fpti->name, "", "");
                        else
                            emitQCode(qNewCharVar, fpti->name, "", "");
                        break;
                    } else if (symbolType == lBracket) { //如果是左中括号表示是一个数组
                        fpti->isArray = true;
                        getNextSymbolAndType();//获取下标
                        if (isNumber(symbol)) { //如果数组下标是合法数字
                            fpti->arraySize = atoi(symbol); //记录数组的大小
                            getNextSymbolAndType();//]
                            if (recordType == Int)
                                emitQCode(qNewIntArray, fpti->name, itoa(fpti->arraySize), "");
                            else if (recordType == Char)
                                emitQCode(qNewCharArray, fpti->name, itoa(fpti->arraySize), "");
                            if (symbolType == rBracket) {
                                getNextSymbolAndType();
                                if (symbolType == comma) //如果是逗号则继续分析
                                    continue;
                                else if (symbolType == semicolon) //如果是分号则结束当前行的分析
                                    break;
                                else {
                                    throw 4; //todo 未出现合法分隔符的容错处理
                                }
                            } else {
                                throw 13; //todo 数组缺少右中括号的容错处理
                            }
                        } else {
                            throw 10; //todo 数组下标非数字的容错处理
                        }
                    } else {
                        throw 4; //todo 出现非分隔符的容错处理
                    }
                } else {
                    funcParamTablePointer--;//退回之前的位置
                    throw 1; //todo 变量重定义的容错处理
                    getNextSymbolAndType();//,
                    continue;
                }
            } else {
                throw 6; //todo 出现保留字的容错处理
            }
        }
        getNextSymbolAndType();//获取下一行的token
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理赋值语句
 * @return
 */
void dealAssign() {
    try {
        getNextSymbolAndType();//获得变量名
        vector<string> findResult = findInAllTable(symbol);
        string recordSymbol = symbol;
        if (findResult.at(0) == "true") {
            int index = std::atoi(findResult.at(2).c_str());
            getNextSymbolAndType();//获取等号
            if (findResult.at(1) == "overall") { //是全局变量
                TokenTableItem tti = tokenTable[index];
                if (tti.type == Int && tti.obj != Const) {
                    emitQCode(qAssignInt, dealExpression(), recordSymbol, "");
                } else if (tti.type == Char && tti.obj != Const) {
                    emitQCode(qAssignChar, dealExpression(), recordSymbol, "");
                } else {
                    throw 33; //todo 非法赋值类型的容错处理
                }
            } else {
                FuncParamTableItem fpti = funcParamTable[index];
                if (fpti.type == Int && fpti.obj != Const) {
                    emitQCode(qAssignInt, dealExpression(), recordSymbol, "");
                } else if (fpti.type == Char && fpti.obj != Const) {
                    emitQCode(qAssignChar, dealExpression(), recordSymbol, "");
                } else {
                    throw 33; //todo 非法类型的容错处理
                }
            }
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理函数调用的传参
 * @param index 函数在符号表的位置
 */
void dealPassParam(int index) {
    try {
        int pIndex = 0;
        TokenTableItem tti = tokenTable[index];
        int startIndex = tti.paramAddr;//在函数表的起始地址
        int paramAccount = tti.paramAccount;//参数个数

        getNextSymbolAndType();//(
        if (symbolType == lParent) {
//        getNextSymbolAndType();//获取下一个参数
            if (paramAccount == 0) { //无参函数直接返回
                getNextSymbolAndType();//)
                getNextSymbolAndType();//获取下一行的token
                return;
            }



//        while(symbolType != rParent) {
//            FuncParamTableItem *fpti = &funcParamTable[startIndex++];
//            fpti->paramTmpName = dealExpression();
//            emitQCode(qPassIntParam, dealExpression(), "P" + itoa(pIndex++), "");
//            getNextSymbolAndType();//获取下一个参数 , or )

//            if(symbolType == rParent)
//                break;
//            FuncParamTableItem fpti = funcParamTable[startIndex + pIndex];
//            if(fpti.type == Int) {
//                emitQCode(qPassIntParam, symbol, "P" + itoa(pIndex++), "");
//            }
//            else if(fpti.type == Char) {
//                emitQCode(qPassCharParam, symbol, "P" + itoa(pIndex++), "");
//            }
//            else {
//                todo 错误的参数类型的容错处理
//            }
//            getNextSymbolAndType();//获取间隔符
//        }
            do {
                FuncParamTableItem *fpti = &funcParamTable[startIndex++];
                fpti->paramTmpName = dealExpression();
                pIndex++;
            } while (symbolType == comma);
            if(pIndex != paramAccount)
                throw 29; //todo 参数个数不正确
            if (symbolType != rParent)
                throw 23; //todo 缺少右括号
            getNextSymbolAndType();//获取下一行的token
        } else {
            throw 25; //todo 函数调用缺少左括号的容错处理
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理返回int的函数调用
 */
string dealCallFunc(int index) {
    TokenTableItem tti = tokenTable[index];
    dealPassParam(index);//处理传参（已获得右括号后一个token）
    for(int i = 0; i < tti.paramAccount; i++) {
        FuncParamTableItem fpti = funcParamTable[tti.paramAddr + i];
        emitQCode(qPassIntParam, fpti.paramTmpName, "P" + itoa(i), "");
    }
    emitQCode(qJFunc, tti.name, "", "");
    string newTmpVar = getNextTmpVar();
//    emitQCode(qJ, tti.name, "", "");
    if(tti.returnType == Int) {
//        emitQCode(qNewIntVar, newTmpVar, "", "");
        emitQCode(qGetReturnInt, "returnValue", newTmpVar, "");
    }
    else if(tti.returnType == Char) {
//        emitQCode(qNewIntVar, newTmpVar, "", "");
        emitQCode(qGetReturnChar, "returnValue", newTmpVar, "");
    }
    else {
        //todo 使用无返回值函数的容错处理
        return "void func";
    }
    return newTmpVar;
}

/**
 * 处理因子
 * @return 返回因子的值
 */
string dealFactor() {
    try {
        isAChar = false;
//    printf("this is a factor\t\t\t%s\n", readIn);
        //已经获得运算符的后一个token
        if (symbolType == aChar) { //如果是一个字符则直接返回字符的ascii码
            isAChar = true;
            string recordSymbol = symbol;
            getNextSymbolAndType();
            return itoa(recordSymbol[0] - '\0');
        } else if (symbolType == aString) {
            isAString = true;
            string result = symbol;
            getNextSymbolAndType();
            return result;
        }
        vector<string> findResult = findInAllTable(symbol);
        if (findResult.at(0) == "true") {
            int index = std::atoi(findResult.at(2).c_str());
            if (findResult.at(1) == "overall") { //是全局变量
                TokenTableItem tti = tokenTable[index];
                if (tti.isArray) { //如果是数组型
                    getNextSymbolAndType();//[
                    if (symbolType != lBracket)
                        throw 9; //todo 缺少左中括号
                    string newTmpVar = getNextTmpVar();
                    //计算数组下标相应的值
                    if (tti.type == Int) {
                        emitQCode(qGetArrayIntValue, tti.name, dealExpression(), newTmpVar);
                    } else {
                        emitQCode(qGetArrayCharValue, tti.name, dealExpression(), newTmpVar);
                    }
                    getNextSymbolAndType();//获取下一行token
                    return newTmpVar;
                } else if (tti.obj == Func) { //如果是一个函数
//                string newTmpVar = getNextTmpVar();
                    if (tti.returnType == Int) { //return int
                        return dealCallFunc(index);
                    } else if (tti.returnType == Char) { //return char
                        return dealCallFunc(index);
                    } else { //return void
                        //todo 使用了无返回值函数的容错处理
                    }
                } else { //非数组及函数的标识符
                    if (tti.type == Int) { //如果是int型变量返回int值
                        string recordSymbol = symbol;
                        getNextSymbolAndType();
                        return recordSymbol;
                    } else if (tti.type == Char) { //如果是char型返回char值
                        string recordSymbol = symbol;
                        getNextSymbolAndType();
                        return recordSymbol;
                    } else {
                        printf("unexpected error in dealFactor\n");
                    }
                }
            } else { //如果是局部函数变量
                FuncParamTableItem fpti = funcParamTable[index];
                if (fpti.isArray) { //如果是数组型
                    getNextSymbolAndType();//[
                    if (symbolType != lBracket)
                        throw 9; //todo 缺少左中括号
                    string newTmpVar = getNextTmpVar();
                    //计算数组下标相应的值
                    if (fpti.type == Int)
                        emitQCode(qGetArrayIntValue, fpti.name, dealExpression(), newTmpVar);
                    else
                        emitQCode(qGetArrayCharValue, fpti.name, dealExpression(), newTmpVar);
                    getNextSymbolAndType();//获取下一行token
                    return newTmpVar;
                } else { //非数组及函数的标识符
                    if (fpti.type == Int) { //如果是int型变量返回int值
                        string recordSymbol = symbol;
                        getNextSymbolAndType();
                        return recordSymbol;
                    } else if (fpti.type == Char) { //如果是char型返回char值
                        isACharVar = true;
                        string recordSymbol = symbol;
                        getNextSymbolAndType();
                        return recordSymbol;
                    } else {
                        printf("unexpected error in dealFactor\n");
                    }
                }
            }
        } else if (symbolType == Plus || symbolType == sub) {
            if (symbolType == sub) {
                getNextSymbolAndType();
                if (isANumber(symbol)) {
                    stringstream ss;
                    ss << symbol;
                    string result = "-" + ss.str();
                    getNextSymbolAndType();//获取下一行的token
                    return result;
                } else {
                    throw 27; //todo 非法的连续加减号的容错处理
                }
            } else if (symbolType == Plus) {
                getNextSymbolAndType();
                if (isANumber(symbol)) {
                    string recordSymbol = symbol;
                    getNextSymbolAndType();
                    return recordSymbol;
                } else {
                    throw 27; //todo 非法的连续加减号的容错处理
                }
            } else {
                throw 27; //todo 非法的连续加减号的容错处理
            }
        } else if (isNumber(symbol)) { //如果是一个数字
            string recordSymbol = symbol;
            getNextSymbolAndType();//获取下一行的token
            return recordSymbol;
        } else if (isChar(symbol)) { //如果是一个字符
            throw 26; //todo 无意义的字母
//            isAChar = true;
            string recordSymbol = symbol;
            getNextSymbolAndType();
            return recordSymbol;
        } else if (symbolType == lParent) { //如果是一个左括号
            string result = dealExpression();
            getNextSymbolAndType();
            return result;
        } else {
            throw 28; //todo 不合法的因子的容错处理
            return "error";
        }
        return "error";
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理项
 */
string dealTerm() {
    //已经获得运算符的后一个字符
    string factor1 = dealFactor();//处理第一个因子
    while(true) {
//        getNextSymbolAndType();// + or - or ; or ] or )
        if(symbolType == mul || symbolType == Div) {
            string newTmp;
            if(symbolType == mul) {
                newTmp = getNextTmpVar();
                getNextSymbolAndType();
                emitQCode(qNewIntVar, newTmp, "", "");
                emitQCode(qMul, factor1, dealFactor(), newTmp);
                factor1 = newTmp;
            }
            else {
                newTmp = getNextTmpVar();
                getNextSymbolAndType();
                emitQCode(qNewIntVar, newTmp, "", "");
                emitQCode(qDiv, factor1, dealFactor(), newTmp);
                factor1 = newTmp;
            }
        }
        else {
            break;
        }
    }
    return factor1;
}

/**
 * 处理表达式
 */
string dealExpression() {
    try {
        getNextSymbolAndType();//间隔符的后一个token
        if (symbolType == rBrace) {
            throw 22;//todo 表达式为空的容错
        }
        string newTmpVar;
        string term1;
        if (symbolType == Plus || symbolType == sub) {
            if (symbolType == sub) { //取反
                getNextSymbolAndType();
                newTmpVar = getNextTmpVar();
//            emitQCode(qNewIntVar, newTmpVar, "", "");
                emitQCode(qReverse, dealTerm(), newTmpVar, "");
                term1 = newTmpVar;
            } else {
                getNextSymbolAndType();//跳过加号
                term1 = dealTerm();
            }
        } else {
            term1 = dealTerm();
        }
        while (true) {
//        getNextSymbolAndType();
            if (symbolType == Plus || symbolType == sub) {
                if (symbolType == Plus) {
                    getNextSymbolAndType();
                    newTmpVar = getNextTmpVar();
//                emitQCode(qNewIntVar, newTmpVar, "", "");
                    emitQCode(qPlus, term1, dealTerm(), newTmpVar);
                    term1 = newTmpVar;
                } else {
                    getNextSymbolAndType();
                    newTmpVar = getNextTmpVar();
//                emitQCode(qNewIntVar, newTmpVar, "", "");
                    emitQCode(qSub, term1, dealTerm(), newTmpVar);
                    term1 = newTmpVar;
                }
            } else if (symbolType == rParent
                       || symbolType == semicolon
                       || symbolType == comma
                       || symbolType == rBracket) {//读到分号或者右括号表示表达式结束
//            getNextSymbolAndType();//获取下一个token
                break;
            } else if (symbolType == big
                       || symbolType == small
                       || symbolType == bigAndEql
                       || symbolType == smallAndEql
                       || symbolType == eql
                       || symbolType == notEql) {
                break;
            } else {
                throw 11;
                printf("unexpected error in dealExpression\n");
                printAllQCode();
                cout << "****token*****" << endl;
                printTokenTable();
                cout << "****func******" << endl;
                printFuncParamTable();
                exit(0);
            }
        }
        return term1;
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
        return "";
    }
}

/**
 * 处理条件语句，读到)或;为止
 * @return 返回条件语句的结果，如果为假返回0，否则返回1
 */
void dealCondition(string label1, string op) {
    try {
        string op1 = dealExpression();//处理第一个表达式并获取结果(已获得后一个token)
        if (symbolType == big || symbolType == small || symbolType == bigAndEql
            || symbolType == smallAndEql || symbolType == eql || symbolType == notEql) {
            if (op == "if") {
                if (symbolType == big)
                    emitQCode(qJle, op1, dealExpression(), label1);
                else if (symbolType == small)
                    emitQCode(qJge, op1, dealExpression(), label1);
                else if (symbolType == bigAndEql)
                    emitQCode(qJl, op1, dealExpression(), label1);
                else if (symbolType == smallAndEql)
                    emitQCode(qJg, op1, dealExpression(), label1);
                else if (symbolType == eql)
                    emitQCode(qJne, op1, dealExpression(), label1);
                else if (symbolType == notEql)
                    emitQCode(qJe, op1, dealExpression(), label1);
                else {
                }
            } else if (op == "for") {
                if (symbolType == big)
                    emitQCode(qJle, op1, dealExpression(), label1);
                else if (symbolType == small)
                    emitQCode(qJge, op1, dealExpression(), label1);
                else if (symbolType == bigAndEql)
                    emitQCode(qJl, op1, dealExpression(), label1);
                else if (symbolType == smallAndEql)
                    emitQCode(qJg, op1, dealExpression(), label1);
                else if (symbolType == eql)
                    emitQCode(qJne, op1, dealExpression(), label1);
                else if (symbolType == notEql)
                    emitQCode(qJe, op1, dealExpression(), label1);
                else {
                    cout << "unexpect error in dealCondition" << endl;
                }
            } else if (op == "doWhile") {
                if (symbolType == big)
                    emitQCode(qJg, op1, dealExpression(), label1);
                else if (symbolType == small)
                    emitQCode(qJl, op1, dealExpression(), label1);
                else if (symbolType == bigAndEql)
                    emitQCode(qJge, op1, dealExpression(), label1);
                else if (symbolType == smallAndEql)
                    emitQCode(qJle, op1, dealExpression(), label1);
                else if (symbolType == eql)
                    emitQCode(qJe, op1, dealExpression(), label1);
                else if (symbolType == notEql)
                    emitQCode(qJne, op1, dealExpression(), label1);
                else {
                    cout << "unexpect error in dealCondition" << endl;
                }
            }
        } else if (symbolType == rParent) { //如果读取到右括号表明条件只有一个表达式
            if (op == "if") {
                emitQCode(qJe, op1, "0", label1);
            } else if (op == "for") {
                emitQCode(qJe, op1, "0", label1);
            } else if (op == "doWhile") {
                emitQCode(qJne, op1, "0", label1);
            } else {
                cout << "unexpect error in dealCondition" << endl; //todo 非法的比较操作的容错处理
            }
        } else {
            throw 23;//todo 非法比较符的容错处理
        }
        getNextSymbolAndType();//获取下一行的token
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理for循环步长语句
 */
void dealForStep() {
    try {
//    getNextSymbolAndType();//结果标识符
        vector<string> findResult = findInAllTable(symbol);
        string recordSymbol = symbol;
        if (findResult.at(0) == "true") {
            int index = std::atoi(findResult.at(2).c_str());
            getNextSymbolAndType();//获取等号=
            if (findResult.at(1) == "overall") { //如果是全局变量
                TokenTableItem tti = tokenTable[index];
                if (symbolType == assign) {
                    if (tti.type == Int && tti.obj != Const) {
                        emitQCode(qAssignInt, dealExpression(), recordSymbol, "");
                    } else if (tti.type == Char && tti.obj != Const) {
                        emitQCode(qAssignChar, dealExpression(), recordSymbol, "");
                    } else {
                        throw 33;//todo 非法的类型
                    }
                } else {
                    throw 2;//todo 缺少赋值号的容错处理
                }

            } else { //局部变量
                FuncParamTableItem fpti = funcParamTable[index];
                if (symbolType == assign) {
                    if (fpti.type == Int && fpti.obj != Const) {
                        emitQCode(qAssignInt, dealExpression(), recordSymbol, "");
                    } else if (fpti.type == Char && fpti.obj != Const) {
                        emitQCode(qAssignChar, dealExpression(), recordSymbol, "");
                    } else {
                        throw 33; //todo 非法的类型
                    }
                } else {
                    throw 2; //todo 缺少赋值号的容错处理
                }
            }
        } else {
            throw 26;//todo 为找到合法变量定义的容错处理
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理for循环
 */
void dealFor() {
    try {
        string label1 = getNextLabel();
        string label2 = getNextLabel();
        string label3 = getNextLabel();
        string label4 = getNextLabel();
        getNextSymbolAndType();//(
        if (symbolType == lParent) {
            dealAssign();//处理赋值语句
            emitQCode(qLabel, label1 + ":", "", "");
            dealCondition(label4, "for");//处理条件语句
            emitQCode(qJ, label3, "", "");
            emitQCode(qLabel, label2 + ":", "", "");
            dealForStep();//处理for循环跳转步数语句，当前token为）
            emitQCode(qJ, label1, "", "");
            emitQCode(qLabel, label3 + ":", "", "");
        } else {
            throw 12; //todo for循环头部容错处理
        }
        getNextSymbolAndType(); //{ or ; or XXXXXX;
        dealStatement();
        emitQCode(qJ, label2, "", "");
        emitQCode(qLabel, label4 + ":", "", "");
//    if(symbolType == lBrace) {
//        dealStatements();
//        emitQCode(qJ, label1, "", "");
//        emitQCode(qLabel, label2 + ":", "", "");
//    }
//    else if(symbolType == semicolon){ //如果是分号表示for循环有一个空循环体，则结束当前行处理
//        emitQCode(qJ, label1, "", "");
//        emitQCode(qLabel, label2 + ":", "", "");
//        return;
//    }
//    else {
//        //todo for循环头部后接非法符号的容错处理
//    }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理do...while...循环
 */
void dealDoWhile() {
    try {
        string label1 = getNextLabel();//获得标签1
        emitQCode(qLabel, label1 + ":", "", "");//生成标签1
        getNextSymbolAndType();//{
        dealStatement();
//    if(symbolType == lBrace) { //如果是左花括号，进入语句列处理
//        dealStatements();
//    }
//    else if(symbolType == While) {
//        //todo 缺少循环实体的容错处理
//    }
//    else { //否则进入语句处理
//        dealStatement();
//    }
//    getNextSymbolAndType();
        if (symbolType == While) {
            getNextSymbolAndType();//(
            if (symbolType == lParent) {
                dealCondition(label1, "doWhile");
//            getNextSymbolAndType();//获取下一行的起始token
            } else {
                throw 25; //todo while后缺少左括号的容错处理
            }
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理if条件语句
 */
void dealIf() {
    try {
        string label1 = getNextLabel();
        string label2 = getNextLabel();
        getNextSymbolAndType();//(
        if (symbolType == lParent) { //如果是左括号进入条件处理语句
            dealCondition(label1, "if");//在处理条件语句的地方生成标签
            dealStatement();
            if (symbolType == Else) { //如果有else块则开始处理else的语句
                emitQCode(qJ, label2, "", "");//生成标签2
                emitQCode(qLabel, label1 + ":", "", "");//生成标签1
                getNextSymbolAndType();
                dealStatement();
                emitQCode(qLabel, label2 + ":", "", "");//生成标签2
            } else { //如果没有else则只生成标签1
                //todo 读到下一句的句首
                emitQCode(qLabel, label1 + ":", "", "");//生成标签1
            }
        } else {
            throw 12; //todo if后缺少左括号的容错处理
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理语句
 */
void dealStatement() {
    tmpVarPointer = 0;//每次进入新的一行就将临时变量号至零
    //已经获取首个token，不需要重新获取
    try {
        vector <string> findResult = findInAllTable(symbol);
        string recordSymbol = symbol;
        if (findResult.at(0) == "true") {
            int index = std::atoi(findResult.at(2).c_str());
            if (findResult.at(1) == "overall") { //全局变量
                TokenTableItem tti = tokenTable[index];
                if (tti.obj == Func) {
                    dealCallFunc(index);
//                    getNextSymbolAndType();//调用函数语句结束之后获取下一行的token
                } else if (tti.isArray) {
                    //todo 数组的赋值语句
                    getNextSymbolAndType();//[
                    if (symbolType == lBracket) {
                        string arrayIndex = dealExpression();
                        getNextSymbolAndType();//=
                        if (symbolType == assign) {
                            if (tti.type == Int)
                                emitQCode(qAssignIntArray, dealExpression(), arrayIndex, tti.name);
                            else
                                emitQCode(qAssignCharArray, dealExpression(), arrayIndex, tti.name);
                        } else {
                            throw 11; //todo 错误的语句类型的容错处理
                        }
                    } else {
                        throw 9; //todo 数组缺少左括号的容错处理
                    }
//                    getNextSymbolAndType();//如果是赋值语句则在处理赋值语句结束之后获取下一行的token
                } else {
                    getNextSymbolAndType();
                    if (symbolType == assign) {
                        if (tti.type == Int  && tti.obj != Const)
                            emitQCode(qAssignInt, dealExpression(), recordSymbol, "");
                        else if(tti.type == Char  && tti.obj != Const)
                            emitQCode(qAssignChar, dealExpression(), recordSymbol, "");
                        else
                            throw 33; //todo 给常量赋值的容错
                    } else {
                        throw 11; //todo 错误的语句类型的容错处理
                    }
//                    getNextSymbolAndType();//如果是赋值语句则在处理赋值语句结束之后获取下一行的token
                }
//                getNextSymbolAndType();//调用函数语句结束之后获取下一行的token
            } else { //局部变量
                FuncParamTableItem fpti = funcParamTable[index];
                if (fpti.obj == Func) {
                    cout << "unexpect error in local func"; //todo 出现局部函数变量的容错处理
                } else if (fpti.isArray) {
                    //todo 数组的赋值语句
                    getNextSymbolAndType();//[
                    if (symbolType == lBracket) {
                        string arrayIndex = dealExpression();
                        getNextSymbolAndType();//=
                        if (symbolType == assign) {
                            if (fpti.type == Int)
                                emitQCode(qAssignIntArray, dealExpression(), arrayIndex, fpti.name);
                            else
                                emitQCode(qAssignCharArray, dealExpression(), arrayIndex, fpti.name);
                        } else {
                            throw 11; //todo 非赋值语句类型的容错处理
                        }
                    } else {
                        throw 9; //todo 数组缺少左括号的容错处理
                    }
//                    getNextSymbolAndType();//如果是赋值语句则在处理赋值语句结束之后获取下一行的token
                } else {
                    getNextSymbolAndType();
                    if (symbolType == assign) {
                        if (fpti.type == Int  && fpti.obj != Const)
                            emitQCode(qAssignInt, dealExpression(), recordSymbol, "");
                        else if(fpti.type == Char && fpti.obj != Const)
                            emitQCode(qAssignChar, dealExpression(), recordSymbol, "");
                        else
                            throw 33; //todo 给常量赋值的容错
                    } else {
                        throw 11; //todo 错误的语句类型的容错处理
                    }
//                    getNextSymbolAndType();//如果是赋值语句则在处理赋值语句结束之后获取下一行的token
                }
//                getNextSymbolAndType();//如果是赋值语句则在处理赋值语句结束之后获取下一行的token
            }
//            cout << "23333: " << symbol << endl;
            if(symbolType != semicolon)
                return; //todo 语句结尾缺少分号,在dealExpression中报错，所以在这里直接return
            getNextSymbolAndType();//如果是赋值语句则在处理赋值语句结束之后获取下一行的token
        } else { //使用了其他的关键字
            if (symbolType == Printf) {
                getNextSymbolAndType();//(
                if (symbolType == lParent) {
                    int i = 0;
                    bool hasStr = false;
                    bool hasVar = false;
                    do {
                        string dealResult = dealExpression();
                        if (isAString) {
                            if(hasStr || hasVar)
                                throw 39; //todo print参数错误
                            printStrings[printStringsPointer] = dealResult;//记录所有string
                            emitQCode(qPrintfString, "str_" + itoa(printStringsPointer++), "", "");
                            isAString = false;
                            hasStr = true;
                        }
                        else {
                            if(hasVar)
                                throw 39; //todo print参数错误
                            emitQCode(qPrintfInt, dealResult, "", "");
                            hasVar = true;
                        }
                        i++;
                        if(i > 2)
                            throw 38;
                    } while (symbolType == comma);
                    if(symbolType != rParent)
                        throw 23; //todo prinf以非右括号结束
                    getNextSymbolAndType();//获取;
                    if(symbolType != semicolon)
                        throw 24; //todo 语句缺少分号
                    getNextSymbolAndType();//获取下一行的起始token
                } else {
                    throw 25; //todo 输出语句缺少括号的容错处理
                }
            } else if (symbolType == Scanf) {
                //todo 输入语句
                getNextSymbolAndType();//(
                if(symbolType != lParent)
                    throw 25;
                do {
                    getNextSymbolAndType();//标识符
                    if(symbolType != customObj)
                        throw 6; //todo 使用了保留字
                    vector <string> result = findInAllTable(symbol);
                    int index = std::atoi(result.at(2).c_str());
                    if (result.at(0) == "true") {
                        if (result.at(1) == "overall") { //是全局变量
                            TokenTableItem tti = tokenTable[index];
                            if (tti.type == Int) {
                                emitQCode(qScanfInt, symbol, "", "");
                            } else {
                                emitQCode(qScanfChar, symbol, "", "");
                            }
                        } else {
                            FuncParamTableItem fpti = funcParamTable[index];
                            if (fpti.type == Int) {
                                emitQCode(qScanfInt, symbol, "", "");
                            } else {
                                emitQCode(qScanfChar, symbol, "", "");
                            }
                        }
                        getNextSymbolAndType();//, or )
                    } else {
                        throw 26; //todo 输入到未定义标识符的容错处理
                    }
                } while (symbolType == comma);
                if(symbolType != rParent)
                    throw 23; //todo 输出语句缺少右括号
                getNextSymbolAndType();//;
                if(symbolType != semicolon)
                    throw 24; //todo 缺少分号
                getNextSymbolAndType();//获取下一行的起始token
            } else if (symbolType == Return) {
                getNextSymbolAndType();//( or ;
                TokenTableItem tti = tokenTable[findTokenInTable(currentDealFunc)];
                if (symbolType == lParent && tti.returnType != Void) {
//                getNextSymbolAndType();//获取标识符
                    if (symbolType == rParent) { //如果是空表示无返回值
                        throw 20; //todo 对有返回值函数使用无返回值的return的容错处理
                    } else { //将返回值存储
                        if (tti.returnType == Char) {
                            emitQCode(qPutReturnChar, dealExpression(), "returnValue", "");
                            emitQCode(qJ, "$end_" + currentDealFunc, "", "");//跳转到函数结尾
                            getNextSymbolAndType();//;
                            getNextSymbolAndType();//获取下一行的起始token
                        } else if (tti.returnType == Int) {
                            emitQCode(qPutReturnInt, dealExpression(), "returnValue", "");
                            emitQCode(qJ, "$end_" + currentDealFunc, "", "");//跳转到函数结尾
                            getNextSymbolAndType();//;
                            getNextSymbolAndType();//获取下一行的起始token
                        } else {
                            cout << "unexpect error in dealStatement" << endl;
                        }
                    }
                } else if (symbolType == semicolon && tti.returnType == Void) {
                    emitQCode(qJ, "$end_" + currentDealFunc, "", "");//跳转到函数结尾
                    getNextSymbolAndType();//获取下一行的起始token
                } else {
                    throw 21; //todo 不合法的返回语句
                }
            } else if (symbolType == lBrace) {
                dealStatements();
            } else if (symbolType == If) {
                dealIf();
            } else if (symbolType == Do) {
                dealDoWhile();
            } else if (symbolType == For) {
                dealFor();
            } else if (symbolType == semicolon) {
                getNextSymbolAndType();//获取下一行的起始token
                return;
            } else if (symbolType == rBrace) {
                if(braceCount == 0)
                    return;
                throw 19;//todo 不应该出现的右花括号
                return;
            } else {
                throw 11;//todo 无效的类型的容错处理
            }
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }

}

/**
 * 处理语句列
 */
void dealStatements() {
//    printf("this is a list of statement\t\t\t%s\n", readIn);
    getNextSymbolAndType();//获取下一行的token
    while(symbolType != rBrace) {
        dealStatement();
//        getNextSymbolAndType();
//        if (symbolType == If) { //进入if...else...循环
//            dealIf();
////            getNextSymbolAndType();
//        }
//        else if (symbolType == For) { //进入for循环
//            dealFor();
////            getNextSymbolAndType();
//        }
//        else if (symbolType == Do) { //进入do...while...循环
//            dealDoWhile();//处理完毕得到(
////            getNextSymbolAndType();
//        }
//        else if(symbolType == Else) {
//            //todo 没有if匹配的else的容错处理
//        }
//        else if(symbolType == rBrace) { //如果读到}则跳出
//            getNextSymbolAndType();
//            return;
//        }
//        else if(symbolType == lBrace) {
//            getNextSymbolAndType();
//            dealStatements();
//        }
//        else if(symbolType == Return) {
//
//        }
//        else if(symbolType == semicolon) {
//            cout << "error in dealStatement" << endl;
//            getNextSymbolAndType();//获取下一行的第一个token
//        }
//        else {
//            dealStatement();
////            getNextSymbolAndType();
//        }
    }
    getNextSymbolAndType();//获取下一行token
}

/**
 * 处理函数参数
 */
void dealParamlist(TokenTableItem *tti) {
    //已获取左括号
    try {
        tti->paramAddr = funcParamTablePointer;
        int pIndex = 0;
        getNextSymbolAndType();
        if (symbolType == rParent) { //如果是空表示没有参数
            tti->paramAccount = 0;
            return;
        } else if (symbolType == Int || symbolType == Char) {
            tti->paramAddr = funcParamTablePointer;
            while (true) {
                if (symbolType == Int || symbolType == Char) {
                    ReservedWord recordType = symbolType;//记录变量的类型用于生成四元式
                    tti->paramAccount++;
                    FuncParamTableItem *fpt = &funcParamTable[funcParamTablePointer++];
                    fpt->type = symbolType;
                    getNextSymbolAndType();
                    if (symbolType == customObj) {
                        if(findFuncParamTable(tti, symbol))
                            throw 1; //todo 重复定义的变量名
                        fpt->name = symbol;//记录参数名
                        if (recordType == Int) {
                            emitQCode(qGetIntParam, "P" + itoa(pIndex++), fpt->name, "");
                        } else {
                            emitQCode(qGetCharParam, "P" + itoa(pIndex++), fpt->name, "");
                        }
                        getNextSymbolAndType();
                        if (symbolType == comma) {
                            getNextSymbolAndType();
                            continue;
                        } else if (symbolType == rParent) {
                            break;
                        } else {
                            throw 23; //todo 参数表出现非法参数的容错处理
                        }
                    } else {
                        throw 6; //todo 参数表用户自定义类型为保留字的容错处理
                        funcParamTablePointer--;//从参数表中退出未成功识别的参数
                        tti->paramAccount--;
                    }
                }
            }
        } else {
            throw 23; //todo 参数表格式错误的容错处理
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 处理函数
 */
void dealFunc(TokenTableItem *tti) {
    try {
        finishOverallConst = true;
        finishOverallVar = true;
        currentDealFunc = tti->name;
        emitQCode(qFuncLabel, getFuncLabel(tti->name), "", "");//生成函数标签
        dealParamlist(tti);
        getNextSymbolAndType();//{
        if (symbolType == lBrace) {
            getNextSymbolAndType();
            while (symbolType == Const) {
                dealInnerConst(tti);
//            getNextSymbolAndType();
            }
            while (symbolType == Int || symbolType == Char) {
                dealInnerVar(tti, symbolType);
//            getNextSymbolAndType();
            }
            do {
                dealStatement();
            } while (symbolType != rBrace);
//        dealStatements();
        } else {
            throw 30; //todo 函数定义缺少花括号的容错处理
        }
        emitQCode(qFuncEndLabel, getFuncLabel("$end_" + tti->name), "", "");
        getNextSymbolAndType();//获取下一行的token
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}

/**
 * 语法分析程序
 */
void grammaticalAnalysis() {
    try {
        getNextSymbolAndType();
        while (true) {
//        getNextSymbolAndType();
            if (lineNowPos != lineLength || !feof(in)) { //如果没有读到文件末尾就一直往下读
                try {
                    if (symbolType == Const) {
                        if(finishOverallConst)
                            throw 34; //todo 非法全局常量的声明
                        dealOverallConst();
                    } else if (symbolType == Int || symbolType == Char) {
                        ReservedWord recordType = symbolType;
                        TokenTableItem *tti = &tokenTable[tokenTablePointer++];//获取下一个表项
                        tti->type = symbolType;//保存类型
                        getNextSymbolAndType();//获取下一个标识符
                        if (findTokenInTable(symbol) != 0)
                            throw 1; //todo 重定义容错处理
                        tti->name = symbol;
                        if (tti->name == "main")
                            throw 31; //todo main函数的返回值需为void
                        getNextSymbolAndType();
                        if (symbolType == lParent) { //如果是(表示是一个函数
                            tti->obj = Func;
                            tti->returnType = recordType;//记录函数返回类型
                            dealFunc(tti);
                        } else if (symbolType == lBracket) { //如果是[表明是一个声明数组变量语句
                            finishOverallConst = true;
                            if(finishInnerVar)
                                throw 35; //todo 非法的声明全局变量位置
                            tti->isArray = true;
                            getNextSymbolAndType();//获取下一个token
                            if (isNumber(symbol)) {
                                tti->arraySize = atoi(symbol);
                                if (recordType == Int)
                                    emitQCode(qNewIntArray, tti->name, itoa(tti->arraySize), "");
                                else
                                    emitQCode(qNewCharArray, tti->name, itoa(tti->arraySize), "");
                                //todo 数组声明过大的容错处理
                                getNextSymbolAndType();
                                if (symbolType == rBracket) {
                                    getNextSymbolAndType();
                                    if (symbolType == semicolon) {
                                        getNextSymbolAndType();
                                        continue;//继续下一行代码的处理
                                    } else if (symbolType == comma)
                                        dealOverallVar(recordType);//继续处理当前行的变量声明
                                    else {
                                        throw 4;//todo 非逗号且非分号的容错处理
                                    }
                                } else {
                                    throw 9;//todo 数组缺少右中括号的容错处理
                                }
                            } else {
                                throw 10;//todo 数组声明的下标不是数字的容错处理
                            }
                        } else if (symbolType == comma) { //如果是逗号表示一个声明变量语句
                            finishOverallConst = true;
                            if(finishInnerVar)
                                throw 35; //todo 非法的声明全局变量位置
                            tti->obj = Var;
                            if (recordType == Int)
                                emitQCode(qNewIntVar, tti->name, "", "");
                            else
                                emitQCode(qNewCharVar, tti->name, "", "");
                            dealOverallVar(recordType);//继续处理逗号后面的变量声明
                        } else if (symbolType == semicolon) { //如果是分号表示一个声明变量语句
                            finishOverallConst = true;
                            if(finishInnerVar)
                                throw 35; //todo 非法的声明全局变量位置
                            tti->obj = Var;
                            if (recordType == Int)
                                emitQCode(qNewIntVar, tti->name, "", "");
                            else
                                emitQCode(qNewCharVar, tti->name, "", "");
                            getNextSymbolAndType();
                        } else {
                            throw 11;//todo 容错处理
                        }
                    } else if (symbolType == Void) {
                        TokenTableItem *tti = &tokenTable[tokenTablePointer++];
                        tti->returnType = Void;
                        tti->obj = Func;
                        getNextSymbolAndType();
                        if (symbolType == customObj) {
                            if (findTokenInTable(symbol) != 0)
                                throw 1; //todo 重定义变量的容错
                            tti->name = symbol;
                            if (tti->name == "main")
                                hasMainFunc = true; //如果出现了main函数置为true
                            getNextSymbolAndType();
                            if (symbolType == lParent) { //如果是左括号则开始处理函数参数表
                                dealFunc(tti);
                            } else {
                                throw 12;//todo 函数声明缺少左括号的错误处理
                            }
                        } else {
                            throw 8;//todo 缺少函数名的容错处理
                        }
                    } else {
                        cout << "error" << endl;
                        throw 7;//todo 非声明语句的容错处理
                    }
                }
                catch (int e) {
                    errorHandler(e);
                    jumpToNext(semicolon);
                }
            } else {
//                getNextSymbolAndType();
                throw 7;
                return;
            }
        }
    }
    catch (int e) {
        errorHandler(e);
        jumpToNext(semicolon);
    }
}