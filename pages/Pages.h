//
// Created by Chiyoshi on 2025/4/9.
//

#ifndef INDEX_H
#define INDEX_H
#include <iostream>
#include <User.h>

void printIndex();
void printMenu();
void admin_login_page();
void student_login_page();
void admin_dashboard_page(User* user);
void student_dashboard_page(User* user);
void manage_books_page(User* user);
#endif //INDEX_H
