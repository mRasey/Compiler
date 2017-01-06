//
// Created by 王震 on 2016/12/2.
//

#include "ErrorInfo.h"

extern char symbol[1024];
extern int lineNumber;
extern char readIn[1024];
extern bool ifCanGenerated;
extern string preLine;

void errorHandler(int index) {
    ifCanGenerated = false;//如果出现错误则无法生成代码
    cout << "error at line " << lineNumber << ": " << index << " ";
    switch (index) {
        case 1:
//            cout << "重定义的标识符 " << symbol << endl;
            cout << "Heavily defined identifier " << symbol << endl;
            break;
        case 2:
//            cout << "缺少赋值号 " << endl;
            cout << "Missing assign(=) " << endl;
            break;
        case 3:
//            cout << "赋值与类型不匹配或未赋值 " << endl;
            cout << "Assignment and type do not match or not assign " << endl;
            break;
        case 4:
//            cout << "缺少分号或逗号 " << endl;
            cout << "Missing semicolon or comma " << endl;
            break;
        case 5:
//            cout << "变量声明缺少类型说明 " << endl;
            cout << "Variable declaration missing type description " << endl;
            break;
        case 6:
//            cout << "使用保留字作为变量名 " << endl;
            cout << "Use reserved words as variable names " << endl;
            break;
        case 7:
//            cout << "不应该出现的语句" << endl;
            cout << "Should not appear in the statement" << endl;
            break;
        case 8:
//            cout << "函数声明缺少函数名" << endl;
            cout << "Function declaration missing function name" << endl;
            break;
        case 9:
//            cout << "缺少左中括号" << endl;
            cout << "Missing left bracket" << endl;
            break;
        case 10:
//            cout << "数组大小声明未使用数字" << endl;
            cout << "Array size statement does not use number" << endl;
            break;
        case 11:
//            cout << "非法的语句" << endl;
            cout << "Illegal statement" << endl;
            break;
        case 12:
//            cout << "缺少左括号" << endl;
            cout << "Missing left bracket" << endl;
            break;
        case 13:
//            cout << "缺少右中括号" << endl;
            cout << "Missing right bracket" << endl;
            break;
        case 14:
//            cout << "非法的分隔符" << endl;
            cout << "Illegal separator" << endl;
            break;
        case 15:
//            cout << "不合法的数字" << endl;
            cout << "Illegal numbers" << endl;
            break;
        case 16:
//            cout << "不合法的字符" << endl;
            cout << "Illegal character" << endl;
            break;
        case 17:
//            cout << "不合法的声明类型" << endl;
            cout << "Illegal declaration type" << endl;
            break;
        case 18:
//            cout << "缺少分号" << endl;
            cout << "Missing semicolon" << endl;
//            cout << "\t" << preLine << endl;
            break;
        case 19:
//            cout << "不应该出现的 } " << endl;
            cout << "Should not appear } " << endl;
            break;
        case 20:
//            cout << "对有返回值函数使用了无返回值的return" << endl;
            cout << "The return value function is used with a return that has no return value." << endl;
            break;
        case 21:
//            cout << "不合法的返回语句" << endl;
            cout << "Illegal return statement" << endl;
            break;
        case 22:
//            cout << "出现了空的表达式" << endl;
            cout << "There is an empty expression." << endl;
            break;
        case 23:
//            cout << "缺少右括号" << endl;
            cout << "right parenthesis missing" << endl;
            break;
        case 24:
//            cout << "缺少分号" << endl;
            cout << "Missing semicolon" << endl;
            break;
        case 25:
//            cout << "缺少左括号" << endl;
            cout << "Missing left bracket" << endl;
            break;
        case 26:
//            cout << "使用了未定义的变量" << endl;
            cout << "Use undefined variables" << endl;
            break;
        case 27:
//            cout << "非法的操作符 " << symbol << endl;
            cout << "Illegal operator " << symbol << endl;
            break;
        case 28:
//            cout << "不合法的因子" << endl;
            cout << "Illegal factor" << endl;
            break;
        case 29:
//            cout << "参数的个数不匹配" << endl;
            cout << "The number of parameters does not match" << endl;
            break;
        case 30:
//            cout << "缺少右花括号" << endl;
            cout << "Missing right curly braces" << endl;
            break;
        case 31:
//            cout << "main函数返回值必须为void型" << endl;
            cout << "The return value of the main function must be of type void" << endl;
            break;
        case 32:
//            cout << "缺少main函数" << endl;
            cout << "Missing main function" << endl;
            break;
        case 33:
//            cout << "给常量赋值或赋值类型不匹配" << endl;
            cout << "Does not match the type of constant assignment or assignment" << endl;
            break;
        case 34:
//            cout << "非法的声明全局常量的位置" << endl;
            cout << "The illegal declaration of the position of the global constant" << endl;
            break;
        case 35:
//            cout << "非法的声明全局变量的位置" << endl;
            cout << "Illegal declaration of global variables." << endl;
            break;
        case 36:
//            cout << "非法的声明局部常量的位置" << endl;
            cout << "Illegal declaration of local constant position" << endl;
            break;
        case 37:
//            cout << "非法的声明局部变量的位置" << endl;
            cout << "Illegal declaration of local variables." << endl;
            break;
        case 38:
//            cout << "printf有过多的参数" << endl;
            cout << "Printf has too many parameters" << endl;
            break;
        case 39:
//            cout << "printf参数错误" << endl;
            cout << "Printf parameter error" << endl;
            break;
        case 40:
//            cout << "不合法的字符声明" << endl;
            cout << "Illegal character declaration" << endl;
            break;
        case 41:
//            cout << "缺少右引号的字符串声明" << endl;
            cout << "String declaration missing right quote" << endl;
            break;
        case 42:
//            cout << "缺少右引号的字符声明" << endl;
            cout << "A character declaration that lacks a right quote" << endl;
            break;
        case 43:
            cout << "error in scanf" << endl;
            break;
        default:
//            cout << "未知错误" << endl;
            cout << "unknown error" << endl;
            break;
    }
    cout << "\t" << preLine;
    cout << "\t" << readIn << endl;
}