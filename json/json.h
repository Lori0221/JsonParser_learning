#pragma once

#include<string>
#include<vector>
#include<map>
using namespace std;

// 防止和其他库名字冲突，使用命名空间来隔离
namespace yazi{
namespace json{

class Json{
public:
    enum Type{
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    // 构造函数
    Json();
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char* value); // C格式的字符串
    Json(const string & value); // C++格式的字符串
    Json(Type type);
    Json(const Json & other);

    // 运算符重载
    operator bool();
    operator int();
    operator double();
    operator string();

    // 重载中括号
    Json & operator [](int index);
    void append(const Json &other);

    Json & operator [](const char* key);
    Json & operator [](const string & key);

    // 重载赋值运算符
    void operator = (const Json &other);
    bool operator == (const Json &other);
    bool operator != (const Json &other);

    // 字符串形式展示json内容
    string str() const;

    void copy(const Json &other);
    void clear();

    // 定义迭代器，方便对数组类型进行遍历
    typedef std::vector<Json>::iterator iterator;
    iterator begin(){
        return (m_value.m_array)->begin();
    }
    iterator end(){
        return (m_value.m_array)->end();
    }


    // 类型判断
    bool isNull() const {
        return m_type == json_null;
    }

    bool isBool() const {
        return m_type == json_bool;
    }

    bool isInt() const {
        return m_type == json_int;
    }

    bool isDouble() const {
        return m_type == json_double;
    }

    bool isString() const {
        return m_type == json_string;
    }

    bool isArray() const {
        return m_type == json_array;
    }

    bool isObject() const {
        return m_type == json_object;
    }

    // 显性转换
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    string asString() const;

    // 判断数组里有没有某个索引，对象里有没有某个key
    bool has(int index);
    bool has(const char* key);
    bool has(const string &key);

    // 删除
    void remove(int index);
    void remove(const char* key);
    void remove(const string &key);

    // 解析
    void parse(const string &str);

    // 修补
    void patch(const string &str);

    Json* getPos(string path);

    template <typename T>
    void addOperation(string path, T value);

    void removeOperation(string path);

    template <typename T>
    void replaceOperation(string path, T value);

    void moveOperation(string from, string path);

    void copyOperation(string from, string path);

    template <typename T>
    void testOperation(string path, T value);

private:
    // 联合体占内存更小一点, double 8byte
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        std::string *m_string;
        std::vector<Json> *m_array;
        std::map<string, Json> *m_object;
    };

    Type m_type;
    Value m_value;
    
};

}}