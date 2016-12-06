//
// Created by 王震 on 2016/11/25.
//

#include <fstream>
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

/**
 * 字符串转数字
 * @param s
 * @return
 */
int stringToInt(string s) {
    return std::atoi(s.c_str());
}

/**
 * 添加新的mips指令
 * @param opName
 * @param op1
 * @param op2
 * @param op3
 */
void addNewMipsCode(string opName, string op1, string op2, string op3) {
    CodeTableItem *cti = &codeTable[codeTablePointer++];
    cti->name = opName;
    cti->operand1 = op1;
    cti->operand2 = op2;
    cti->operand3 = op3;

//    std::cout << left << setw(20) << cti->name;
//    std::cout << left << setw(15) << cti->operand1;
//    std::cout << left << setw(15) << cti->operand2;
//    std::cout << left << setw(15) << cti->operand3 << endl;
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
 * 通过临时变量编号获得临时变量地址
 * @param qcti
 * @return
 */
string getAddrFromShape(string s) {
    int index =  std::atoi(s.substr(1, s.length()).c_str());
    return  itoa((index + 1) * 4) + "($sp)";
}

string getTmpVarAddr(string tmpVar) {
    int index = std::atoi(tmpVar.substr(1, tmpVar.length()).c_str());//去掉#并转换成数字
    return itoa((4) * (index + 1)) + "($sp)";
}

/**
 * 通过分析全局数组下标的种类添加不同的指令
 * @return
 */
void getOverallIntArrayShiftAndAddMips(string arrayAddr, string op2, string reg1, string reg2, string tVarAddr) {
    if(isANumber(op2)) { //如果是数字则直接
        int shift = std::atoi(op2.c_str());
        addNewMipsCode("lw", reg1, arrayAddr + "+" + itoa(shift * 4), "");
        addNewMipsCode("sw", reg1, tVarAddr, "");//存储进临时变量在内存中的位置
    }
    else if(op2.substr(0, 1) == "#") { //如果是临时变量就用寄存器将变量的值加载至寄存器然后array($reg)
//        int tIndex = std::atoi(op.substr(1, op.length()).c_str());
        addNewMipsCode("lw", reg1, getTmpVarAddr(op2), "");//加载偏移至指定寄存器
        addNewMipsCode("lw", reg2, arrayAddr + "(" + reg1 +  ")", "");//根据偏移加载数组值至指定寄存器
        addNewMipsCode("sw", reg2, tVarAddr, "");//存储进临时变量在内存中的位置
    }
    else { //如果是全局或局部变量
        vector<string> findResult = findInAllTable(op2);
        if(findResult.at(1) == "overall") { //如果是全局变量
            int index = std::atoi(findResult.at(2).c_str());
            TokenTableItem tti = tokenTable[index];
            addNewMipsCode("lw", reg1, op2, "");//通过标签加载值进内存
            addNewMipsCode("mul", reg1, reg1, "4");
            addNewMipsCode("lw", reg2, arrayAddr + "(" + reg1 +  ")", "");//根据偏移加载数组值至指定寄存器
            addNewMipsCode("sw", reg2, tVarAddr, "");//存储进临时变量在内存中的位置
        }
        else { //如果是局部变量
            int index = std::atoi(findResult.at(2).c_str());
            FuncParamTableItem fpti = funcParamTable[index];
            addNewMipsCode("lw", reg1, fpti.addr, "");//通过地址加载值进内存
            addNewMipsCode("mul", reg1, reg1, "4");
            addNewMipsCode("lw", reg2, arrayAddr + "(" + reg1 +  ")", "");//根据偏移加载数组值至指定寄存器
            addNewMipsCode("sw", reg2, tVarAddr, "");//存储进临时变量在内存中的位置
        }
    }
}

/**
 * 通过分析全局数组下标的种类添加不同的指令
 * @return
 */
void getOverallCharArrayShiftAndAddMips(string arrayAddr, string op2, string reg1, string reg2, string tVarAddr) {
    if(isANumber(op2)) { //如果是数字则直接
        int shift = std::atoi(op2.c_str());
        addNewMipsCode("lw", reg1, arrayAddr + "+" + itoa(shift * 4), "");
        addNewMipsCode("and", reg1, reg1, "0xFF");
        addNewMipsCode("sw", reg1, tVarAddr, "");//存储进临时变量在内存中的位置
    }
    else if(op2.substr(0, 1) == "#") { //如果是临时变量就用寄存器将变量的值加载至寄存器然后array($reg)
//        int tIndex = std::atoi(op.substr(1, op.length()).c_str());
        addNewMipsCode("lw", reg1, getTmpVarAddr(op2), "");//加载偏移至指定寄存器
        addNewMipsCode("lw", reg2, arrayAddr + "(" + reg1 +  ")", "");//根据偏移加载数组值至指定寄存器
        addNewMipsCode("and", reg2, reg2, "0xFF");
        addNewMipsCode("sw", reg2, tVarAddr, "");//存储进临时变量在内存中的位置
    }
    else { //如果是全局或局部变量
        vector<string> findResult = findInAllTable(op2);
        if(findResult.at(1) == "overall") { //如果是全局变量
            int index = std::atoi(findResult.at(2).c_str());
            TokenTableItem tti = tokenTable[index];
            addNewMipsCode("lw", reg1, op2, "");//通过标签加载值进内存
            addNewMipsCode("mul", reg1, reg1, "4");
            addNewMipsCode("lw", reg2, arrayAddr + "(" + reg1 +  ")", "");//根据偏移加载数组值至指定寄存器
            addNewMipsCode("and", reg2, reg2, "0xFF");
            addNewMipsCode("sw", reg2, tVarAddr, "");//存储进临时变量在内存中的位置
        }
        else { //如果是局部变量
            int index = std::atoi(findResult.at(2).c_str());
            FuncParamTableItem fpti = funcParamTable[index];
            addNewMipsCode("lw", reg1, fpti.addr, "");//通过地址加载值进内存
            addNewMipsCode("mul", reg1, reg1, "4");
            addNewMipsCode("lw", reg2, arrayAddr + "(" + reg1 +  ")", "");//根据偏移加载数组值至指定寄存器
            addNewMipsCode("and", reg2, reg2, "0xFF");
            addNewMipsCode("sw", reg2, tVarAddr, "");//存储进临时变量在内存中的位置
        }
    }
}

/**
 * 通过分析局部数组下标的种类添加不同的指令
 * @param fpci
 * @param op2
 * @param reg1
 * @param reg2
 * @param tmpVarAddr 存储最终从数组中取出的值的临时变量的地址
 */
void getInnerIntArrayShiftAndAddMips(FuncParamTableItem fpci, string op2, string reg1, string tmpVarAddr) {
    if(isANumber(op2)) {
        int shift = std::atoi(op2.c_str());
        int baseAddr = stringToInt(fpci.addr);
        int shiftAddr = baseAddr - shift * 4;
        string shiftSp = itoa(shiftAddr) + "($fp)";
        addNewMipsCode("lw", reg1, shiftSp, "");
        addNewMipsCode("sw", reg1, tmpVarAddr, "");
    }
    else if(op2.substr(0, 1) == "#") { //如果是临时变量则从相应地址中加载值进寄存器
        addNewMipsCode("lw", reg1, getTmpVarAddr(op2), "");
        addNewMipsCode("mul", reg1, reg1, "4");//将地址乘4对齐
        addNewMipsCode("sub", reg1, reg1, fpci.addr);//reg1保存相对fp的偏移地址
        addNewMipsCode("sub", reg1, "$fp", reg1);
        addNewMipsCode("lw", reg1, "(" + reg1 + ")", "");//从reg1中存储的地址中加载值
        addNewMipsCode("sw", reg1, tmpVarAddr, "");
    }
    else { //如果是全局变量或者局部变量
        vector<string> findResult = findInAllTable(op2);
        int index = stringToInt(findResult.at(2));
        if(findResult.at(1) == "overall") { //如果是全局变量
            addNewMipsCode("lw", reg1, op2, "");//通过标签加载全局变量的值到寄存器
            addNewMipsCode("mul", reg1, reg1, "4");//将值乘4作为偏移
            addNewMipsCode("sub", reg1, reg1, fpci.addr);//reg1保存相对fp的偏移地址
            addNewMipsCode("sub", reg1, "$fp", reg1);
            addNewMipsCode("lw", reg1, "(" + reg1 + ")", "");//从reg1中存储的地址中加载值
            addNewMipsCode("sw", reg1, tmpVarAddr, "");
        }
        else { //如果是局部变量
            FuncParamTableItem tmpFpci = funcParamTable[index];
            addNewMipsCode("lw", reg1, tmpFpci.addr, "");
            addNewMipsCode("mul", reg1, reg1, "4");//将值乘4作为偏移
            addNewMipsCode("sub", reg1, reg1, fpci.addr);//reg1保存相对fp的偏移地址
            addNewMipsCode("sub", reg1, "$fp", reg1);
            addNewMipsCode("lw", reg1, "(" + reg1 + ")", "");//从reg1中存储的地址中加载值
            addNewMipsCode("sw", reg1, tmpVarAddr, "");
        }
    }
}

/**
 * 通过分析局部数组下标的种类添加不同的指令
 * @param fpci
 * @param op2
 * @param reg1
 * @param reg2
 * @param tmpVarAddr 存储最终从数组中取出的值的临时变量的地址
 */
void getInnerCharArrayShiftAndAddMips(FuncParamTableItem fpci, string op2, string reg1, string tmpVarAddr) {
    if(isANumber(op2)) {
        int shift = std::atoi(op2.c_str());
        int baseAddr = stringToInt(fpci.addr);
        int shiftAddr = baseAddr - shift * 4;
        string shiftSp = itoa(shiftAddr) + "($fp)";
        addNewMipsCode("lw", reg1, shiftSp, "");
        addNewMipsCode("and", reg1, reg1, "0xFF");
        addNewMipsCode("sw", reg1, tmpVarAddr, "");
    }
    else if(op2.substr(0, 1) == "#") { //如果是临时变量则从相应地址中加载值进寄存器
        addNewMipsCode("lw", reg1, getTmpVarAddr(op2), "");
        addNewMipsCode("mul", reg1, reg1, "4");//将地址乘4对齐
        addNewMipsCode("sub", reg1, reg1, fpci.addr);//reg1保存相对fp的偏移地址
        addNewMipsCode("sub", reg1, "$fp", reg1);
        addNewMipsCode("lw", reg1, "(" + reg1 + ")", "");//从reg1中存储的地址中加载值
        addNewMipsCode("and", reg1, reg1, "0xFF");
        addNewMipsCode("sw", reg1, tmpVarAddr, "");
    }
    else { //如果是全局变量或者局部变量
        vector<string> findResult = findInAllTable(op2);
        int index = stringToInt(findResult.at(2));
        if(findResult.at(1) == "overall") { //如果是全局变量
            addNewMipsCode("lw", reg1, op2, "");//通过标签加载全局变量的值到寄存器
            addNewMipsCode("mul", reg1, reg1, "4");//将值乘4作为偏移
            addNewMipsCode("sub", reg1, reg1, fpci.addr);//reg1保存相对fp的偏移地址
            addNewMipsCode("sub", reg1, "$fp", reg1);
            addNewMipsCode("lw", reg1, "(" + reg1 + ")", "");//从reg1中存储的地址中加载值
            addNewMipsCode("and", reg1, reg1, "0xFF");
            addNewMipsCode("sw", reg1, tmpVarAddr, "");
        }
        else { //如果是局部变量
            FuncParamTableItem tmpFpci = funcParamTable[index];
            addNewMipsCode("lw", reg1, tmpFpci.addr, "");
            addNewMipsCode("mul", reg1, reg1, "4");//将值乘4作为偏移
            addNewMipsCode("sub", reg1, reg1, fpci.addr);//reg1保存相对fp的偏移地址
            addNewMipsCode("sub", reg1, "$fp", reg1);
            addNewMipsCode("lw", reg1, "(" + reg1 + ")", "");//从reg1中存储的地址中加载值
            addNewMipsCode("and", reg1, reg1, "0xFF");
            addNewMipsCode("sw", reg1, tmpVarAddr, "");
        }
    }
}

/**
 * 处理对数组赋值的指令并生成mips代码
 * @param op1
 * @param op2
 * @param op3
 * @param reg1
 * @param reg2
 * @param reg3
 */
void assignIntArrayShiftAndAddMips(string op1, string op2, string op3, string reg1, string reg2, string reg3) {
    if(isANumber(op1)) { //如果op1是一个数字
        addNewMipsCode("li", reg1, op1, "");//将要赋的值存储在reg1
    }
    else if(op1.substr(0, 1) == "#") { //如果op1是一个临时变量
        addNewMipsCode("lw", reg1, getTmpVarAddr(op1), "");
    }
    else { //如果op1是全局或局部变量
        vector<string> findResult1 = findInAllTable(op1);
        if(findResult1.at(1) == "overall") { //如果是一个全局变量
            addNewMipsCode("lw", reg1, op1, "");
        }
        else { //如果是一个局部变量
            FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];//取出在参数表中的项
            addNewMipsCode("lw", reg1, fpti.addr, "");
        }
    }

    if(isANumber(op2)) { //如果op2是一个数字
        addNewMipsCode("li", reg2, op2, "");//将数组偏移存储在reg2
    }
    else if(op2.substr(0, 1) == "#") { //如果op2是一个临时变量
        addNewMipsCode("lw", reg2, getTmpVarAddr(op1), "");
    }
    else {
        vector<string> findResult2 = findInAllTable(op2);
        if(findResult2.at(1) == "overall") { //如果op2是一个全局变量
            addNewMipsCode("lw", reg2, op1, "");
        }
        else { //如果op2是一个局部变量
            FuncParamTableItem fpti = funcParamTable[stringToInt(findResult2.at(2))];//取出在参数表中的项
            addNewMipsCode("lw", reg2, fpti.addr, "");
        }
    }

    vector<string> findResult3 = findInAllTable(op3);
    if(findResult3.at(1) == "overall") { //如果是一个全局变量
        addNewMipsCode("mul", reg2, reg2, "4");//将reg2中存储的偏移乘4
        addNewMipsCode("sw", reg1, op3 + "(" + reg2 + ")", "");

//        addNewMipsCode("la", reg3, op3, "");//将全局变量的地址加载到寄存器
//        addNewMipsCode("mul", reg2, reg2, "4");//将下标乘4作为偏移地址
//        addNewMipsCode("add", reg3, reg3, reg2);//将地址加上reg2的偏移
//        addNewMipsCode("sw", reg1, "(" + reg3 + ")", "");//将reg1的值存储到指定地址
    }
    else {
        FuncParamTableItem fpti = funcParamTable[stringToInt(findResult3.at(2))];
        addNewMipsCode("add",reg3, "$fp", fpti.addr);//将数组的基地址保存在reg3中(记录的是负数)
        addNewMipsCode("mul", reg2, reg2, "4");//将下标乘4作为偏移地址
        addNewMipsCode("sub",reg3, reg3, reg2);//将保存在reg2中的下标的偏移地址作为基地址的偏移
        addNewMipsCode("sw", reg1, "(" + reg3 + ")", "");//将reg1的值存储到reg3中的指定地址
    }
}

/**
 * 处理对数组赋值的指令并生成mips代码
 * @param op1
 * @param op2
 * @param op3
 * @param reg1
 * @param reg2
 * @param reg3
 */
void assignCharArrayShiftAndAddMips(string op1, string op2, string op3, string reg1, string reg2, string reg3) {
    if(isANumber(op1)) { //如果op1是一个数字
        addNewMipsCode("li", reg1, op1, "");//将要赋的值存储在reg1
    }
    else if(op1.substr(0, 1) == "#") { //如果op1是一个临时变量
        addNewMipsCode("lw", reg1, getTmpVarAddr(op1), "");
    }
    else { //如果op1是全局或局部变量
        vector<string> findResult1 = findInAllTable(op1);
        if(findResult1.at(1) == "overall") { //如果是一个全局变量
            addNewMipsCode("lw", reg1, op1, "");
        }
        else { //如果是一个局部变量
            FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];//取出在参数表中的项
            addNewMipsCode("lw", reg1, fpti.addr, "");
        }
    }

    if(isANumber(op2)) { //如果op2是一个数字
        addNewMipsCode("li", reg2, op2, "");//将数组偏移存储在reg2
    }
    else if(op2.substr(0, 1) == "#") { //如果op2是一个临时变量
        addNewMipsCode("lw", reg2, getTmpVarAddr(op1), "");
    }
    else {
        vector<string> findResult2 = findInAllTable(op2);
        if(findResult2.at(1) == "overall") { //如果op2是一个全局变量
            addNewMipsCode("lw", reg2, op1, "");
        }
        else { //如果op2是一个局部变量
            FuncParamTableItem fpti = funcParamTable[stringToInt(findResult2.at(2))];//取出在参数表中的项
            addNewMipsCode("lw", reg2, fpti.addr, "");
        }
    }

    vector<string> findResult3 = findInAllTable(op3);
    if(findResult3.at(1) == "overall") { //如果是一个全局变量
        addNewMipsCode("mul", reg2, reg2, "4");//将reg2中存储的偏移乘4
        addNewMipsCode("and", reg1, reg1, "0xFF");
        addNewMipsCode("sw", reg1, op3 + "(" + reg2 + ")", "");

//        addNewMipsCode("la", reg3, op3, "");//将全局变量的地址加载到寄存器
//        addNewMipsCode("mul", reg2, reg2, "4");//将下标乘4作为偏移地址
//        addNewMipsCode("add", reg3, reg3, reg2);//将地址加上reg2的偏移
//        addNewMipsCode("sw", reg1, "(" + reg3 + ")", "");//将reg1的值存储到指定地址
    }
    else {
        FuncParamTableItem fpti = funcParamTable[stringToInt(findResult3.at(2))];
        addNewMipsCode("add",reg3, "$fp", fpti.addr);//将数组的基地址保存在reg3中(记录的是负数)
        addNewMipsCode("mul", reg2, reg2, "4");//将下标乘4作为偏移地址
        addNewMipsCode("sub",reg3, reg3, reg2);//将保存在reg2中的下标的偏移地址作为基地址的偏移
        addNewMipsCode("and", reg1, reg1, "0xFF");
        addNewMipsCode("sw", reg1, "(" + reg3 + ")", "");//将reg1的值存储到reg3中的指定地址
    }
}

/**
 * 通过分析操作数的不同种类返回不同的地址
 * @param op
 * @return
 */
string getAddressAndAddMipsInstrByOpKind(string op, string reg) {
    if(isANumber(op)) {
        addNewMipsCode("li", reg, op, "");//如果是数字则加载立即数
        return op;
    }
    else if (op.substr(0, 1) == "#") { //如果是临时变量
        addNewMipsCode("lw", reg, getAddrFromShape(op), "");//从sp的偏移加载临时变量
        return getAddrFromShape(op);
    }
    else {
        vector<string> findResult = findInAllTable(op);
        if(findResult.at(1) == "overall") { //如果是全局变量，就使用data区的标签
            addNewMipsCode("lw", reg, op, "");//否则从内存加载至寄存器
            return op;
        }
        else {
            FuncParamTableItem fpci = funcParamTable[stringToInt(findResult.at(2))];
            addNewMipsCode("lw", reg, fpci.addr, "");//否则从内存加载至寄存器
            return fpci.addr;
        }
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
                case qNewIntCons:
                    addNewMipsCode(qcti.operand1 + ":", ".word", qcti.operand2, "");
                    break;
                case qNewCharCons:
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
                    addNewMipsCode(qcti.operand1 + ":", ".word", "0", "");
                    break;
                case qNewCharVar:
                    addNewMipsCode(qcti.operand1 + ":", ".word", "\0", "");
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
    string preDealFunc = currentDealFunc;//记录之前正在处理的函数
    currentDealFunc = qcti.operand1.substr(0, qcti.operand1.length() - 1);//函数名标签
    addNewMipsCode(qcti.operand1, "", "", "");//添加函数标签
    int index = findTokenInTable(currentDealFunc);//找到函数名在符号表中的位置
    TokenTableItem tti = tokenTable[index];
    int startIndex = tti.paramAddr;
    int paramAccount = tti.paramAccount;//参数个数
    int localVarAccount = tti.localVarAccount;//局部变量个数
    int localVarPointer = 0;//局部变量指针
    int fpShift = 12 + paramAccount * 4;
    int tIndex = 0;//临时变量的下标


    //todo 给参数和局部变量分配空间
    int addParamAddrIndex = 3;
    int i;
    for(i = startIndex; i < startIndex + paramAccount; i++) {
        FuncParamTableItem *fpti = &funcParamTable[i];
        fpti->addr = itoa(- addParamAddrIndex * 4) + "($fp)";
        addParamAddrIndex++;
    }
    for(i = i ; i < startIndex + paramAccount + localVarAccount; i++) {
        FuncParamTableItem *fpti = &funcParamTable[i];
        fpti->addr = itoa(- addParamAddrIndex * 4) + "($fp)";
        addParamAddrIndex++;
    }


    int shift = 8;//fp + ra 计算局部变量偏移
    for(int j = startIndex; j < startIndex + paramAccount + localVarAccount; j++) {
        FuncParamTableItem fpti = funcParamTable[j];
        if(!fpti.isArray) {
            shift += 4;
        }
        else {
            shift = shift + 4 * fpti.arraySize;
        }
    }
    int maxTmpVarIndex = tokenTable[stringToInt(findInAllTable(currentDealFunc).at(2).c_str())].maxTmpVar;
    shift += (maxTmpVarIndex + 1) * 4;
    addNewMipsCode("sw", "$fp", "-4($sp)", "");//保存当前的fp指针
    addNewMipsCode("move", "$fp", "$sp", "");//将fp指针挪到sp的位置
    addNewMipsCode("subu", "$sp", "$sp", itoa(shift));//申请局部变量的空间
    addNewMipsCode("sw", "$ra", "-8($fp)", "");//保存返回地址

//    addNewMipsCode(currentDealFunc, "", "", "");//函数名标签指令
//    addNewMipsCode("sw", "$fp", "($sp)", "");//保存当前的fp指针
//    addNewMipsCode("move", "$fp", "$sp", "");//将fp指针挪到sp的位置
//    addNewMipsCode("subu", "$sp", "$sp", "");//申请局部变量的空间
//    addNewMipsCode("sw", "$ra", "-4($fp)", "");//保存返回地址
//    addNewMipsCode("sw", "$v0", "-8($fp)", "");//保存返回值

    string reg1;
    string reg2;
    string reg3;
    while(true) {
        qcti = qCodeInstrs[qCodeTableScanner++];
        string op1 = qcti.operand1;
        string op2 = qcti.operand2;
        string op3 = qcti.result;
        switch (qcti.quadCodeInstr) {
            case qNewIntCons: {
                //记录在内存的相对地址
                FuncParamTableItem *fpti = &funcParamTable[startIndex + paramAccount + localVarPointer++];
                fpti->addr = "-" + itoa(fpShift) + "($fp)";
                addNewMipsCode("li", "$t8", op2, "");
                addNewMipsCode("sw", "$t8", fpti->addr, "");
                fpShift += 4;
            }
                break;
            case qNewCharCons: {
                //记录在内存的相对地址
                FuncParamTableItem *fpti = &funcParamTable[startIndex + paramAccount + localVarPointer++];
                fpti->addr = "-" + itoa(fpShift) + "($fp)";
                addNewMipsCode("li", "$t8", "'" + op2 + "'", "");
                addNewMipsCode("sw", "$t8", fpti->addr, "");
                fpShift += 4;
                break;
            }
            case qNewIntVar: {
                if (op1.substr(0, 1) == "#") { //如果是临时变量则添加到临时变量表
                    //todo 临时变量则添加到临时变量表
                } else {
                    FuncParamTableItem *fpti = &funcParamTable[startIndex + paramAccount + localVarPointer++];
                    fpti->addr = "-" + itoa(fpShift) + "($fp)";
                    fpShift += 4;
                }
                break;
            }
            case qNewCharVar: {
                if (op1.substr(0, 1) == "#") { //如果是临时变量则添加到临时变量表
                    //todo 临时变量则添加到临时变量表
                } else {
                    FuncParamTableItem *fpti = &funcParamTable[startIndex + paramAccount + localVarPointer++];
                    fpti->addr = "-" + itoa(fpShift) + "($fp)";
                    fpShift += 4;
                }
                break;
            }
            case qNewIntArray: {
                //记录在内存的起始地址和长度
                FuncParamTableItem *fpti = &funcParamTable[startIndex + paramAccount + localVarPointer++];
                fpti->addr = "-" + itoa(fpShift);
                fpShift += fpti->arraySize * 4;
                break;
            }
            case qNewCharArray: {
                //记录在内存的起始地址和长度
                FuncParamTableItem *fpti = &funcParamTable[startIndex + paramAccount + localVarPointer++];
                fpti->addr = "-" + itoa(fpShift);
                fpShift += fpti->arraySize * 4;
                break;
            }
            case qLabel: {
                addNewMipsCode(op1, "", "", "");//添加普通标签
                break;
            }
            case qFuncLabel: {
                cout << "error" << endl;
                break;
            }
            case qFuncEndLabel: {
                addNewMipsCode(op1, "", "", "");//函数结束标签
                if(op1 != "$end_main:") {
                    addNewMipsCode("lw", "$ra", "-8($fp)", "");//保存返回地址
                    addNewMipsCode("move", "$sp", "$fp", "");//退栈
                    addNewMipsCode("lw", "$fp", "-4($fp)", "");//获得之前的fp指针
                    addNewMipsCode("jr", "$ra", "", "");//return
                }
                else { //如果是main函数则exit
                    addNewMipsCode("li", "$v0", "10", "");
                    addNewMipsCode("syscall", "", "", "");
                }
                return;
            }
            case qPlus: {
                //操作数1
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("add", "$t9", "$t8", "$t9");//计算
                tIndex = std::atoi(op3.substr(1, op3.length()).c_str());
                addNewMipsCode("sw", "$t9", itoa((tIndex + 1) * 4) + "($sp)", "");//保存结果至内存
                break;
            }
            case qSub: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("sub", "$t9", "$t8", "$t9");//计算
                tIndex = std::atoi(op3.substr(1, op3.length()).c_str());
                addNewMipsCode("sw", "$t9", itoa((tIndex + 1) * 4) + "($sp)", "");//保存结果至内存
                break;
            }
            case qMul: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("mul", "$t9", "$t8", "$t9");//计算
                tIndex = std::atoi(op3.substr(1, op3.length()).c_str());
                addNewMipsCode("sw", "$t9", itoa((tIndex + 1) * 4) + "($sp)", "");//保存结果至内存
                break;
            }
            case qDiv: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("div", "$t9", "$t8", "$t9");//计算
                tIndex = std::atoi(op3.substr(1, op3.length()).c_str());
                addNewMipsCode("sw", "$t9", itoa((tIndex + 1) * 4) + "($sp)", "");//保存结果至内存
                break;
            }
            case qJl: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("blt", "$t8", "$t9", op3);
                break;
            }
            case qJg: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("bgt", "$t8", "$t9", op3);
                break;
            }
            case qJne: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("bne", "$t8", "$t9", op3);
                break;
            }
            case qJle: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("ble", "$t8", "$t9", op3);
                break;
            }
            case qJge: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("bge", "$t8", "$t9", op3);
                break;
            }
            case qJe: {
                getAddressAndAddMipsInstrByOpKind(op1, "$t8");//将操作数1加载至寄存器
                getAddressAndAddMipsInstrByOpKind(op2, "$t9");//将操作数2加载至寄存器
                addNewMipsCode("beq", "$t8", "$t9", op3);
                break;
            }
            case qJ: {
                addNewMipsCode("j", qcti.operand1, "", "");
                break;
            }
            case qJFunc: {
//                addNewMipsCode("sw", "$fp", "($sp)", "");//保存当前的fp指针
//                addNewMipsCode("move", "$fp", "$sp", "");//将fp指针挪到sp的位置
//                int shift = 0;//计算局部变量偏移
//                if (paramAccount <= 4)
//                    shift = localVarAccount * 4;
//                else
//                    shift = (paramAccount + localVarAccount - 4) * 4;
//                addNewMipsCode("subu", "$sp", "$sp", itoa(shift));//申请局部变量的空间
//                addNewMipsCode("sw", "$ra", "-4($fp)", "");//保存返回地址
//                addNewMipsCode("sw", "$v0", "-8($fp)", "");//保存返回值
                addNewMipsCode("jal", qcti.operand1, "", "");//跳转
                if (paramAccount > 4) { //todo 退出传递参数时候使用的栈
                    addNewMipsCode("subu", "$sp", "$sp", itoa((paramAccount - 4) * 4));//todo 退出传递参数时候使用的栈
                }
                break;
            }
            case qGetArrayIntValue: {
                vector<string> findResult1 = findInAllTable(op1);
                if (findResult1.at(1) == "overall") {
                    getOverallIntArrayShiftAndAddMips(op1, op2, "$t8", "$t9", getTmpVarAddr(op3));
                } else {
                    FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                    getInnerIntArrayShiftAndAddMips(fpti, op2, "$t8", getTmpVarAddr(op3));
                }
                break;
            }
            case qGetArrayCharValue: {
                vector<string> findResult1 = findInAllTable(op1);
                if (findResult1.at(1) == "overall") {
                    getOverallCharArrayShiftAndAddMips(op1, op2, "$t8", "$t9", getTmpVarAddr(op3));
                } else {
                    FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                    getInnerCharArrayShiftAndAddMips(fpti, op2, "$t8", getTmpVarAddr(op3));
                }
                break;
            }
            case qAssignInt:{
                if(isANumber(op1)) {
                    addNewMipsCode("li", "$t8", op1, "");
                    vector<string> findResult2 = findInAllTable(op2);
                    if (findResult2.at(1) == "overall") { //如果op2是全局变量
                        addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                    } else { //op2是局部变量
                        FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                        addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                    }
                }
                else if (op1.substr(0, 1) == "#") { //如果op1是临时变量
                    addNewMipsCode("lw", "$t8", getTmpVarAddr(op1), "");
                    vector<string> findResult2 = findInAllTable(op2);
                    if (findResult2.at(1) == "overall") { //如果op2是全局变量
                        addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                    } else { //op2是局部变量
                        FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                        addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                    }
                }
                else { //如果op1是全局或临时变量
                    vector<string> findResult1 = findInAllTable(op1);
                    if (findResult1.at(1) == "overall") { //如果op1是全局变量
                        vector<string> findResult2 = findInAllTable(op2);
                        if (findResult2.at(1) == "overall") { //如果op2是全局变量
                            addNewMipsCode("lw", "$t8", op1, "");//将op1加载至寄存器
                            addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                        } else { //op2是局部变量
                            FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                            addNewMipsCode("lw", "$t8", op1, "");//将op1加载至寄存器
                            addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                        }
                    } else { //op1是局部变量
                        vector<string> findResult2 = findInAllTable(op2);
                        FuncParamTableItem fpti1 = funcParamTable[stringToInt(findResult1.at(2))];
                        if (findResult2.at(1) == "overall") { //如果op2是全局变量
                            addNewMipsCode("lw", "$t8", fpti1.addr, "");//将op1加载至寄存器
                            addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                        } else { //op2是局部变量
                            FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                            addNewMipsCode("lw", "$t8", fpti1.addr, "");//将op1加载至寄存器
                            addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                        }
                    }
                }
                break;
            }
            case qAssignChar:{
                if(isANumber(op1)) {
                    addNewMipsCode("li", "$t8", op1, "");
                    addNewMipsCode("and", "$t8", "$t8", "0xFF");
                    vector<string> findResult2 = findInAllTable(op2);
                    if (findResult2.at(1) == "overall") { //如果op2是全局变量
                        addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                    } else { //op2是局部变量
                        FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                        addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                    }
                }
                else if (op1.substr(0, 1) == "#") { //如果op1是临时变量
                    addNewMipsCode("lw", "$t8", getTmpVarAddr(op1), "");
                    addNewMipsCode("and", "$t8", "$t8", "0xFF");
                    vector<string> findResult2 = findInAllTable(op2);
                    if (findResult2.at(1) == "overall") { //如果op2是全局变量
                        addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                    } else { //op2是局部变量
                        FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                        addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                    }
                }
                else { //如果op1是全局或临时变量
                    vector<string> findResult1 = findInAllTable(op1);
                    if (findResult1.at(1) == "overall") { //如果op1是全局变量
                        vector<string> findResult2 = findInAllTable(op2);
                        if (findResult2.at(1) == "overall") { //如果op2是全局变量
                            addNewMipsCode("lw", "$t8", op1, "");//将op1加载至寄存器
                            addNewMipsCode("and", "$t8", "$t8", "0xFF");
                            addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                        } else { //op2是局部变量
                            FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                            addNewMipsCode("lw", "$t8", op1, "");//将op1加载至寄存器
                            addNewMipsCode("and", "$t8", "$t8", "0xFF");
                            addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                        }
                    } else { //op1是局部变量
                        vector<string> findResult2 = findInAllTable(op2);
                        FuncParamTableItem fpti1 = funcParamTable[stringToInt(findResult1.at(2))];
                        if (findResult2.at(1) == "overall") { //如果op2是全局变量
                            addNewMipsCode("lw", "$t8", fpti1.addr, "");//将op1加载至寄存器
                            addNewMipsCode("and", "$t8", "$t8", "0xFF");
                            addNewMipsCode("sw", "$t8", op2, "");//将寄存器的值存至目标寄存器
                        } else { //op2是局部变量
                            FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                            addNewMipsCode("lw", "$t8", fpti1.addr, "");//将op1加载至寄存器
                            addNewMipsCode("and", "$t8", "$t8", "0xFF");
                            addNewMipsCode("sw", "$t8", fpti2.addr, "");//将寄存器的值存至目标寄存器
                        }
                    }
                }
                break;
            }
            case qAssignIntArray: {
                assignIntArrayShiftAndAddMips(op1, op2, op3, "$t7", "$t8", "$t9");
                break;
            }
            case qAssignCharArray: {
                assignCharArrayShiftAndAddMips(op1, op2, op3, "$t7", "$t8", "$t9");
                break;
            }
            case qReverse: {
                reg1 = "$t8";
                if(isANumber(op1)) {
                    addNewMipsCode("li", reg1, op1, "");
                }
                else if (op1.substr(0, 1) == "#") { //如果是临时变量
                    addNewMipsCode("lw", reg1, getTmpVarAddr(op1), "");
                }
                else {
                    vector<string> findResult1 = findInAllTable(op1);
                    if (findResult1.at(1) == "overall") { //如果是全局变量
                        addNewMipsCode("lw", reg1, op1, "");
                    } else { //如果是局部变量
                        FuncParamTableItem fpti1 = funcParamTable[stringToInt(findResult1.at(2))];
                        addNewMipsCode("lw", reg1, fpti1.addr, "");
                    }
                }
                addNewMipsCode("mul", reg1, reg1, "-1");//取反
                if (op2.substr(0, 1) == "#") { //如果是临时变量
                    addNewMipsCode("sw", reg1, getTmpVarAddr(op2), "");
                } else {
                    vector<string> findResult2 = findInAllTable(op2);
                    if (findResult2.at(1) == "overall") { //如果是全局变量
                        addNewMipsCode("sw", reg1, op2, "");
                    } else { //如果是局部变量
                        FuncParamTableItem fpti2 = funcParamTable[stringToInt(findResult2.at(2))];
                        addNewMipsCode("sw", reg1, fpti2.addr, "");
                    }
                }
                break;
            }
            case qPassIntParam:
            case qPassCharParam: {
                int passIndex = 0;
                reg1 = "$t8";
                string passParamSaveAddr = "";
                while (qcti.quadCodeInstr == qPassIntParam || qcti.quadCodeInstr == qPassCharParam) {
                    if (passIndex < 4) { //如果小于4则使用a寄存器
                        passParamSaveAddr = "$a" + itoa(passIndex);
                        if (isANumber(op1)) { //如果是一个数字
                            addNewMipsCode("li", passParamSaveAddr, op1, "");
                        } else if (op1.substr(0, 1) == "#") { //如果是临时变量
                            addNewMipsCode("lw", passParamSaveAddr, getTmpVarAddr(op1), "");
                        } else {
                            vector<string> findResult1 = findInAllTable(op1);
                            if (findResult1.at(1) == "overall") { //如果是全局变量
                                addNewMipsCode("lw", passParamSaveAddr, op1, "");
                            } else { //如果是局部变量
                                FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                                addNewMipsCode("lw", passParamSaveAddr, fpti.addr, "");
                            }
                        }
                    } else { //否则使用运行栈
                        addNewMipsCode("sub", "$sp", "$sp", "4");//申请新的sp空间
                        if (isANumber(op1)) { //如果是一个数字
                            addNewMipsCode("li", reg1, op1, "");
                            addNewMipsCode("sw", reg1, "($sp)", "");
                        } else if (op1.substr(0, 1) == "#") { //如果是临时变量
                            addNewMipsCode("lw", reg1, getTmpVarAddr(op1), "");
                            addNewMipsCode("sw", reg1, "($sp)", "");
                        } else {
                            vector<string> findResult1 = findInAllTable(op1);
                            if (findResult1.at(1) == "overall") { //如果是全局变量
                                addNewMipsCode("lw", reg1, op1, "");
                                addNewMipsCode("sw", reg1, "($sp)", "");
                            } else { //如果是局部变量
                                FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                                addNewMipsCode("lw", reg1, fpti.addr, "");
                                addNewMipsCode("sw", reg1, "($sp)", "");
                            }
                        }
                    }

//                    addNewMipsCode("lw", "$a" + itoa(passIndex), "", "");
                    qcti = qCodeInstrs[qCodeTableScanner++];
                    op1 = qcti.operand1;
                    op2 = qcti.operand2;
                    op3 = qcti.result;
                    passIndex++;
                }
                qCodeTableScanner--;
                break;
            }
            case qGetIntParam:
            case qGetCharParam: {
                int getIndex = 0;
                reg1 = "";
                string getParamSaveAddr = "";
                reg1 = "$t8";
                i = paramAccount;//参数个数
                while (qcti.quadCodeInstr == qGetIntParam || qcti.quadCodeInstr == qGetCharParam) {
                    if (getIndex < 4) { //如果小于4则从a寄存器里取值
                        if(qcti.quadCodeInstr == qGetIntParam)
                            addNewMipsCode("sw", "$a" + itoa(getIndex), itoa(-(getIndex + 3) * 4) + "($fp)", "");
                        else {
                            addNewMipsCode("and", "$a0", "$a0", "0xFF");
                            addNewMipsCode("sw", "$a" + itoa(getIndex), itoa(-(getIndex + 3) * 4) + "($fp)", "");
                        }
                    } else { //否则从栈中取值
                        if(qcti.quadCodeInstr == qGetIntParam) {
                            addNewMipsCode("lw", reg1, itoa((i - 5) * 4) + "($fp)", "");
                            addNewMipsCode("sw", reg1, itoa(-(getIndex + 3) * 4) + "($fp)", "");
                        }
                        else {
                            addNewMipsCode("lw", reg1, itoa((i - 5) * 4) + "($fp)", "");
                            addNewMipsCode("and", reg1, reg1, "0xFF");
                            addNewMipsCode("sw", reg1, itoa(-(getIndex + 3) * 4) + "($fp)", "");
                        }
                        i--;
                    }
                    qcti = qCodeInstrs[qCodeTableScanner++];
                    op1 = qcti.operand1;
                    op2 = qcti.operand2;
                    op3 = qcti.result;
                    getIndex++;
                }
                qCodeTableScanner--;
                break;
            }
            case qScanfInt: {
                addNewMipsCode("li", "$v0", "5", "");
                addNewMipsCode("syscall", "", "", "");
                vector<string> findResult1 = findInAllTable(op1);
                if(findResult1.at(1) == "overall") { //如果是全局变量就用标签存
                    addNewMipsCode("sw", "$v0", op1, "");
                }
                else {
                    FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                    addNewMipsCode("sw", "$v0", fpti.addr, "");
                }
                break;
            }
            case qScanfChar: {
                addNewMipsCode("li", "$v0", "12", "");
                addNewMipsCode("syscall", "", "", "");
                addNewMipsCode("and", "$v0", "$v0", "0xFF");
                vector<string> findResult1 = findInAllTable(op1);
                if(findResult1.at(1) == "overall") { //如果是全局变量就用标签存
                    addNewMipsCode("sw", "$v0", op1, "");
                }
                else {
                    FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                    addNewMipsCode("sw", "$v0", fpti.addr, "");
                }
                break;
            }
            case qPrintfString: {
                addNewMipsCode("la", "$a0", op1, "");
                addNewMipsCode("li", "$v0", "4", "");
                addNewMipsCode("syscall", "", "", "");
                break;
            }
            case qPrintfInt: {
                if (isANumber(op1)) { //如果是一个数字
                    addNewMipsCode("li", "$a0", op1, "");
                } else if (op1.substr(0, 1) == "#") { //如果是一个临时变量
                    addNewMipsCode("lw", "$a0", getTmpVarAddr(op1), "");
                } else {
                    vector<string> findResult1 = findInAllTable(op1);
                    if (findResult1.at(1) == "overall") { //如果是一个全局变量
                        addNewMipsCode("lw", "$a0", op1, "");
                    } else { //如果是一个局部变量
                        FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                        addNewMipsCode("lw", "$a0", fpti.addr, "");
                    }
                }
                addNewMipsCode("li", "$v0", "1", "");
                addNewMipsCode("syscall", "", "", "");
                break;
            }
            case qPrintfChar: {
                if(isANumber(op1)) { //如果是一个数字
                    addNewMipsCode("li", "$a0", op1, "");
                }
                else if (op1.substr(0, 1) == "#") { //如果是一个临时变量
                    addNewMipsCode("lw", "$a0", getTmpVarAddr(op1), "");
                } else {
                    vector<string> findResult1 = findInAllTable(op1);
                    if (findResult1.at(1) == "overall") { //如果是一个全局变量
                        addNewMipsCode("lw", "$a0", op1, "");
                    } else { //如果是一个局部变量
                        FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                        addNewMipsCode("lw", "$a0", fpti.addr, "");
                    }
                }
                addNewMipsCode("li", "$v0", "11", "");
                addNewMipsCode("syscall", "", "", "");
                break;
            }
            case qGetReturnInt:
            case qGetReturnChar: {
                addNewMipsCode("sw", "$v0", getTmpVarAddr(op2), "");//从v0寄存器中取值
                break;
            }
            case qPutReturnInt:
            case qPutReturnChar: {
                reg1 = "$v0";
                if (isANumber(op1)) { //op1如果是一个数字
                    addNewMipsCode("li", reg1, op1, "");
                } else if (op1.substr(0, 1) == "#") { //如果是一个临时变量
                    addNewMipsCode("lw", "$v0", getTmpVarAddr(op1), "");
                } else { //如果是一个全局或局部变量
                    vector<string> findResult1 = findInAllTable(op1);
                    if (findResult1.at(1) == "overall") { //如果是一个全局变量
                        addNewMipsCode("lw", "$v0", op1, "");
                    } else {
                        FuncParamTableItem fpti = funcParamTable[stringToInt(findResult1.at(2))];
                        addNewMipsCode("lw", "$v0", fpti.addr, "");
                    }
                }
                break;
            }
            case qSaveAddr:
                break;
            case qLoadAddr:
                break;
            case qReturn:
                break;
            case qCallFunc:
                break;
            case qPassParam:
                break;
            default:
                break;
        }
    }

}

/**
 * 生成MIPS代码
 */
void generateAll() {
    addNewMipsCode(".data", "", "", "");//声明data区
    generateOverallConst();//生成全局常量
    generateOverallVar();//生成全局变量
    generateAllStrings();//声明所有的字符串
    addNewMipsCode(".text", "", "", "");//声明text区
    addNewMipsCode("move", "$fp", "$sp", "");//初始化sp
    addNewMipsCode("j", "main", "", "");//跳到main函数

    QuadCodeTableItem qcti = qCodeInstrs[qCodeTableScanner++];
    do  {
        generateFunc(qcti);//生成函数
        qcti = qCodeInstrs[qCodeTableScanner++];
    } while(qcti.quadCodeInstr == qFuncLabel);
    printAllMipsCode();//输出所有mips目标代码
    printToMipsFile();//输出到mips文件
}

/**
 * 将MIPS代码打印到MIPS文件
 */
void printToMipsFile() {
    ofstream outFile;
    outFile.open("/Users/billy/Documents/Github/Compiler/14061131_test.mips");
    for(int i = 0; i < codeTablePointer; i++) {
        CodeTableItem *cti = &codeTable[i];
        outFile << left << setw(10) << cti->name;
        outFile << left << setw(10) << cti->operand1;
        outFile << left << setw(10) << cti->operand2;
        outFile << left << setw(10) << cti->operand3 << endl;
    }
    outFile.close();
}
