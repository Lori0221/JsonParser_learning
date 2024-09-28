#pragma once

#include "json.h"
#include <string>
using namespace std;

namespace yazi{
namespace json{

class Parser{
public:
    Parser();
    void load(const string& str);

    Json parse();

private:
    string m_str;
    int m_idx;

    // 解析助手，忽略回车换行等
    void skip_white_space();
    char get_next_token();

    // 分类解析
    Json parse_null();
    Json parse_bool();
    Json parse_number();
    string parse_string();
    Json parse_array();
    Json parse_object();


};

}}