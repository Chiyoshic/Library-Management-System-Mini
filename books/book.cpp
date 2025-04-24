//
// Created by Chiyoshi on 2025/4/7.
//

#include "book.h"
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// 默认构造函数
book::book() : bookId(0), title(""), author(""), bookType(FICTION), 
               publisher(""), isbn(""), isAvailable(true) {}

// 带参数的构造函数 (含有所有参数的构造函数)
book::book(int id, const std::string& title, const std::string& author, 
           type bookType, const std::string& publisher, const std::string& isbn, 
           bool isAvailable) 
    : bookId(id), title(title), author(author), bookType(bookType), 
      publisher(publisher), isbn(isbn), isAvailable(isAvailable) {}

// Getter 方法
int book::getBookId() const {
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
void book::setBookId(int id) {
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
    // 处理bookId转换 - 判断是否为数字类型
    if (j.at("bookId").is_number()) {
        bookId = j.at("bookId").get<int>();
    } else {
        // 如果是字符串，尝试提取数字部分
        std::string idStr = j.at("bookId").get<std::string>();
        // 移除非数字字符，只保留数字部分
        std::string numericPart = "";
        for (char c : idStr) {
            if (std::isdigit(c)) {
                numericPart += c;
            }
        }
        // 转换为整数，如果为空则设置为0
        bookId = numericPart.empty() ? 0 : std::stoi(numericPart);
    }
    
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
    return loadFromFile("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/books.json");
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
    return saveToFile(allBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/books.json");
}

// 根据ID查找图书
book* book::findBookById(int id) {
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
        return saveToFile(allBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/books.json");
    } else {
        std::cerr << "Book not found" << std::endl;
        return false;
    }
}

// 删除图书
bool book::deleteBook(int bookId) {
    // 加载所有图书
    std::vector<book> allBooks = loadAllBooks();
    bool found = false;
    
    // 创建一个新的图书集合，排除要删除的图书
    std::vector<book> updatedBooks;
    for (const auto& b : allBooks) {
        if (b.bookId == bookId) {
            found = true; // 找到要删除的图书
        } else {
            updatedBooks.push_back(b); // 保留其他图书
        }
    }
    
    // 如果找到图书并删除，保存更新后的图书集合
    if (found) {
        return saveToFile(updatedBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/books.json");
    } else {
        std::cerr << "图书不存在，无法删除 ID: " << bookId << std::endl;
        return false;
    }
}

// 借阅图书
bool book::borrowBook(int bookId) {
    // 加载所有图书
    std::vector<book> allBooks = loadAllBooks();
    bool found = false;
    bool success = false;
    
    // 查找并更新图书状态
    for (auto& b : allBooks) {
        if (b.bookId == bookId) {
            found = true;
            if (b.isAvailable) {
                b.isAvailable = false; // 设置为已借出
                success = true;
            } else {
                std::cerr << "图书已被借出，无法再次借阅 ID: " << bookId << std::endl;
            }
            break;
        }
    }
    
    // 根据处理结果返回
    if (!found) {
        std::cerr << "图书不存在，无法借阅 ID: " << bookId << std::endl;
        return false;
    } else if (success) {
        // 借阅成功，保存更新后的图书信息
        return saveToFile(allBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/books.json");
    } else {
        return false; // 图书存在但已被借出
    }
}

// 归还图书
bool book::returnBook(int bookId) {
    // 加载所有图书
    std::vector<book> allBooks = loadAllBooks();
    bool found = false;
    bool success = false;
    
    // 查找并更新图书状态
    for (auto& b : allBooks) {
        if (b.bookId == bookId) {
            found = true;
            if (!b.isAvailable) {
                b.isAvailable = true; // 设置为可借阅
                success = true;
            } else {
                std::cerr << "图书已在库中，无需归还 ID: " << bookId << std::endl;
            }
            break;
        }
    }
    
    // 根据处理结果返回
    if (!found) {
        std::cerr << "图书不存在，无法归还 ID: " << bookId << std::endl;
        return false;
    } else if (success) {
        // 归还成功，保存更新后的图书信息
        return saveToFile(allBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/books.json");
    } else {
        return false; // 图书存在但已在库中
    }
}
