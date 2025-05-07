//
// Created by Chiyoshi on 2025/4/6.
//

#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class record {
private:
    int borrowerID;
    int bookID;
    time_t borrowTime;
    time_t returnTime; // 如果是0，表示尚未归还

public:
    // 默认构造函数
    record();
    
    // 借书记录构造函数（未归还）
    record(int borrowerID, int bookID, time_t borrowTime);

    
    // Getter方法
    int getBorrowerID() const;
    int getBookID() const;
    time_t getBorrowTime() const;
    time_t getReturnTime() const;
    
    // Setter方法
    void setBorrowerID(int id);
    void setBookID(int id);
    void setBorrowTime(time_t time);
    void setReturnTime(time_t time);
    
    // 判断是否已归还
    bool isReturned() const;
    
    // 判断图书是否已归还（检查所有记录）
    static bool isBookReturned(int bookID, const std::vector<record>& records);
    
    // 设置归还时间（用于归还图书）
    void returnBook();
    
    // 检查借阅是否逾期（借阅期为15天）
    bool isOverdue() const;
    
    // 检查借阅是否即将逾期（n天内）
    bool willBeOverdueSoon(int days = 3) const;
    
    // 获取即将逾期的记录
    static std::vector<record> getSoonOverdueRecords(const std::vector<record>& records, int days = 3);
    
    // 获取逾期天数
    int getOverdueDays() const;
    
    // 获取所有逾期记录
    static std::vector<record> getOverdueRecords(const std::vector<record>& records);
    
    // JSON序列化
    json toJson() const;
    
    // JSON反序列化
    static record fromJson(const json& j);
    
    // 从JSON文件读取所有记录
    static std::vector<record> readFromFile(const std::string& filename);
    
    // 将所有记录写入JSON文件
    static void writeToFile(const std::vector<record>& records, const std::string& filename);
    
    // 添加借阅记录并保存到文件
    static void addBorrowRecord(int borrowerID, int bookID, const std::string& filename);
    
    // 添加归还记录并保存到文件
    static bool addReturnRecord(int borrowerID, int bookID, const std::string& filename);
    
    // 获取用户的所有借阅记录
    static std::vector<record> getUserRecords(int userID, const std::vector<record>& records);
    
    // 从文件中获取用户的所有借阅记录
    static std::vector<record> getUserRecordsFromFile(int userID, const std::string& filename);
    
    // 获取图书的所有借阅记录
    static std::vector<record> getBookRecords(int bookID, const std::vector<record>& records);
    
    // 获取当前未归还的记录
    static std::vector<record> getUnreturnedRecords(const std::vector<record>& records);
    
    // 检查图书是否已被借出
    static bool isBookBorrowed(int bookID, const std::vector<record>& records);
    
    // 检查用户是否已借此书
    static bool hasUserBorrowedBook(int userID, int bookID, const std::vector<record>& records);
    
    // 检查用户是否已归还所有借阅的图书
    static bool hasUserReturnedAllBooks(int userID, const std::vector<record>& records);
    
    // 格式化显示时间
    static std::string formatTime(time_t time);
    
    // 打印记录信息
    void printRecord() const;

    // 统计书籍借阅次数（从记录集合）
    static std::map<int, int> getBookBorrowCounts(const std::vector<record>& records);
    
    // 统计书籍借阅次数（从文件）
    static std::map<int, int> getBookBorrowCountsFromFile(const std::string& filename);

    // 根据日期查询该日期之后到期的书籍
    static std::vector<record> getBooksWithDueAfterDate(time_t date, const std::vector<record>& records);
    
    // 从文件中查询该日期之后到期的书籍
    static std::vector<record> getBooksWithDueAfterDateFromFile(time_t date, const std::string& filename);
    
    // 获取所有逾期未归还的书籍
    static std::vector<record> getOverdueUnreturnedRecords(const std::vector<record>& records);
    
    // 从文件中获取所有逾期未归还的书籍
    static std::vector<record> getOverdueUnreturnedRecordsFromFile(const std::string& filename);
    
    // 获取记录的应还日期
    time_t getDueDate() const;
};

#endif //RECORD_H