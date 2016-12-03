//
// Created by 王震 on 2016/11/25.
//

#include "Analysis.h"
#include "Generate.h"

extern FILE* in;
extern bool ifCanGenerated;

/**
 * 主函数
 * @return 0
 */
int main() {
    init();
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/14061131_test.txt", "r");
    in = fopen("/Users/billy/Documents/Github/Compiler/test/testCode.txt", "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/publictest.txt", "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/test4.txt", "r");
    grammaticalAnalysis();

//    printf("***********start***********\n");
//    printAllSymbol();
//    printf("************end************\n");
    if(ifCanGenerated) {
        printAllQCode();
        printTokenTable();
        printFuncParamTable();
        generateAll();
//        printToMipsFile();
    }
    else {
        cout << "\n生成失败" << endl;
    }
    return 0;
}