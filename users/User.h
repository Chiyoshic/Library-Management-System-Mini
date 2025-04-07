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
private:
    int id;
    std::string name;
    std::string password;
    enum role {
        admin,
        teacher,
        student
    };

public:
    User(); // Default constructor
    User(int id, const std::string& name, const std::string& password);

    int getId() const;
    std::string getName() const;
    std::string getPassword() const;

    void setId(int id);
    void setName(const std::string& name);
    void setPassword(const std::string& password);

    json toJson() const;
    void fromJson(const json& j);
};

#endif //USER_H
