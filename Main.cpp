//
// Created by Chiyoshi on 2025/4/6.
//
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "users/User.h"
#include "index/Index.h"
#include <thread>
#include <chrono>
using namespace std;
using json = nlohmann::json;

int main() {
    printIndex();
    this_thread::sleep_for(chrono::seconds(3));
    system("clear");
    printMenu();
}
