//
// Created by Chiyoshi on 2025/4/6.
//

#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
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
    User(); // Default constructor
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
};

#endif //USER_H
