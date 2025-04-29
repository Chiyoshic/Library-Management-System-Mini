//
// Created by Chiyoshi on 2025/4/6.
//

#include "record.h"
#include <iomanip>
#include <sstream>
#include "nlohmann/json.hpp"

// 默认构造函数
record::record() : borrowerID(0), bookID(0), borrowTime(0), returnTime(0) {}

// 借书记录构造函数
record::record(int borrowerID, int bookID, time_t borrowTime) 
    : borrowerID(borrowerID), bookID(bookID), borrowTime(borrowTime), returnTime(0) {}

// Getter方法
int record::getBorrowerID() const {
    return borrowerID;
}

int record::getBookID() const {
    return bookID;
}

time_t record::getBorrowTime() const {
    return borrowTime;
}

time_t record::getReturnTime() const {
    return returnTime;
}

// Setter方法
void record::setBorrowerID(int id) {
    borrowerID = id;
}

void record::setBookID(int id) {
    bookID = id;
}

void record::setBorrowTime(time_t time) {
    borrowTime = time;
}

void record::setReturnTime(time_t time) {
    returnTime = time;
}

// 判断是否已归还
bool record::isReturned() const {
    return returnTime != 0;
}

// 设置归还时间（用于归还图书）
void record::returnBook() {
    returnTime = time(nullptr);
}

// 检查借阅是否逾期（借阅期为15天）
bool record::isOverdue() const {
    // 借阅期限（15天，转换为秒）
    const time_t BORROW_PERIOD = 15 * 24 * 60 * 60; 
    
    if (isReturned()) {
        // 已归还，检查是否逾期归还
        return (returnTime - borrowTime) > BORROW_PERIOD;
    } else {
        // 未归还，检查当前是否已逾期
        time_t currentTime = time(nullptr);
        return (currentTime - borrowTime) > BORROW_PERIOD;
    }
}

// 获取逾期天数
int record::getOverdueDays() const {
    // 借阅期限（15天，转换为秒）
    const time_t BORROW_PERIOD = 15 * 24 * 60 * 60;
    
    // 计算实际借阅时间（秒）
    time_t actualBorrowTime;
    if (isReturned()) {
        // 已归还，使用实际归还时间
        actualBorrowTime = returnTime - borrowTime;
    } else {
        // 未归还，使用当前时间计算
        actualBorrowTime = time(nullptr) - borrowTime;
    }
    
    // 如果未逾期，返回0
    if (actualBorrowTime <= BORROW_PERIOD) {
        return 0;
    }
    
    // 计算逾期天数（向上取整）
    time_t overdueSeconds = actualBorrowTime - BORROW_PERIOD;
    int overdueDays = static_cast<int>(overdueSeconds / (24 * 60 * 60));
    // 如果有不足一天的部分，算作一天
    if (overdueSeconds % (24 * 60 * 60) > 0) {
        overdueDays++;
    }
    
    return overdueDays;
}

// 获取所有逾期记录
std::vector<record> record::getOverdueRecords(const std::vector<record>& records) {
    std::vector<record> overdueRecords;
    
    for (const auto& rec : records) {
        if (rec.isOverdue()) {
            overdueRecords.push_back(rec);
        }
    }
    
    return overdueRecords;
}

// 检查借阅是否即将逾期（n天内）
bool record::willBeOverdueSoon(int days) const {
    // 如果已经归还或已经逾期，则不是即将逾期
    if (isReturned() || isOverdue()) {
        return false;
    }
    
    // 借阅期限（15天，转换为秒）
    const time_t BORROW_PERIOD = 15 * 24 * 60 * 60;
    
    // 当前时间
    time_t currentTime = time(nullptr);
    
    // 借阅已经过的时间
    time_t elapsedTime = currentTime - borrowTime;
    
    // 剩余时间（秒）
    time_t remainingTime = BORROW_PERIOD - elapsedTime;
    
    // 将预警天数转换为秒
    time_t warningPeriod = days * 24 * 60 * 60;
    
    // 如果剩余时间小于预警时间，但大于0（未逾期），则是即将逾期
    return (remainingTime <= warningPeriod) && (remainingTime > 0);
}

// 获取即将逾期的记录
std::vector<record> record::getSoonOverdueRecords(const std::vector<record>& records, int days) {
    std::vector<record> soonOverdueRecords;
    
    for (const auto& rec : records) {
        if (rec.willBeOverdueSoon(days)) {
            soonOverdueRecords.push_back(rec);
        }
    }
    
    return soonOverdueRecords;
}

// JSON序列化
json record::toJson() const {
    json j;
    j["borrowerID"] = borrowerID;
    j["bookID"] = bookID;
    j["borrowTime"] = borrowTime;
    j["returnTime"] = returnTime;
    return j;
}

// JSON反序列化
record record::fromJson(const json& j) {
    return record(
        j["borrowerID"].get<int>(),
        j["bookID"].get<int>(),
        j["borrowTime"].get<time_t>()
    );
}

// 从JSON文件读取所有记录
std::vector<record> record::readFromFile(const std::string& filename) {
    std::vector<record> records;
    std::ifstream file(filename);
    
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            
            for (const auto& item : j) {
                records.push_back(fromJson(item));
            }
        } catch (const std::exception& e) {
            std::cerr << "读取记录文件时出错: " << e.what() << std::endl;
        }
        
        file.close();
    } else {
        std::cerr << "无法打开记录文件: " << filename << std::endl;
    }
    
    return records;
}

// 将所有记录写入JSON文件
void record::writeToFile(const std::vector<record>& records, const std::string& filename) {
    json j = json::array();
    
    for (const auto& rec : records) {
        j.push_back(rec.toJson());
    }
    
    std::ofstream file(filename);
    if (file.is_open()) {
        file << std::setw(4) << j << std::endl;
        file.close();
    } else {
        std::cerr << "无法打开记录文件进行写入: " << filename << std::endl;
    }
}

// 添加借阅记录并保存到文件
void record::addBorrowRecord(int borrowerID, int bookID, const std::string& filename) {
    // 读取现有记录
    std::vector<record> records = readFromFile(filename);
    
    // 检查图书是否已被借出
    if (isBookBorrowed(bookID, records)) {
        std::cerr << "该图书已被借出，无法再次借阅" << std::endl;
        return;
    }
    
    // 创建新记录
    record newRecord(borrowerID, bookID, time(nullptr));
    records.push_back(newRecord);
    
    // 保存记录
    writeToFile(records, filename);
    
    std::cout << "借阅记录已添加" << std::endl;
}

// 添加归还记录并保存到文件
bool record::addReturnRecord(int borrowerID, int bookID, const std::string& filename) {
    // 读取现有记录
    std::vector<record> records = readFromFile(filename);
    bool found = false;
    
    // 查找未归还的匹配记录
    for (auto& rec : records) {
        if (rec.getBorrowerID() == borrowerID && 
            rec.getBookID() == bookID && 
            !rec.isReturned()) {
            // 设置归还时间
            rec.returnBook();
            found = true;
            break;
        }
    }
    
    if (!found) {
        std::cerr << "未找到该用户的此图书未归还记录" << std::endl;
        return false;
    }
    
    // 保存更新后的记录
    writeToFile(records, filename);
    
    std::cout << "图书已成功归还" << std::endl;
    return true;
}

// 获取用户的所有借阅记录
std::vector<record> record::getUserRecords(int userID, const std::vector<record>& records) {
    std::vector<record> userRecords;
    
    for (const auto& rec : records) {
        if (rec.getBorrowerID() == userID) {
            userRecords.push_back(rec);
        }
    }
    
    return userRecords;
}

// 获取图书的所有借阅记录
std::vector<record> record::getBookRecords(int bookID, const std::vector<record>& records) {
    std::vector<record> bookRecords;
    
    for (const auto& rec : records) {
        if (rec.getBookID() == bookID) {
            bookRecords.push_back(rec);
        }
    }
    
    return bookRecords;
}

// 获取当前未归还的记录
std::vector<record> record::getUnreturnedRecords(const std::vector<record>& records) {
    std::vector<record> unreturnedRecords;
    
    for (const auto& rec : records) {
        if (!rec.isReturned()) {
            unreturnedRecords.push_back(rec);
        }
    }
    
    return unreturnedRecords;
}

// 检查图书是否已被借出
bool record::isBookBorrowed(int bookID, const std::vector<record>& records) {
    for (const auto& rec : records) {
        if (rec.getBookID() == bookID && !rec.isReturned()) {
            return true;
        }
    }
    
    return false;
}

// 检查用户是否已借此书
bool record::hasUserBorrowedBook(int userID, int bookID, const std::vector<record>& records) {
    for (const auto& rec : records) {
        if (rec.getBorrowerID() == userID && 
            rec.getBookID() == bookID && 
            !rec.isReturned()) {
            return true;
        }
    }
    
    return false;
}

// 检查用户是否已归还所有借阅的图书
bool record::hasUserReturnedAllBooks(int userID, const std::vector<record>& records) {
    // 获取用户的借阅记录
    std::vector<record> userRecords = getUserRecords(userID, records);
    
    // 检查是否有未归还的图书
    for (const auto& rec : userRecords) {
        if (!rec.isReturned()) {
            return false; // 发现未归还的图书
        }
    }
    
    // 如果没有未归还的图书，但用户有借阅记录，则表示已全部归还
    return !userRecords.empty();
}

// 格式化显示时间
std::string record::formatTime(time_t time) {
    if (time == 0) {
        return "未归还";
    }
    
    struct tm* timeinfo = localtime(&time);
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// 打印记录信息
void record::printRecord() const {
    std::cout << "借阅者ID: " << borrowerID << std::endl;
    std::cout << "图书ID: " << bookID << std::endl;
    std::cout << "借阅时间: " << formatTime(borrowTime) << std::endl;
    std::cout << "归还时间: " << formatTime(returnTime) << std::endl;
    std::cout << "状态: " << (isReturned() ? "已归还" : "未归还") << std::endl;
}
