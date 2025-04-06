//
// Created by Chiyoshi on 2025/4/6.
//
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "users/User.h"
using namespace std;
using json = nlohmann::json;

int main() {
    User user(1, "John Doe", "password123");
    json j = user.toJson();
    cout << "User JSON: " << j.dump(4) << endl;
    return 0;
}
