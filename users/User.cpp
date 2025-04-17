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
