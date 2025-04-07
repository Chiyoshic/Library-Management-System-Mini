//
// Created by Chiyoshi on 2025/4/7.
//

#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>


class book {
    std::string title;
    std::string author;
    enum type {

    };
    std::string publisher;
    std::string isbn;
    bool isAvailable = false;
};



#endif //BOOK_H
