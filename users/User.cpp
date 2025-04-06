//
// Created by Chiyoshi on 2025/4/6.
//

#include "User.h"

User::User() : id(0), name(""), password("") {}

User::User(int id, const std::string& name, const std::string& password) : id(id), name(name), password(password) {}

int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

std::string User::getPassword() const {
    return password;
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

json User::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["password"] = password;
    return j;
}

void User::fromJson(const json& j) {
    id = j.at("id").get<int>();
    name = j.at("name").get<std::string>();
    password = j.at("password").get<std::string>();
}
