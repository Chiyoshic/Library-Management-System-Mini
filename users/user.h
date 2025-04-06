//
// Created by Chiyoshi on 2025/4/6.
//

#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

class user {
    private:
        int id;
        std::string name;
        std::string password;

    public:
        user(int id, std::string name, std::string password) : id(id), name(name), password(password) {}

        int getId() const {
            return id;
        }

        std::string getName() const {
            return name;
        }

        std::string getPassword() const {
            return password;
        }

        void setName(std::string newName) {
            name = newName;
        }

        void setPassword(std::string newPassword) {
            password = newPassword;
        }

};



#endif //USER_H
