//
// Created by 王震 on 2016/11/25.
//

#ifndef COMPILER_GENERATE_H
#define COMPILER_GENERATE_H

#include "Analysis.h"

typedef struct {
    string name;//变量名
    string belongFunc;//所属函数
    int value;//变量值
} TmpVarItem;

typedef struct {

} MipsTableItem;

void generateOverallConst() ;
void generateAll() ;

#endif //COMPILER_GENERATE_H
