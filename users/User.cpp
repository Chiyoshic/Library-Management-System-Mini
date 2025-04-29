//
// Created by Chiyoshi on 2025/4/6.
//

#include "User.h"

User::User() : id(0), name(""), password(""), userRole(STUDENT) {}

User::User(int id, const std::string& name, const std::string& password, Role role) 
    : id(id), name(name), password(password), userRole(role) {}

int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

std::string User::getPassword() const {
    return password;
}

User::Role User::getRole() const {
    return userRole;
}

void User::setId(int id) {
    this->id = id;
}

void User::setName(const std::string& name) {
    this->name = name;
}

void User::setPassword(const std::string& password) {
    this->password = password;
}

void User::setRole(Role role) {
    this->userRole = role;
}

json User::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["password"] = password;
    
    // 将枚举转换为字符串以便在JSON中更加可读
    if (userRole == ADMIN) {
        j["role"] = "ADMIN";
    } else {
        j["role"] = "STUDENT";
    }
    
    return j;
}

void User::fromJson(const json& j) {
    id = j.at("id").get<int>();
    name = j.at("name").get<std::string>();
    password = j.at("password").get<std::string>();
    
    // 将字符串转换回枚举类型
    std::string roleStr = j.at("role").get<std::string>();
    if (roleStr == "ADMIN") {
        userRole = ADMIN;
    } else {
        userRole = STUDENT;
    }
}

std::vector<User> User::loadFromFile(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename);
    
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            
            if (j.is_array()) {
                // 遍历JSON数组中的每个用户
                for (const auto& userJson : j) {
                    User user;
                    user.fromJson(userJson);
                    users.push_back(user);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "从" << filename << "加载用户时出错: " << e.what() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "无法打开文件: " << filename << std::endl;
    }
    
    return users;
}

std::vector<User> User::loadAllUsers() {
    std::vector<User> users;
    // 加载用户
    std::vector<User> regularUsers = loadFromFile("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/users/user.json");
    users.insert(users.end(), regularUsers.begin(), regularUsers.end());
    
    return users;
}

bool User::saveToFile(const std::vector<User>& users, const std::string& filename) {
    try {
        json j = json::array();
        for (const auto& user : users) {
            j.push_back(user.toJson());
        }
        
        std::ofstream file(filename);
        if (file.is_open()) {
            file << j.dump(2); // 使用2个空格缩进格式化输出
            file.close();
            return true;
        } else {
            std::cerr << "无法打开文件进行写入: " << filename << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "将用户保存到" << filename << "时出错: " << e.what() << std::endl;
        return false;
    }
}

User* User::authenticate(const std::string& username, const std::string& password) {
    static User authenticatedUser;
    std::vector<User> allUsers = loadAllUsers();
    
    for (const auto& user : allUsers) {
        if (user.getName() == username && user.getPassword() == password) {
            authenticatedUser = user;
            return &authenticatedUser;
        }
    }
    
    return nullptr; // 认证失败
}

bool User::registerUser(const std::string& username, const std::string& password, Role role) {
    // 检查用户名是否已存在
    std::vector<User> allUsers = loadAllUsers();
    for (const auto& user : allUsers) {
        if (user.getName() == username) {
            std::cerr << "用户名已存在" << std::endl;
            return false;
        }
    }
    
    // 生成新用户ID（当前最大ID + 1）
    int maxId = 0;
    for (const auto& user : allUsers) {
        if (user.getId() > maxId) {
            maxId = user.getId();
        }
    }
    
    // 创建新用户
    User newUser(maxId + 1, username, password, role);
    
    // 根据角色确定要更新的文件
    std::vector<User> usersList = loadFromFile("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/users/user.json");
    usersList.push_back(newUser);
    
    // 将更新后的用户列表保存回文件
    return saveToFile(usersList, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/users/user.json");
}

bool User::changePassword(int userId, const std::string& oldPassword, const std::string& newPassword) {
    // 加载所有用户
    std::vector<User> allUsers = loadAllUsers();
    bool userFound = false;
    
    // 查找要修改密码的用户
    for (auto& user : allUsers) {
        if (user.getId() == userId) {
            // 验证原密码
            if (user.getPassword() != oldPassword) {
                std::cerr << "原密码不正确" << std::endl;
                return false;
            }
            
            // 更新密码
            user.setPassword(newPassword);
            userFound = true;
            break;
        }
    }
    
    if (!userFound) {
        std::cerr << "未找到ID为" << userId << "的用户" << std::endl;
        return false;
    }
    
    // 保存更新后的用户列表
    return saveToFile(allUsers, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/users/user.json");
}

bool User::adminChangeUserPassword(int adminId, int userId, const std::string& newPassword) {
    // 加载所有用户
    std::vector<User> allUsers = loadAllUsers();
    bool adminFound = false;
    bool userFound = false;
    
    // 首先检查是否为管理员
    for (const auto& user : allUsers) {
        if (user.getId() == adminId) {
            if (user.getRole() != ADMIN) {
                std::cerr << "没有管理员权限" << std::endl;
                return false;
            }
            adminFound = true;
            break;
        }
    }
    
    if (!adminFound) {
        std::cerr << "未找到ID为" << adminId << "的管理员" << std::endl;
        return false;
    }
    
    // 查找目标用户并修改密码
    for (auto& user : allUsers) {
        if (user.getId() == userId) {
            // 更新密码
            user.setPassword(newPassword);
            userFound = true;
            break;
        }
    }
    
    if (!userFound) {
        std::cerr << "未找到ID为" << userId << "的用户" << std::endl;
        return false;
    }
    
    // 保存更新后的用户列表
    return saveToFile(allUsers, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/users/user.json");
}
