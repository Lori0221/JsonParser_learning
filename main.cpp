#include<iostream>
using namespace std;

#include "json.h"
using namespace yazi::json;


int main(){
    // // json 测试
    // Json v1;
    // Json v2 = true;
    // Json v3 = 123;
    // Json v4 = 1.23;
    // Json v5 = "hello world";
    // std::cout << "my json" << endl;

    // // json转基本类型 => 运算符重载
    // bool b = v2;
    // int i = v3;
    // double f = v4;
    // const string &s = v5;

    // json数组类型, 通过索引和append来添加元素，然后通过下标可以获得json的值且能动态转化为基本类型
    // 重载中括号[]
    // Json arr;
    // arr[0] = true;
    // arr[1] = 234;

    // arr.append(1.33);
    // arr.append("haha");

    // // 想看到指针里的内容，实现字符串形式内容展示
    // std::cout << arr.str() << std::endl;

    // bool b1 = arr[0];
    // int i1 = arr[1];
    // double f1 = arr[2];
    // const string &s1 = arr[3];


    // json对象, 括号重载, 赋值运算符重载
    Json obj;
    obj["bool"] = true;
    obj["int"] = 555;
    obj["double"] = "5.20";
    obj["str"] = "woc";

    std::cout << obj.str() << std::endl;
    std::cout << obj.has("bool") << std::endl;
    std::cout << obj.has("woc") << std::endl;

    obj.remove("bool");
    std::cout << obj.str() << std::endl;
    

    
    // Json arr1;
    // arr1[0] = true;
    // arr1[1] = 234;

    // arr1.append(1.33);
    // arr1.append("haha");

    // std::cout << arr1.has(0) << std::endl;
    // std::cout << arr1.has(4) << std::endl;

    // arr1.remove(0);
    // std::cout << arr1.str() << std::endl;
    
    // 关于内存释放，如果有obj["arr"] = arr; 那么arr的存储会赋给obj，只需要释放obj

    return 0;
}