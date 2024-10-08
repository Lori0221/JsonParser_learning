#include "json.h"
#include "parser.h"
#include <sstream>
#include<iostream>
using namespace std;

using namespace yazi::json;

Json::Json() : m_type(json_null){}   // 默认空值

Json::Json(bool value) : m_type(json_bool){
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(json_int){
    m_value.m_int = value;
}

Json::Json(double value) : m_type(json_double){
    m_value.m_double = value;
}

// 指针，动态内存分配
Json::Json(const char* value) : m_type(json_string){
    m_value.m_string = new string(value);
}

Json::Json(const string & value) : m_type(json_string){
    m_value.m_string = new string(value);
}

Json::Json(Type type) : m_type(type){
    // 根据不同类型来构造
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        m_value.m_string = new string("");
        break;
    case json_array:
        m_value.m_array = new std::vector<Json>();
        break;
    case json_object:
        m_value.m_object = new std::map<string, Json>();
        break;
    default:
        break;
    }
}

Json::Json(const Json & other){
    copy(other);
}

Json::operator bool(){
    // 先判断类型是否正确，然后返回json里的值
    if(m_type != json_bool){
        throw new logic_error("type error, not bool value");
    }
    return m_value.m_bool;
}

Json::operator int(){
    if(m_type != json_int){
        throw new logic_error("type error, not int value");
    }
    return m_value.m_int;
}

Json::operator double(){
    if(m_type != json_double){
        throw new logic_error("type error, not double value");
    }
    return m_value.m_double;
}

Json::operator string(){
    if(m_type != json_string){
        throw new logic_error("type error, not string value");
    }
    return *(m_value.m_string); // 指针解应用
}

Json & Json::operator [](int index){
    // 类型先一致
    if(m_type != json_array){
        clear();
        m_type = json_array;
        m_value.m_array = new vector<Json>();
    }
    // 判断index有效性
    if(index < 0){
        throw new logic_error("array[] index < 0");
    }
    int size = (m_value.m_array)->size();
    if(index >= size){
        // 扩容
        for(int i = size; i <= index; i++){
            (m_value.m_array)->push_back(Json());
        }
    }
    return (m_value.m_array)->at(index);  // 返回所在索引位置
}

void Json::append(const Json &other){
    // 类型先一致
    if(m_type != json_array){
        clear();
        m_type = json_array;
        m_value.m_array = new vector<Json>();
    }
    (m_value.m_array)->push_back(other); // 调stl库的push_back

}

Json & Json::operator [](const char* key){
    string name(key);
    // 放到下面那个里去实现
    return (*(this))[name];
}

Json & Json::operator [](const string & key){
    if(m_type != json_object){
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<string, Json>();
    }
    return (*(m_value.m_object))[key];
}

void Json::operator = (const Json &other){
    clear();
    copy(other);
}

bool Json::operator == (const Json &other){
    if(m_type != other.m_type) return false;
    switch (m_type)
    {
    case json_null:
        return true;
    case json_bool:
        return m_value.m_bool == other.m_value.m_bool;
    case json_int:
        return m_value.m_int == other.m_value.m_int;
    case json_double:
        // 原作者这里直接判断浮点数是否相等，需要改进
        return m_value.m_double == other.m_value.m_double;
    case json_string:
        return *(m_value.m_string) == *(other.m_value.m_string);
    case json_array:
        // 为了效率，简单判定俩数组指针是否相等，不严谨
        return m_value.m_array == other.m_value.m_array;
    case json_object:
        // 跟数组那里一样不严谨
        return m_value.m_object == other.m_value.m_object;
    default:
        break;
    }
    
    return false;   // 其他
}

bool Json::operator != (const Json &other){
    return !((*this) == other);
}

void Json::copy(const Json &other){
    m_type = other.m_type;
    // 根据不同类型来构造
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = other.m_value.m_bool;
        break;
    case json_int:
        m_value.m_int = other.m_value.m_int;
        break;
    case json_double:
        m_value.m_double = other.m_value.m_double;
        break;
    case json_string:
        // 原项目这及后面都是浅拷贝，为了提升性能, 但是会导致内存泄漏
        m_value.m_string = other.m_value.m_string;  
        break;
    case json_array:
        m_value.m_array = other.m_value.m_array;
        break;
    case json_object:
        m_value.m_object = other.m_value.m_object;
        break;
    default:
        break;
    }
}

void Json::clear(){
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        // 原项目这及后面都是浅拷贝，为了提升性能, 但是会导致内存泄漏
        delete m_value.m_string;  
        break;
    case json_array:
        // 先释放数组里的元素
        for(auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++){
            it->clear();
        }
        delete m_value.m_array;
        break;
    case json_object:
        for(auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++){
            (it->second).clear();
        }
        delete m_value.m_object;
        break;
    default:
        break;
    }
    m_type = json_null;
}

string Json::str() const{
    stringstream ss;
    switch (m_type)
    {
    case json_null:
        ss << "null";
        break;
    case json_bool:
        if(m_value.m_bool) ss << "true";
        else ss << "false";
        break;
    case json_int:
        ss << m_value.m_int;
        break;
    case json_double:
        ss << m_value.m_double;
        break;
    case json_string:
        // 字符串前后都需要引号, and字符串需要解引用
        ss << '\"' << *(m_value.m_string) << '\"';
        break;
    case json_array:
        ss << '[';
        for(auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++){
            if(it != (m_value.m_array)->begin()){
                ss << ',';
            }
            ss << it->str();
        }
        ss << ']';
        break;
    case json_object:
        ss << '{';
        for(auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++){
            if(it != (m_value.m_object)->begin()){
                ss << ',';
            }
            ss << '\"' << it->first << "\":" << it->second.str();
        }
        ss << '}';
        break;
    default:
        break;
    }
    return ss.str();
}

bool Json::asBool() const{
    if(m_type != json_bool){
        throw new std::logic_error("type error, not bool value");
    }
    return m_value.m_bool;
}

int Json::asInt() const{
    if(m_type != json_int){
        throw new std::logic_error("type error, not int value");
    }
    return m_value.m_int;
}

double Json::asDouble() const{
    if(m_type != json_double){
        throw new std::logic_error("type error, not double value");
    }
    return m_value.m_double;
}

string Json::asString() const{
    if(m_type != json_string){
        throw new std::logic_error("type error, not string value");
    }
    return *(m_value.m_string);
}

bool Json::has(int index){
    if(m_type != json_array){
        return false;
    }
    int size = (m_value.m_array)->size();
    return (index >= 0 && index < size);
}

bool Json::has(const char* key){
    string name(key);
    return has(name);
}

bool Json::has(const string &key){
    if(m_type != json_object){
        return false;
    }
    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

void Json::remove(int index){
    if(m_type != json_array){
        return;     // 原作者这里啥都不干，可以改函数类型进行提示或者直接报错
    }
    int size = (m_value.m_array)->size();
    if(index < 0 || index >= size){
        return;     // 也是啥都不做，很呆，不如直接用刚刚写的has来判断
    }
    (m_value.m_array)->at(index).clear();   // 先把内存释放掉
    (m_value.m_array)->erase((m_value.m_array)->begin() + index);
}

void Json::remove(const char* key){
    string name(key);
    return remove(name);
}

void Json::remove(const string &key){
    if(m_type != json_object){
        return;
    }
    auto it = (m_value.m_object)->find(key); // 作者又忘了has
    if(it == (m_value.m_object)->end()){return;}
    (*(m_value.m_object))[key].clear();
    (m_value.m_object)->erase(key);
}

void Json::parse(const string &str){
    Parser p;
    p.load(str);
    *this = p.parse();
}

void Json::patch(const string &str){
    // std::cout << this->str() << std::endl;
    Json other;
    other.parse(str);
    if(!other.isArray()){
        throw std::runtime_error("Please input the patch array in string");
    }
    int size = (other.m_value.m_array)->size();
    for (int i = 0; i < size; i++) {
        auto operation = other.m_value.m_array->at(i);
        if(!operation.has("op") || !operation.has("path")){
            throw logic_error("need key-name \"op\" and \"path\"");
        }
        string op = operation["op"];
        string path = operation["path"];
    
        if (op == "add") {
            try{
                addOperation(path, operation["value"]);
            }catch (std::exception & e){
                throw std::logic_error("add operation error");
            }
        } else if (op == "remove") {
            try{
                removeOperation(path);
            }catch (std::exception & e){
                throw std::logic_error("remove operation error");
            }
        } else if (op == "replace") {
            try{
                replaceOperation(path, operation["value"]);
            }catch (std::exception & e){
                throw std::logic_error("replace operation error");
            }
        } else if (op == "move") {
            try{
                moveOperation(operation["from"], path);
            }catch (std::exception & e){
                throw std::logic_error("move operation error");
            }
        } else if (op == "copy") {
            try{
                copyOperation(operation["from"], path);
            }catch (std::exception & e){
                throw std::logic_error("copy operation error");
            }
        } else if (op == "test") {
            try{
                testOperation(operation["from"], path);
            }catch (std::exception & e){
                throw std::logic_error("test operation error");
            }
        }else {
            throw std::runtime_error("Unknown operation: " + op);
        }
    }
    std::cout << this->str() << std::endl;
}

Json* Json::getPos(string path){
    stringstream ss(path);
    vector<string> pa;
    string token;
    while(getline(ss, token, '/')){
        if(token != ""){
            pa.push_back(token);
        }
    }
    // 下降去找key
    Json* current = &(*this);
    for (const auto& p : pa) {
        current = &(*current)[p];  // 逐级遍历路径
    }
    return current;
}

template <typename T>
void Json::addOperation(string path, T value){
    auto pos = getPos(path);
    Json val = value;
    *pos = val;
}

void Json::removeOperation(string path){
    auto pos = getPos(path);
    if((*pos).isNull()){
        throw logic_error("path didn't exist");
    }
    pos->clear();
}


template <typename T>
void Json::replaceOperation(string path, T value){
    addOperation(path, value);
}

void Json::moveOperation(string from, string path){
    auto source = getPos(from);
    auto target = getPos(path);
    *target = *source;
    source->clear();
}

void Json::copyOperation(string from, string path){
    auto source = getPos(from);
    auto target = getPos(path);
    *target = *source;
}

template <typename T>
void Json::testOperation(string path, T value){
    auto pos = getPos(path);
    // 比较
    Json val = value;
    if(val != *pos){
        throw logic_error("not equal");
    }
}
