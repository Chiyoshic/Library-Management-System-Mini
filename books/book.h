//
// Created by Chiyoshi on 2025/4/7.
//

#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class book {
public:
    static int nextId;
    std::string bookId;
    std::string title;
    std::string author;
    enum type {
        FICTION,
        NON_FICTION,
        SCIENCE,
        HISTORY,
        BIOGRAPHY,
        FANTASY,
        MYSTERY,
        ROMANCE
    } bookType;
    std::string publisher;
    std::string isbn;
    bool isAvailable = false;
    
    // 构造函数
    book();
    static std::string generateBookId();
    book(const std::string& id, const std::string& title, const std::string& author, 
         type bookType, const std::string& publisher, const std::string& isbn, 
         bool isAvailable);
    
    // Getter 方法
    std::string getBookId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    type getBookType() const;
    std::string getPublisher() const;
    std::string getIsbn() const;
    bool getIsAvailable() const;
    
    // Setter 方法
    void setBookId(const std::string& id);
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setBookType(type bookType);
    void setPublisher(const std::string& publisher);
    void setIsbn(const std::string& isbn);
    void setIsAvailable(bool available);
    
    // JSON序列化和反序列化
    json toJson() const;
    void fromJson(const json& j);
    
    // 文件操作
    static std::vector<book> loadFromFile(const std::string& filename);
    static std::vector<book> loadAllBooks();
    static bool saveToFile(const std::vector<book>& books, const std::string& filename);
    
    // 图书管理方法
    static bool addBook(const book& newBook);
    static book* findBookById(const std::string& id);
    static bool updateBook(const book& updatedBook);
};

#endif //BOOK_H
