//
// Created by 王震 on 2016/11/25.
//

#include <fstream>
#include "Analysis.h"
#include "Generate.h"

extern FILE* in;
extern bool ifCanGenerated;
extern bool hasMainFunc;

/**
 * 主函数
 * @return 0
 */
int main() {
    init();
    string path;
    cin >> path;

    in = fopen(path.c_str(), "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/14061131_test.txt", "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/testCode.txt", "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/test.txt", "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/publictest.txt", "r");
//    in = fopen("/Users/billy/Documents/Github/Compiler/test/test4.txt", "r");
    try {
        grammaticalAnalysis();
    }
    catch (exception& e) {
        cout << "\n生成失败" << endl;
    }
//    printAllSymbol();
    try {
        if (!hasMainFunc)
            throw 32;
        if (ifCanGenerated) {
            printAllQCode();
            printTokenTable();
            printFuncParamTable();
            generateAll();
        } else {
            cout << "\n生成失败" << endl;
        }
    }
    catch (int e) {
        errorHandler(e);
        cout << "\n生成失败" << endl;
    }
    return 0;
}