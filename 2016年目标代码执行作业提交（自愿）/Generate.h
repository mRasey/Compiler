//
// Created by 王震 on 2016/11/25.
//

#ifndef COMPILER_GENERATE_H
#define COMPILER_GENERATE_H

#include "Analysis.h"

typedef struct {
    int value;//变量值
} TmpVarItem;

typedef struct {

} MipsTableItem;

void generateOverallConst() ;
void generateAll() ;
void printToMipsFile() ;

#endif //COMPILER_GENERATE_H
