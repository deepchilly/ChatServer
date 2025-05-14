#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

// User表的ORM(对象关系映射)类
class User{
public:
    User(int id = -1, string name = "", string pwd = "", string state = "offline")
    : id(id), name(name), password(pwd), state(state)
    {}

    // 获取私有成员
    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setPwd(string pwd) { this->password = pwd; }
    void setState(string state) { this->state = state; }

    // 修改私有成员
    int getId() { return this->id; }
    string getName() { return this->name; }
    string getPwd() { return this->password; }
    string getState() { return this->state; }

protected:
    int id;
    string name;
    string password;
    string state;
};

#endif