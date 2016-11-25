//
// Created by 王震 on 2016/11/25.
//

#include "Generate.h"
#include "Analysis.h"

extern Interval intervals[100];//间隔符表
extern Keyword keywords[100];//关键字表

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

extern int labelPointer;//标签指针
extern int tmpVarPointer;//临时变量指针

extern TokenTableItem tokenTable[1024];//符号表
extern int tokenTablePointer;//符号表指针，从1开始，0表示未找到
extern CodeTableItem codeTable[1024];//代码表
extern int codeTablePointer;//代码表指针
extern QuadCodeTableItem quadCodeTable[1024];//四元式表
extern int quadCodeTablePointer;//四元式表指针
extern FuncParamTableItem funcParamTable[1024];//函数表
extern int funcParamTablePointer;//函数表指针
extern QuadCodeTableItem qCodeInstrs[1024];//四元式表
extern int qCodePointer;//四元式指针
TmpVarItem tmpVarTable[1024];//临时变量表
int tmpVarTablePointer = 0;//临时变量表指针
extern string currentDealFunc;//当前处理函数名
extern string printStrings[1024];
extern int printStringsPointer;
int qCodeTableScanner = 0;//四元式表扫描指针



void addNewMipsCode(string opName, string op1, string op2, string op3) {
    CodeTableItem *cti = &codeTable[codeTablePointer++];
    cti->name = opName;
    cti->operand1 = op1;
    cti->operand2 = op2;
    cti->operand3 = op3;
}

/**
 * 输出所有mips目标代码
 */
void printAllMipsCode() {
    for(int i = 0; i < codeTablePointer; i++) {
        CodeTableItem *cti = &codeTable[i];
        std::cout << left << setw(20) << cti->name;
        std::cout << left << setw(15) << cti->operand1;
        std::cout << left << setw(15) << cti->operand2;
        std::cout << left << setw(15) << cti->operand3 << endl;
    }
}

/**
 * 生成所有string
 */
void generateAllStrings() {
    for(int i = 0; i < printStringsPointer; i++) {
        addNewMipsCode("str_" + itoa(i) + ":", ".asciiz", "\"" + printStrings[i] + "\"", "");
    }
}

/**
 * 生成全局常量
 */
void generateOverallConst() {
    while(qCodeTableScanner < qCodePointer) {
        QuadCodeTableItem qcti = qCodeInstrs[qCodeTableScanner++];
        if(qcti.quadCodeInstr == qNewIntCons
                || qcti.quadCodeInstr == qNewCharCons) {
            switch (qcti.quadCodeInstr) {
                case qNewIntVar:
                    addNewMipsCode(qcti.operand1 + ":", ".word", qcti.operand2, "");
                    break;
                case qNewCharVar:
                    addNewMipsCode(qcti.operand1 + ":", ".word", "'"+qcti.operand2+"'", "");
                    break;
                default:
                    cout << "error" << endl;
                    break;
            }
        }
        else {
            qCodeTableScanner--;//退回扫描指针
            break;
        }
    }
}

/**
 * 生成全局变量
 */
void generateOverallVar() {
    while(qCodeTableScanner < qCodePointer) {
        QuadCodeTableItem qcti = qCodeInstrs[qCodeTableScanner++];
        if(qcti.quadCodeInstr == qNewIntVar
                || qcti.quadCodeInstr == qNewCharVar
                || qcti.quadCodeInstr == qNewIntArray
                || qcti.quadCodeInstr == qNewCharArray) {
            switch (qcti.quadCodeInstr) {
                case qNewIntVar:
                    addNewMipsCode(qcti.operand1 + ":", ".word", qcti.operand2, "");
                    break;
                case qNewCharVar:
                    addNewMipsCode(qcti.operand1 + ":", ".word", "'"+qcti.operand2+"'", "");
                    break;
                case qNewIntArray:
                    addNewMipsCode(qcti.operand1 + ":", ".space", itoa(std::atoi(qcti.operand2.c_str()) * 4), "");
                    break;
                case qNewCharArray:
                    addNewMipsCode(qcti.operand1 + ":", ".space", itoa(std::atoi(qcti.operand2.c_str()) * 4), "");
                    break;
                default:
                    cout << "error" << endl;
                    break;
            }
        }
        else {
            qCodeTableScanner--;//退回扫描指针
            break;
        }
    }
}

/**
 * 生成函数
 */
void generateFunc(QuadCodeTableItem qcti) {
    currentDealFunc = qcti.operand1.substr(0, qcti.operand1.length() - 1);//函数名标签
    int index = findTokenInTable(currentDealFunc);//找到函数名在符号表中的位置
    TokenTableItem tti = tokenTable[index];
    int startIndex = tti.paramAddr;
    int paramAccount = tti.paramAccount;//参数个数
    int localVarAccount = tti.localVarAccount;//局部变量个数
    addNewMipsCode(currentDealFunc, "", "", "");//函数名标签指令
    addNewMipsCode("sw", "$fp", "($sp)", "");//保存当前的fp指针
    addNewMipsCode("move", "$fp", "$sp", "");//保存当前sp指针为fp指针
    addNewMipsCode("subu", "$sp", "$sp", "");//申请局部变量的空间
    addNewMipsCode("sw", "$ra", "-4($fp)", "");//保存返回地址
    addNewMipsCode("sw", "$v0", "-8($fp)", "");//保存返回值

    while(true) {
        qcti = qCodeInstrs[qCodeTableScanner++];
        switch (qcti.quadCodeInstr) {
            case qNewIntCons:
                break;
            case qNewCharCons:
                break;
            case qNewIntVar:
                break;
            case qNewCharVar:
                break;
            case qNewIntArray:
                break;
            case qNewCharArray:
                break;
            case qLabel:
                break;
            case qFuncLabel:
                break;
            case qFuncEndLabel:
                addNewMipsCode("lw", "$ra", "-4($fp)", "");//保存返回地址
                addNewMipsCode("move", "$sp", "$fp", "");//退栈
                addNewMipsCode("lw", "$fp", "($fp)", "");//获得之前的fp指针
                addNewMipsCode("jr", "$ra", "", "");//return
                return;
            case qCallFunc:
                break;
            case qPlus:
                break;
            case qSub:
                break;
            case qMul:
                break;
            case qDiv:
                break;
            case qJl:
                break;
            case qJg:
                break;
            case qJne:
                break;
            case qJle:
                break;
            case qJge:
                break;
            case qJe:
                break;
            case qJ:
                break;
            case qGetArrayIntValue:
                break;
            case qGetArrayCharValue:
                break;
            case qAssignInt:
                break;
            case qAssignChar:
                break;
            case qAssignIntArray:
                break;
            case qAssignCharArray:
                break;
            case qReverse:
                break;
            case qPassParam:
                break;
            case qReturn:
                break;
            case qScanfInt:
                addNewMipsCode("li", "$v0", "5", "");
                addNewMipsCode("syscall", "", "", "");
                break;
            case qScanfChar:
                addNewMipsCode("li", "$v0", "5", "");
                addNewMipsCode("syscall", "", "", "");
                break;
            case qPrintfString:
                addNewMipsCode("la", "$a0", qcti.operand1, "");
                addNewMipsCode("li", "$v0", "4", "");
                addNewMipsCode("syscall", "", "", "");
                break;
            case qPrintfInt:
                addNewMipsCode("li", "$a0", qcti.operand1, "");//todo 替换成地址
                addNewMipsCode("li", "$v0", "1", "");
                addNewMipsCode("syscall", "", "", "");
                break;
            case qSaveAddr:
                break;
            case qLoadAddr:
                break;
            default:
                break;
        }
    }

}

/**
 * 生成函数内常量
 */
void generateInnerConst() {

}

/**
 * 生成函数内变量
 */
void generateInnerVar() {

}

/**
 * 生成MIPS代码
 */
void generateAll() {
    addNewMipsCode(".data", "", "", "");//声明data区
    generateAllStrings();//声明所有的字符串
    generateOverallConst();//生成全局常量
    generateOverallVar();//生成全局变量
    addNewMipsCode(".text", "", "", "");//声明text区
    addNewMipsCode("li", "$fp", "0", "");//初始化fp
    addNewMipsCode("li", "$sp", "0", "");//初始化sp
    addNewMipsCode("j", "main", "", "");//跳到main函数

//    QuadCodeTableItem qcti = qCodeInstrs[qCodeTableScanner++];
//    do  {
//        generateFunc(qcti);//生成函数
//        qcti = qCodeInstrs[qCodeTableScanner++];
//    } while(qcti.quadCodeInstr == qFuncLabel);
    printAllMipsCode();//输出所有mips目标代码
}
