#include "parser.h"

using namespace yazi::json;

Parser::Parser() : m_str(""), m_idx(0){}

void Parser::load(const string& str){
    m_str = str;
    m_idx = 0;
}

void Parser::skip_white_space(){
    while(m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\r' || m_str[m_idx] == '\t'){
        m_idx++;    // 简单忽略
    }
}

char Parser::get_next_token(){
    skip_white_space();
    return m_str[m_idx++];
}

Json Parser::parse(){
    // 组合递归
    char ch = get_next_token();
    switch (ch)
    {
    case 'n':
        // get_next_token那里，idx++导致位置后移了一些; 原代码这里每个都单独判断--，感觉可以优化
        m_idx--;
        return parse_null();
    case 't':
    case 'f':
        m_idx--;
        return parse_bool();  // true和false，字符串会以 “ 开头
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        m_idx--;
        return parse_number();
    case '"': return Json(parse_string());
    case '[': return parse_array();
    case '{': return parse_object();
    default:
        break;
    }
    throw new std::logic_error("unexpected char");
}

Json Parser::parse_null(){
    if(m_str.compare(m_idx, 4, "null") == 0){
        m_idx += 4;
        return Json();
    }
    throw new std::logic_error("parse null error");
}

Json Parser::parse_bool(){
    if(m_str.compare(m_idx, 4, "true") == 0){
        m_idx += 4;
        return Json(true);
    }else if(m_str.compare(m_idx, 5, "false") == 0){
        m_idx += 5;
        return Json(false);
    }
    throw new std::logic_error("parse bool error");
}

Json Parser::parse_number(){
    int pos = m_idx;
    if(m_str[m_idx] == '-'){
        m_idx++;
    }
    if(m_str[m_idx] < '0' || m_str[m_idx] > '9'){
        throw new std::logic_error("parse number error");
    }
    while(m_str[m_idx] >= '0' && m_str[m_idx] <= '9'){
        m_idx++;
    }
    if(m_str[m_idx] != '.'){
        return Json(std::atoi(m_str.c_str() + pos));
    }

    // 小数点
    m_idx++;
    if(m_str[m_idx] < '0' || m_str[m_idx] > '9'){
        throw new std::logic_error("parse number error");
    }
    while(m_str[m_idx] >= '0' && m_str[m_idx] <= '9'){
        m_idx++;
    }
    double f = std::atof(m_str.c_str() + pos);
    
    return Json(f);
}

string Parser::parse_string(){
    string out;
    while (true)
    {
        char ch = m_str[m_idx++];   // 这里需要保留空格
        if(ch == '"') break;
        if(ch == '\\'){
            ch = m_str[m_idx++];
            switch (ch)
            {
            case '\n':
                out += '\n';
                break;
            case '\t':
                out += '\t';
                break;
            case '\r':
                out += '\r';
                break;
            case '\b':
                out += '\b';
                break;
            case '\f':
                out += '\f';
                break;
            case '"':
                out += '\\\"';
                break;
            case '\\':
                out += '\\\\';
                break;
            case 'u':
                out += '\\u';
                for(int i = 0; i < 4; i++){
                    out += m_str[m_idx++];
                }
                break;
            default:
                break;
            }
        }else{
            out += ch;
        }
    }
    
    return out;
}

Json Parser::parse_array(){
    Json arr(Json::json_array);
    char ch = get_next_token();
    if(ch == ']') return arr;   // 空数组

    m_idx--;    //回退一步
    while(true){
        arr.append(parse());
        ch = get_next_token();
        if(ch == ']') break;
        if(ch != ','){
            throw new std::logic_error("parse array error");
        }
        m_idx++;
    }
    return arr;
}

Json Parser::parse_object(){
    Json obj(Json::json_object);
    char ch = get_next_token();
    if(ch == '}') return obj;   // 空对象

    m_idx--;
    while(true){
        ch = get_next_token();
        
        // 键值对
        if(ch != '"') throw new std::logic_error("parse object error");
        string key = parse_string();
        ch = get_next_token();

        if(ch != ':') throw new std::logic_error("parse object error");
        obj[key] = parse();
        ch = get_next_token();
        if(ch == '}') break;
        if(ch != ',') throw new std::logic_error("parse object error");

        m_idx++;
    }
    return obj;
}