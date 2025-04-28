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
void student_register_page();
void admin_dashboard_page(User* user);
void student_dashboard_page(User* user);
void manage_books_page(User* user);
void search_books_page(User* user);
void add_book_page(User* user);
void delete_book_page(User* user); // 添加删除图书页面的函数声明
#endif //INDEX_H
