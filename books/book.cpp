//
// Created by Chiyoshi on 2025/4/7.
//

#include "book.h"
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// 默认构造函数
book::book() : bookId(""), title(""), author(""), bookType(FICTION), 
               publisher(""), isbn(""), isAvailable(true) {}

// 带参数的构造函数 (含有所有参数的构造函数)
book::book(const std::string& id, const std::string& title, const std::string& author, 
           type bookType, const std::string& publisher, const std::string& isbn, 
           bool isAvailable) 
    : bookId(id), title(title), author(author), bookType(bookType), 
      publisher(publisher), isbn(isbn), isAvailable(isAvailable) {}

// Getter 方法
std::string book::getBookId() const {
    return bookId;
}

std::string book::getTitle() const {
    return title;
}

std::string book::getAuthor() const {
    return author;
}

book::type book::getBookType() const {
    return bookType;
}

std::string book::getPublisher() const {
    return publisher;
}

std::string book::getIsbn() const {
    return isbn;
}

bool book::getIsAvailable() const {
    return isAvailable;
}

// Setter 方法
void book::setBookId(const std::string& id) {
    this->bookId = id;
}

void book::setTitle(const std::string& title) {
    this->title = title;
}

void book::setAuthor(const std::string& author) {
    this->author = author;
}

void book::setBookType(type bookType) {
    this->bookType = bookType;
}

void book::setPublisher(const std::string& publisher) {
    this->publisher = publisher;
}

void book::setIsbn(const std::string& isbn) {
    this->isbn = isbn;
}

void book::setIsAvailable(bool available) {
    this->isAvailable = available;
}

// 将图书对象转换为JSON对象 (序列化)
json book::toJson() const {
    json j;
    j["bookId"] = bookId;
    j["title"] = title;
    j["author"] = author;
    j["publisher"] = publisher;
    j["isbn"] = isbn;
    j["isAvailable"] = isAvailable;
    
    // 将枚举转换为字符串，便于JSON中阅读
    switch (bookType) {
        case FICTION:
            j["bookType"] = "FICTION";
            break;
        case NON_FICTION:
            j["bookType"] = "NON_FICTION";
            break;
        case SCIENCE:
            j["bookType"] = "SCIENCE";
            break;
        case HISTORY:
            j["bookType"] = "HISTORY";
            break;
        case BIOGRAPHY:
            j["bookType"] = "BIOGRAPHY";
            break;
        case FANTASY:
            j["bookType"] = "FANTASY";
            break;
        case MYSTERY:
            j["bookType"] = "MYSTERY";
            break;
        case ROMANCE:
            j["bookType"] = "ROMANCE";
            break;
    }
    
    return j;
}

// 从JSON对象中加载图书信息 (反序列化)
void book::fromJson(const json& j) {
    bookId = j.at("bookId").get<std::string>();
    title = j.at("title").get<std::string>();
    author = j.at("author").get<std::string>();
    publisher = j.at("publisher").get<std::string>();
    isbn = j.at("isbn").get<std::string>();
    isAvailable = j.at("isAvailable").get<bool>();
    
    // 将字符串转回枚举类型
    std::string typeStr = j.at("bookType").get<std::string>();
    if (typeStr == "FICTION") {
        bookType = FICTION;
    } else if (typeStr == "NON_FICTION") {
        bookType = NON_FICTION;
    } else if (typeStr == "SCIENCE") {
        bookType = SCIENCE;
    } else if (typeStr == "HISTORY") {
        bookType = HISTORY;
    } else if (typeStr == "BIOGRAPHY") {
        bookType = BIOGRAPHY;
    } else if (typeStr == "FANTASY") {
        bookType = FANTASY;
    } else if (typeStr == "MYSTERY") {
        bookType = MYSTERY;
    } else if (typeStr == "ROMANCE") {
        bookType = ROMANCE;
    }
}

// 从文件中加载图书数据
std::vector<book> book::loadFromFile(const std::string& filename) {
    std::vector<book> books;
    std::ifstream file(filename);
    
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            
            if (j.is_array()) {
                // 遍历JSON数组中的每本书
                for (const auto& bookJson : j) {
                    book newBook;
                    newBook.fromJson(bookJson);
                    books.push_back(newBook);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error loading books from " << filename << ": " << e.what() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    
    return books;
}

// 加载所有图书
std::vector<book> book::loadAllBooks() {
    // 从默认路径加载图书
    return loadFromFile("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/book.json");
}

// 将图书数据保存到文件
bool book::saveToFile(const std::vector<book>& books, const std::string& filename) {
    try {
        json j = json::array();
        for (const auto& book : books) {
            j.push_back(book.toJson());
        }
        
        std::ofstream file(filename);
        if (file.is_open()) {
            file << j.dump(2); // 使用2空格缩进格式化输出
            file.close();
            return true;
        } else {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving books to " << filename << ": " << e.what() << std::endl;
        return false;
    }
}

// 添加新书
bool book::addBook(const book& newBook) {
    std::vector<book> allBooks = loadAllBooks();
    
    // 检查是否已存在相同ID或ISBN的图书
    for (const auto& b : allBooks) {
        if (b.bookId == newBook.bookId || b.isbn == newBook.isbn) {
            std::cerr << "Book with the same ID or ISBN already exists" << std::endl;
            return false;
        }
    }
    
    // 添加新书并保存
    allBooks.push_back(newBook);
    return saveToFile(allBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/book.json");
}

// 根据ID查找图书
book* book::findBookById(const std::string& id) {
    static book foundBook;
    std::vector<book> allBooks = loadAllBooks();
    
    for (const auto& b : allBooks) {
        if (b.bookId == id) {
            foundBook = b;
            return &foundBook;
        }
    }
    
    return nullptr; // 未找到图书
}

// 更新图书信息
bool book::updateBook(const book& updatedBook) {
    std::vector<book> allBooks = loadAllBooks();
    bool found = false;
    
    for (auto& b : allBooks) {
        if (b.bookId == updatedBook.bookId) {
            b = updatedBook;
            found = true;
            break;
        }
    }
    
    if (found) {
        return saveToFile(allBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/book.json");
    } else {
        std::cerr << "Book not found" << std::endl;
        return false;
    }
}
