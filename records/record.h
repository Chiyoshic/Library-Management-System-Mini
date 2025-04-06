//
// Created by Chiyoshi on 2025/4/6.
//

#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <ctime>

class record {
private:
    int borrowerID;
    int bookID;
    time_t borrowTime;
    time_t returnTime;

public:
    record(int borrowerID, int bookID, time_t borrowTime, time_t returnTime) :
        borrowerID(borrowerID),
        bookID(bookID),
        borrowTime(borrowTime),
        returnTime(returnTime) {}
};

#endif //RECORD_H