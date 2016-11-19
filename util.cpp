//
// Created by 王震 on 2016/11/15.
//

#include <cstring>
#include <cstdlib>

/**
 * 判断一个字符串是否是数字
 * @param s 要判断的字符
 * @return 如果是数字返回true，否则返回false
 */
bool isNumber(char* s) {
    for(int i = 0; i < strlen(s); i++) {
        if(s[i] < '0' || s[i] > '9')
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
           || s[0] == '_'
           || ('0' <= s[0] && s[0] <= '9')
           || ('A' <= s[0] && s[0] <= 'z');
}