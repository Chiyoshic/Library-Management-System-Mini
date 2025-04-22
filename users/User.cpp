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
    
    // Convert enum to string for better readability in JSON
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
    
    // Convert string back to enum
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
                // Iterate through each user in the JSON array （遍历JSON数组中的每个用户）
                for (const auto& userJson : j) {
                    User user;
                    user.fromJson(userJson);
                    users.push_back(user);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error loading users from " << filename << ": " << e.what() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    
    return users;
}

std::vector<User> User::loadAllUsers() {
    std::vector<User> users;
    // Load users
    std::vector<User> regularUsers = loadFromFile("users/user.json");
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
            file << j.dump(2); // Pretty print with 2-space indentation
            file.close();
            return true;
        } else {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving users to " << filename << ": " << e.what() << std::endl;
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
    
    return nullptr; // Authentication failed
}

bool User::registerUser(const std::string& username, const std::string& password, Role role) {
    // Check if username already exists
    std::vector<User> allUsers = loadAllUsers();
    for (const auto& user : allUsers) {
        if (user.getName() == username) {
            std::cerr << "Username already exists" << std::endl;
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
    std::string filename = (role == ADMIN) ? "users/admin.json" : "users/user.json";
    std::vector<User> usersList = loadFromFile(filename);
    usersList.push_back(newUser);
    
    // 将更新后的用户列表保存回文件
    return saveToFile(usersList, filename);
}
