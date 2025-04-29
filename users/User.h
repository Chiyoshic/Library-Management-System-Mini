//
// Created by Chiyoshi on 2025/4/6.
//

#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class User {
public:
    enum Role {
        ADMIN,
        STUDENT
    };

private:
    int id;
    std::string name;
    std::string password;
    Role userRole;

public:
    User(); // 默认构造函数
    User(int id, const std::string& name, const std::string& password, Role role = STUDENT);

    int getId() const;
    std::string getName() const;
    std::string getPassword() const;
    Role getRole() const;

    void setId(int id);
    void setName(const std::string& name);
    void setPassword(const std::string& password);
    void setRole(Role role);

    json toJson() const;
    void fromJson(const json& j);
    
    // 用户管理的静态方法
    static std::vector<User> loadFromFile(const std::string& filename);
    static std::vector<User> loadAllUsers();
    static bool saveToFile(const std::vector<User>& users, const std::string& filename);
    static User* authenticate(const std::string& username, const std::string& password);
    static bool registerUser(const std::string& username, const std::string& password, Role role = STUDENT);
    
    // 新增用户密码修改方法
    static bool changePassword(int userId, const std::string& oldPassword, const std::string& newPassword);
    // 新增管理员修改学生密码方法
    static bool adminChangeUserPassword(int adminId, int userId, const std::string& newPassword);
};

#endif //USER_H
