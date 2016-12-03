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
            cout << "重定义的标识符 " << symbol << endl;
            break;
        case 2:
            cout << "缺少赋值号 " << endl;
            break;
        case 3:
            cout << "赋值与类型不匹配或未赋值 " << endl;
            break;
        case 4:
            cout << "缺少分号或逗号 " << endl;
            break;
        case 5:
            cout << "变量声明缺少类型说明 " << endl;
            break;
        case 6:
            cout << "使用保留字作为变量名 " << endl;
            break;
        case 7:
            cout << "不应该出现的语句" << endl;
            break;
        case 8:
            cout << "函数声明缺少函数名" << endl;
            break;
        case 9:
            cout << "缺少左中括号" << endl;
            break;
        case 10:
            cout << "数组大小声明未使用数字" << endl;
            break;
        case 11:
            cout << "非法的语句" << endl;
            break;
        case 12:
            cout << "缺少左括号" << endl;
            break;
        case 13:
            cout << "缺少右中括号" << endl;
            break;
        case 14:
            cout << "非法的分隔符" << endl;
            break;
        case 15:
            cout << "不合法的数字" << endl;
            break;
        case 16:
            cout << "不合法的字符" << endl;
            break;
        case 17:
            cout << "不合法的声明类型" << endl;
            break;
        case 18:
            cout << "缺少分号" << endl;
//            cout << "\t" << preLine << endl;
            break;
        case 19:
            cout << "不应该出现的 } " << endl;
            break;
        case 20:
            cout << "对有返回值函数使用了无返回值的return" << endl;
            break;
        case 21:
            cout << "不合法的返回语句" << endl;
            break;
        case 22:
            cout << "出现了空的表达式" << endl;
            break;
        case 23:
            cout << "缺少右括号" << endl;
            break;
        case 24:
            cout << "缺少分号" << endl;
            break;
        case 25:
            cout << "缺少左括号" << endl;
            break;
        case 26:
            cout << "使用了未定义的变量" << endl;
            break;
        case 27:
            cout << "非法的操作符 " << symbol << endl;
            break;
        case 28:
            cout << "不合法的因子" << endl;
            break;
        case 29:
            cout << "参数的个数不匹配" << endl;
            break;
        case 30:
            cout << "缺少右花括号" << endl;
            break;
        default:
            cout << "未知错误" << endl;
            break;
    }
    cout << "\t" << preLine;
    cout << "\t" << readIn << endl;
}