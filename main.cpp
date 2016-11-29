//
// Created by 王震 on 2016/11/25.
//

#include "Analysis.h"
#include "Generate.h"

extern FILE* in;

/**
 * 主函数
 * @return 0
 */
int main() {
    init();
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/14061131_test.txt", "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/testCode.txt", "r");
    in = fopen("/Users/billy/Documents/Github/Compiler/test/publictest.txt", "r");
    grammaticalAnalysis();
    printAllQCode();
    printTokenTable();
    printFuncParamTable();
//    printf("***********start***********\n");
//    printAllSymbol();
//    printf("************end************\n");
    generateAll();
//    printToMipsFile();
    return 0;
}