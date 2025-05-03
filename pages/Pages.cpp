//
// Created by Chiyoshi on 2025/4/9.
//
#include <iostream>
#include "Pages.h"
#include <User.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <book.h>
#include <vector>
#include <algorithm>
#include <record.h> // Add include for the record class

class book;
using namespace std;

// 清屏函数
void clearScreen() {
    system("clear");  // 在Unix/Linux/macOS系统上
    // 如果在Windows系统上，可以使用system("cls");
}

void printIndex() {
    clearScreen();
    std::cout << " __          __  _                            _        " << std::endl;
    std::cout << " \\ \\        / / | |                          | |       " << std::endl;
    std::cout << "  \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___  | |_ ___  " << std::endl;
    std::cout << "   \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\ " << std::endl;
    std::cout << "    \\  /\\  /  __/ | (_| (_) | | | | | |  __/ | || (_) |" << std::endl;
    std::cout << "  _  \\/  \\/_\\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/ " << std::endl;
    std::cout << " | |    (_) |                                          " << std::endl;
    std::cout << " | |     _| |__  _ __ __ _ _ __ _   _                  " << std::endl;
    std::cout << " | |    | | '_ \\| '__/ _` | '__| | | |                 " << std::endl;
    std::cout << " | |____| | |_) | | | (_| | |  | |_| |                 " << std::endl;
    std::cout << " |______|_|_.__/|_|  \\__,_|_|   \\__, |                 " << std::endl;
    std::cout << "                                 __/ |                 " << std::endl;
    std::cout << "                                |___/                  " << std::endl;
}

void admin_dashboard_page(User* user);
void student_dashboard_page(User* user);
void manage_books_page(User* user);
void student_register_page();
void add_book_page(User* user);
void view_stats_page(User* user);  // Forward declaration
void my_borrows_page(User* user);  // Forward declaration
void borrow_return_page(User* user); // Forward declaration

void add_book_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 图书信息变量
    std::string book_id;  // 修改：添加图书ID输入变量
    std::string title;
    std::string author;
    std::string publisher;
    std::string isbn;
    int bookType = 0;
    std::string error_message;
    bool show_error = false;
    bool show_success = false;
    
    // 创建输入框
    auto id_input = Input(&book_id, "请输入图书ID") | size(WIDTH, EQUAL, 30);  // 修改：添加图书ID输入框
    auto title_input = Input(&title, "请输入图书标题") | size(WIDTH, EQUAL, 30);
    auto author_input = Input(&author, "请输入作者") | size(WIDTH, EQUAL, 30);
    auto publisher_input = Input(&publisher, "请输入出版社") | size(WIDTH, EQUAL, 30);
    auto isbn_input = Input(&isbn, "请输入ISBN") | size(WIDTH, EQUAL, 30);
    
    // 图书类型选择
    std::vector<std::string> type_entries = {
        "小说", "非小说", "科学", "历史", 
        "传记", "奇幻", "悬疑", "爱情"
    };
    int type_selected = 0;
    auto type_menu = Radiobox(&type_entries, &type_selected);
    
    // 创建保存按钮
    bool save_clicked = false;
    auto save_button = Button("保存", [&] { save_clicked = true; });
    
    // 创建返回按钮
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        manage_books_page(user);
    });
    
    // 组合所有组件
    auto container = Container::Vertical({
        id_input,  // 修改：添加图书ID输入框到界面
        title_input,
        author_input,
        publisher_input,
        isbn_input,
        type_menu,
        Container::Horizontal({
            save_button,
            back_button
        })
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("录入图书") | bold | hcenter,
            separator(),
            vbox({
                hbox(text(" 图书ID: "), id_input->Render()) | hcenter,  // 修改：添加图书ID输入框到渲染
                hbox(text(" 标题: "), title_input->Render()) | hcenter,
                hbox(text(" 作者: "), author_input->Render()) | hcenter,
                hbox(text(" 出版社: "), publisher_input->Render()) | hcenter,
                hbox(text(" ISBN: "), isbn_input->Render()) | hcenter,
                hbox(text(" 类型: "), type_menu->Render()) | hcenter,
                show_error ? text(error_message) | color(Color::Red) | hcenter : text(""),
                show_success ? text("图书录入成功！") | color(Color::Green) | hcenter : text(""),
                separator(),
                hbox(save_button->Render(), back_button->Render()) | hcenter,
            }) | border,
        });
    });

    // 处理保存按钮点击事件
    auto save_handler = CatchEvent(renderer, [&](Event event) {
        if (save_clicked) {
            save_clicked = false;
            
            // 基本输入验证
            if (book_id.empty() || title.empty() || author.empty() || 
                publisher.empty() || isbn.empty()) {
                show_error = true;
                show_success = false;
                error_message = "所有字段都不能为空";
                return true;
            }
            
            // 创建图书对象
            book newBook;
            newBook.setBookId(book_id);  // 修改：直接设置用户输入的ID
            newBook.setTitle(title);
            newBook.setAuthor(author);
            newBook.setPublisher(publisher);
            newBook.setIsbn(isbn);
            
            // 根据选择的类型设置书籍类型
            book::type bookTypeEnum;
            switch(type_selected) {
                case 0: bookTypeEnum = book::FICTION; break;
                case 1: bookTypeEnum = book::NON_FICTION; break;
                case 2: bookTypeEnum = book::SCIENCE; break;
                case 3: bookTypeEnum = book::HISTORY; break;
                case 4: bookTypeEnum = book::BIOGRAPHY; break;
                case 5: bookTypeEnum = book::FANTASY; break;
                case 6: bookTypeEnum = book::MYSTERY; break;
                case 7: bookTypeEnum = book::ROMANCE; break;
                default: bookTypeEnum = book::FICTION; break;
            }
            newBook.setBookType(bookTypeEnum);
            newBook.setIsAvailable(true);
            
            // 调用book::addBook方法保存图书
            bool success = book::addBook(newBook);
            if (success) {
                show_error = false;
                show_success = true;
                
                // 清空输入框，方便继续添加
                book_id.clear();
                title.clear();
                author.clear();
                publisher.clear();
                isbn.clear();
                type_selected = 0;
                
                // 显示保存成功，并在短暂延迟后自动返回
                std::thread([&screen]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                    screen.ExitLoopClosure()();
                }).detach();
                
                return true;
            } else {
                show_error = true;
                show_success = false;
                error_message = "保存失败，图书ID可能已存在";
                return true;
            }
        }
        
        return false;
    });

    screen.Loop(save_handler);
    
    // 如果是因为保存成功而退出的，则返回管理页面
    if (show_success) {
        clearScreen();
        manage_books_page(user);
    }
}

void admin_login_page() {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 账户和密码变量
    std::string username;
    std::string password;
    std::string error_message;
    bool show_error = false;
    
    // 创建输入框，密码使用*号显示
    InputOption password_option;
    password_option.password = true;
    
    auto username_input = Input(&username, "请输入管理员账号");
    auto password_input = Input(&password, "请输入密码", password_option);
    
    // 创建登录按钮
    bool login_clicked = false;
    auto login_button = Button("登录", [&] { login_clicked = true; });
    
    // 创建返回按钮
    auto back_button = Button("返回", screen.ExitLoopClosure());
    
    // 组合所有组件
    auto container = Container::Vertical({
        username_input,
        password_input,
        Container::Horizontal({
            login_button,
            back_button
        })
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("管理员登录") | bold | hcenter,
            separator(),
            vbox({
                hbox(text(" 账号: "), username_input->Render()) | hcenter,
                hbox(text(" 密码: "), password_input->Render()) | hcenter,
                show_error ? text(error_message) | color(Color::Red) | hcenter : text(""),
                separator(),
                hbox(login_button->Render(), back_button->Render()) | hcenter,
            }) | border,
        });
    });

    // 处理登录按钮点击事件
    auto login_handler = CatchEvent(renderer, [&](Event event) {
        if (login_clicked) {
            login_clicked = false;

            User* user = User::authenticate(username, password);
            if (user && user->getRole() == User::ADMIN) {
                show_error = false;
                screen.ExitLoopClosure()();
                clearScreen();
                admin_dashboard_page(user);
                return true;
            } else {
                show_error = true;
                error_message = "账号或密码错误，或无管理员权限";
                return true;
            }
        }
        return false;
    });

    screen.Loop(login_handler);
}

void student_login_page() {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 账户和密码变量
    std::string username;
    std::string password;
    std::string error_message;
    bool show_error = false;
    
    // 创建输入框，密码使用*号显示
    InputOption password_option;
    password_option.password = true;
    
    auto username_input = Input(&username, "请输入学生学号");
    auto password_input = Input(&password, "请输入密码", password_option);
    
    // 创建登录按钮
    bool login_clicked = false;
    auto login_button = Button("登录", [&] { login_clicked = true; });
    
    // 创建注册按钮
    auto register_button = Button("注册", [&] {
        screen.ExitLoopClosure()();
        student_register_page();
    });
    
    // 创建返回按钮
    auto back_button = Button("返回", screen.ExitLoopClosure());
    
    // 组合所有组件
    auto container = Container::Vertical({
        username_input,
        password_input,
        Container::Horizontal({
            login_button,
            register_button,
            back_button
        })
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("学生登录") | bold | hcenter,
            separator(),
            vbox({
                hbox(text(" 学号: "), username_input->Render()) | hcenter,
                hbox(text(" 密码: "), password_input->Render()) | hcenter,
                show_error ? text(error_message) | color(Color::Red) | hcenter : text(""),
                separator(),
                hbox(login_button->Render(), register_button->Render(), back_button->Render()) | hcenter,
            }) | border,
        });
    });

    // 处理登录按钮点击事件
    auto login_handler = CatchEvent(renderer, [&](Event event) {
        if (login_clicked) {
            login_clicked = false;
            
            // 实现真正的登录验证逻辑
            User* user = User::authenticate(username, password);
            if (user && user->getRole() == User::STUDENT) {
                show_error = false;
                screen.ExitLoopClosure()();
                clearScreen();
                student_dashboard_page(user);
                return true;
            } else {
                show_error = true;
                error_message = "账号或密码错误";
                return true;
            }
        }
        return false;
    });

    screen.Loop(login_handler);
}

void admin_dashboard_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();
    
    // 创建功能按钮
    auto manage_books_button = Button("图书管理", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        manage_books_page(user);
    });
    
    auto manage_users_button = Button("用户管理", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        manage_users_page(user);
    });
    
    auto view_stats_button = Button("统计信息", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        view_stats_page(user);
    });
    
    auto logout_button = Button("退出登录", screen.ExitLoopClosure());
    
    auto container = Container::Vertical({
        manage_books_button,
        manage_users_button,
        view_stats_button,
        logout_button
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("管理员控制面板") | bold | hcenter,
            text("欢迎, " + user->getName() + "!") | hcenter,
            separator(),
            vbox({
                manage_books_button->Render(),
                manage_users_button->Render(),
                view_stats_button->Render(),
                separator(),
                logout_button->Render(),
            }) | border,
        });
    });
    
    screen.Loop(renderer);
}

void manage_books_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();
    
    // 创建图书管理功能按钮
    auto search_books_button = Button("查询图书", [&] {
        // 调用图书搜索页面
        screen.ExitLoopClosure()();
        clearScreen();
        search_books_page(user);
    });
    
    auto add_books_button = Button("录入图书", [&] {
        // 调用图书录入页面
        screen.ExitLoopClosure()();
        clearScreen();
        add_book_page(user);
    });
    
    auto delete_books_button = Button("删除图书", [&] {
        // 调用图书删除页面
        screen.ExitLoopClosure()();
        clearScreen();
        delete_book_page(user);
    });
    
    auto back_button = Button("返回上一页", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        admin_dashboard_page(user);
    });
    
    auto container = Container::Vertical({
        search_books_button,
        add_books_button,
        delete_books_button,
        back_button
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("图书管理") | bold | hcenter,
            text("管理员: " + user->getName()) | hcenter,
            separator(),
            vbox({
                search_books_button->Render(),
                add_books_button->Render(),
                delete_books_button->Render(),
                separator(),
                back_button->Render(),
            }) | border,
        });
    });
    
    screen.Loop(renderer);
}

void student_dashboard_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();
    
    // 创建功能按钮
    auto search_books_button = Button("搜索图书", [&] {
        // 调用图书搜索页面
        screen.ExitLoopClosure()();
        clearScreen();
        search_books_page(user);
    });
    
    auto my_borrows_button = Button("我的借阅", [&] {
        // 调用我的借阅页面
        screen.ExitLoopClosure()();
        clearScreen();
        my_borrows_page(user);
    });
    
    auto borrow_return_button = Button("借书还书", [&] {
        // 调用借书还书页面
        screen.ExitLoopClosure()();
        clearScreen();
        borrow_return_page(user);
    });
    
    auto logout_button = Button("退出登录", screen.ExitLoopClosure());
    
    auto container = Container::Vertical({
        search_books_button,
        my_borrows_button,
        borrow_return_button,
        logout_button
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("学生控制面板") | bold | hcenter,
            text("欢迎, " + user->getName() + "!") | hcenter,
            separator(),
            vbox({
                search_books_button->Render(),
                my_borrows_button->Render(),
                borrow_return_button->Render(),
                separator(),
                logout_button->Render(),
            }) | border,
        });
    });
    
    screen.Loop(renderer);
}

void my_borrows_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 获取用户的借阅记录
    std::vector<record> user_records = record::getUserRecordsFromFile(
        user->getId(), 
        "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json"
    );

    // 加载所有图书信息，用于显示图书详情
    std::vector<book> all_books = book::loadAllBooks();
    std::map<std::string, book> books_map;
    
    // 创建图书ID到图书对象的映射
    for (const auto& b : all_books) {
        books_map[b.getBookId()] = b;
    }

    // 借阅期限（15天，转换为秒）
    const time_t BORROW_PERIOD = 15 * 24 * 60 * 60;
    
    // 为每条记录准备显示项
    std::vector<std::string> borrow_entries;
    
    for (const auto& rec : user_records) {
        // 获取图书信息
        std::string bookId = std::to_string(rec.getBookID());
        std::string bookTitle = "未知图书";
        std::string bookAuthor = "";
        
        // 查找图书详情
        if (books_map.find(bookId) != books_map.end()) {
            bookTitle = books_map[bookId].getTitle();
            bookAuthor = books_map[bookId].getAuthor();
        }
        
        // 计算应归还日期
        time_t due_date = rec.getBorrowTime() + BORROW_PERIOD;
        
        // 格式化显示时间
        std::string borrowTimeStr = record::formatTime(rec.getBorrowTime());
        std::string dueTimeStr = record::formatTime(due_date);
        std::string returnTimeStr = rec.isReturned() ? record::formatTime(rec.getReturnTime()) : "未归还";
        
        // 计算状态
        std::string status;
        if (rec.isReturned()) {
            status = "已归还";
        } else if (rec.isOverdue()) {
            status = "逾期未还";
        } else {
            status = "借阅中";
        }
        
        // 逾期天数
        std::string overdueDays = "";
        if (rec.getOverdueDays() > 0) {
            overdueDays = "逾期" + std::to_string(rec.getOverdueDays()) + "天";
        }
        
        // 固定宽度以防止溢出
        const int id_width = 8;
        const int title_width = 20;
        const int author_width = 15;
        const int borrow_time_width = 20;
        const int due_time_width = 20;
        const int return_time_width = 20;
        const int status_width = 10;
        
        // 截断过长的字符串并确保填充宽度不为负数
        std::string id = bookId.substr(0, id_width);
        std::string title = bookTitle.substr(0, title_width);
        std::string author = bookAuthor.substr(0, author_width);
        
        // 添加空格填充至固定宽度
        int id_padding = std::max(0, id_width - (int)id.length());
        int title_padding = std::max(0, title_width - (int)title.length());
        int author_padding = std::max(0, author_width - (int)author.length());
        int borrow_time_padding = std::max(0, borrow_time_width - (int)borrowTimeStr.length());
        int due_time_padding = std::max(0, due_time_width - (int)dueTimeStr.length());
        int return_time_padding = std::max(0, return_time_width - (int)returnTimeStr.length());
        int status_padding = std::max(0, status_width - (int)status.length());
        
        // 创建格式化的条目字符串
        std::string entry = 
            id + std::string(id_padding, ' ') + " | " +
            title + std::string(title_padding, ' ') + " | " +
            author + std::string(author_padding, ' ') + " | " +
            borrowTimeStr + std::string(borrow_time_padding, ' ') + " | " +
            dueTimeStr + std::string(due_time_padding, ' ') + " | " +
            returnTimeStr + std::string(return_time_padding, ' ') + " | " +
            status + std::string(status_padding, ' ') + (overdueDays.empty() ? "" : " | " + overdueDays);
            
        borrow_entries.push_back(entry);
    }

    // 创建菜单组件，用于显示和选择借阅记录
    int borrow_menu_selected = 0;
    auto borrow_menu = Menu(&borrow_entries, &borrow_menu_selected);
    
    // 创建自定义菜单渲染器，以便对逾期的记录使用红色显示
    auto borrow_menu_renderer = Renderer(borrow_menu, [&] {
        Elements elements;
        int index = 0;
        for (const auto& rec : user_records) {
            Element e = text(borrow_entries[index]);
            
            // 如果未归还且逾期，显示为红色
            if (!rec.isReturned() && rec.isOverdue()) {
                e = e | color(Color::Red);
            } else if (rec.isReturned() && rec.isOverdue()) {
                // 已归还但曾经逾期，显示为黄色
                e = e | color(Color::Yellow);
            }
            
            // 如果是被选中的项目，添加高亮
            if (index == borrow_menu_selected) {
                e = e | bgcolor(Color::Blue);
            }
            
            elements.push_back(e);
            index++;
        }
        return vbox(elements) | frame;
    });
    
    // 创建返回按钮
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        student_dashboard_page(user);
    });
    
    // 主容器
    auto container = Container::Vertical({
        borrow_menu,
        back_button
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        // 表头
        Element header = hbox({
            text("图书ID") | size(WIDTH, EQUAL, 8) | bold,
            text(" | "),
            text("图书标题") | size(WIDTH, EQUAL, 20) | bold,
            text(" | "),
            text("作者") | size(WIDTH, EQUAL, 15) | bold,
            text(" | "),
            text("借阅时间") | size(WIDTH, EQUAL, 20) | bold,
            text(" | "),
            text("应归还时间") | size(WIDTH, EQUAL, 20) | bold,
            text(" | "),
            text("实际归还时间") | size(WIDTH, EQUAL, 20) | bold,
            text(" | "),
            text("状态") | size(WIDTH, EQUAL, 10) | bold,
            text(" | "),
            text("逾期信息") | size(WIDTH, EQUAL, 10) | bold
        });
        
        // 创建说明文本
        Elements legendElements;
        legendElements.push_back(text("图例：") | bold);
        legendElements.push_back(text("普通文本 - 正常借阅") | color(Color::White));
        legendElements.push_back(text("红色文本 - 当前逾期未还") | color(Color::Red));
        legendElements.push_back(text("黄色文本 - 已归还但曾经逾期") | color(Color::Yellow));
        
        return vbox({
            text("我的借阅记录") | bold | hcenter,
            text("用户: " + user->getName()) | hcenter,
            vbox(legendElements) | hcenter,
            separator(),
            
            user_records.empty() 
                ? vbox({
                    text("你还没有借阅记录") | hcenter,
                    separator()
                  })
                : vbox({
                    header,
                    separator(),
                    borrow_menu_renderer->Render() | yframe | vscroll_indicator | size(HEIGHT, LESS_THAN, 15)
                  }) | border,
            
            separator(),
            back_button->Render() | hcenter
        });
    });
    
    // 启动交互循环
    screen.Loop(renderer);
}

void borrow_return_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 变量定义
    std::string book_id;                // 图书ID输入
    std::string operation_message;      // 操作结果消息
    bool show_error = false;            // 是否显示错误
    bool show_success = false;          // 是否显示成功信息
    bool show_book_info = false;        // 是否显示图书信息
    book current_book;                  // 当前查询/操作的图书
    record current_record;              // 当前查询/操作的借阅记录
    bool is_borrowed = false;           // 图书是否已被借出
    int current_borrower_id = 0;        // 当前借阅者ID
    time_t borrow_time = 0;             // 借阅时间
    time_t due_time = 0;                // 应归还时间
    int overdue_days = 0;               // 逾期天数
    
    // 创建输入框
    auto id_input = Input(&book_id, "输入图书ID") | size(WIDTH, EQUAL, 30);
    
    // 创建按钮
    bool borrow_clicked = false;
    auto borrow_button = Button("借书", [&] { borrow_clicked = true; });
    
    bool return_clicked = false;
    auto return_button = Button("还书", [&] { return_clicked = true; });
    
    bool query_clicked = false;
    auto query_button = Button("查询", [&] { query_clicked = true; });
    
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        student_dashboard_page(user);
    });
    
    // 主容器
    auto container = Container::Vertical({
        id_input,
        Container::Horizontal({
            borrow_button,
            return_button,
            query_button,
            back_button
        })
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        Elements elements;
        
        // 页面标题和用户信息
        elements.push_back(text("借书还书") | bold | hcenter);
        elements.push_back(text("用户: " + user->getName()) | hcenter);
        elements.push_back(separator());
        
        // 输入区域
        elements.push_back(vbox({
            hbox(text("图书ID: "), id_input->Render()) | hcenter,
            separator(),
            hbox(
                borrow_button->Render(),
                text(" "),
                return_button->Render(),
                text(" "),
                query_button->Render(),
                text(" "),
                back_button->Render()
            ) | hcenter,
        }) | border);
        
        // 操作结果消息
        if (show_error) {
            elements.push_back(text(operation_message) | color(Color::Red) | hcenter);
        } else if (show_success) {
            elements.push_back(text(operation_message) | color(Color::Green) | hcenter);
        }
        
        // 显示图书信息（如果有）
        if (show_book_info) {
            // 将图书类型转换为中文字符串
            std::string bookTypeStr = "未知";
            switch(current_book.getBookType()) {
                case book::FICTION: bookTypeStr = "小说"; break;
                case book::NON_FICTION: bookTypeStr = "非小说"; break;
                case book::SCIENCE: bookTypeStr = "科学"; break;
                case book::HISTORY: bookTypeStr = "历史"; break;
                case book::BIOGRAPHY: bookTypeStr = "传记"; break;
                case book::FANTASY: bookTypeStr = "奇幻"; break;
                case book::MYSTERY: bookTypeStr = "悬疑"; break;
                case book::ROMANCE: bookTypeStr = "爱情"; break;
            }
            
            Elements book_info_elements;
            
            // 图书基本信息
            book_info_elements.push_back(text("图书信息") | bold | hcenter);
            book_info_elements.push_back(text("ID: " + current_book.getBookId()));
            book_info_elements.push_back(text("标题: " + current_book.getTitle()));
            book_info_elements.push_back(text("作者: " + current_book.getAuthor()));
            book_info_elements.push_back(text("出版社: " + current_book.getPublisher()));
            book_info_elements.push_back(text("ISBN: " + current_book.getIsbn()));
            book_info_elements.push_back(text("类型: " + bookTypeStr));
            
            // 借阅状态
            book_info_elements.push_back(separator());
            book_info_elements.push_back(text("借阅状态") | bold | hcenter);
            
            if (is_borrowed) {
                // 显示借阅信息
                book_info_elements.push_back(text("状态: 已借出") | color(Color::Red));
                book_info_elements.push_back(text("借阅者ID: " + std::to_string(current_borrower_id)));
                book_info_elements.push_back(text("借阅日期: " + record::formatTime(borrow_time)));
                book_info_elements.push_back(text("应还日期: " + record::formatTime(due_time)));
                
                if (overdue_days > 0) {
                    // 如果已经逾期，显示逾期天数
                    book_info_elements.push_back(text("逾期: " + std::to_string(overdue_days) + "天") | color(Color::Red));
                }
            } else {
                // 显示可借阅状态
                book_info_elements.push_back(text("状态: 可借阅") | color(Color::Green));
            }
            
            // 添加图书信息到主元素列表
            elements.push_back(vbox(book_info_elements) | border);
        }
        
        return vbox(elements);
    });
    
    // 事件处理函数
    auto event_handler = CatchEvent(renderer, [&](Event event) {
        // 借书按钮点击
        if (borrow_clicked) {
            borrow_clicked = false;
            
            // 重置显示状态
            show_error = false;
            show_success = false;
            show_book_info = false;
            
            // 检查输入是否为空
            if (book_id.empty()) {
                show_error = true;
                operation_message = "请输入图书ID";
                return true;
            }
            
            // 每次操作时重新从文件查找图书，避免使用缓存
            book* bookToBorrow = book::findBookById(book_id);
            if (!bookToBorrow) {
                show_error = true;
                operation_message = "未找到ID为 " + book_id + " 的图书";
                return true;
            }
            
            // 保存当前图书信息用于显示
            current_book = *bookToBorrow;
            show_book_info = true;
            
            // 每次操作时重新读取所有记录
            std::vector<record> all_records = record::readFromFile(
                "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
            
            // 检查图书是否可借阅
            try {
                int bookIDInt = std::stoi(book_id);
                if (record::isBookBorrowed(bookIDInt, all_records)) {
                    show_error = true;
                    operation_message = "该图书已被借出";
                    
                    // 查找当前借阅者信息
                    for (const auto& rec : all_records) {
                        if (rec.getBookID() == bookIDInt && !rec.isReturned()) {
                            current_borrower_id = rec.getBorrowerID();
                            borrow_time = rec.getBorrowTime();
                            due_time = borrow_time + (15 * 24 * 60 * 60); // 15天转为秒
                            is_borrowed = true;
                            
                            // 计算逾期天数
                            overdue_days = rec.getOverdueDays();
                            break;
                        }
                    }
                    return true;
                }
            } catch (const std::exception& e) {
                show_error = true;
                operation_message = "图书ID格式错误";
                return true;
            }
            
            // 执行借书操作
            try {
                int bookIDInt = std::stoi(book_id);
                int userIDInt = user->getId();
                
                // 添加借阅记录
                record::addBorrowRecord(
                    userIDInt, 
                    bookIDInt, 
                    "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json"
                );
                
                // 更新显示状态
                show_success = true;
                operation_message = "借书成功";
                
                // 重新加载图书信息确保数据是最新的
                book* updatedBook = book::findBookById(book_id);
                if (updatedBook) {
                    current_book = *updatedBook;
                }
                
                // 重新加载记录确认借阅状态
                all_records = record::readFromFile(
                    "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
                
                is_borrowed = true;
                current_borrower_id = userIDInt;
                borrow_time = time(nullptr);
                due_time = borrow_time + (15 * 24 * 60 * 60); // 15天转为秒
                
            } catch (const std::exception& e) {
                show_error = true;
                operation_message = "借书失败: " + std::string(e.what());
            }
            
            return true;
        }
        
        // 还书按钮点击
        if (return_clicked) {
            return_clicked = false;
            
            // 重置显示状态
            show_error = false;
            show_success = false;
            show_book_info = false;
            
            // 检查输入是否为空
            if (book_id.empty()) {
                show_error = true;
                operation_message = "请输入图书ID";
                return true;
            }
            
            // 每次操作时重新从文件查找图书
            book* bookToReturn = book::findBookById(book_id);
            if (!bookToReturn) {
                show_error = true;
                operation_message = "未找到ID为 " + book_id + " 的图书";
                return true;
            }
            
            // 保存当前图书信息用于显示
            current_book = *bookToReturn;
            show_book_info = true;
            
            // 检查用户是否真的借了这本书
            try {
                int bookIDInt = std::stoi(book_id);
                int userIDInt = user->getId();
                
                // 每次操作时重新读取所有记录
                std::vector<record> all_records = record::readFromFile(
                    "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
                
                // 检查用户是否借了这本书
                if (!record::hasUserBorrowedBook(userIDInt, bookIDInt, all_records)) {
                    show_error = true;
                    operation_message = "您没有借阅这本书，无法归还";
                    
                    // 查找当前真实借阅状态
                    if (!record::isBookReturned(bookIDInt, all_records)) {
                        // 书确实被借出，但不是这个用户借的
                        for (const auto& rec : all_records) {
                            if (rec.getBookID() == bookIDInt && !rec.isReturned()) {
                                is_borrowed = true;
                                current_borrower_id = rec.getBorrowerID();
                                borrow_time = rec.getBorrowTime();
                                due_time = borrow_time + (15 * 24 * 60 * 60); // 15天转为秒
                                overdue_days = rec.getOverdueDays();
                                break;
                            }
                        }
                    } else {
                        // 图书实际上已归还
                        is_borrowed = false;
                    }
                    
                    return true;
                }
                
                // 执行还书操作
                bool success = record::addReturnRecord(
                    userIDInt, 
                    bookIDInt, 
                    "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json"
                );
                
                if (success) {
                    // 重新加载图书信息确保数据是最新的
                    book* updatedBook = book::findBookById(book_id);
                    if (updatedBook) {
                        current_book = *updatedBook;
                    }
                    
                    // 更新显示状态
                    show_success = true;
                    operation_message = "还书成功";
                    is_borrowed = false;
                } else {
                    show_error = true;
                    operation_message = "还书失败，可能是系统错误";
                }
                
            } catch (const std::exception& e) {
                show_error = true;
                operation_message = "还书失败: " + std::string(e.what());
            }
            
            return true;
        }
        
        // 查询按钮点击 - 不修改任何文件
        if (query_clicked) {
            query_clicked = false;
            
            // 重置显示状态
            show_error = false;
            show_success = false;
            show_book_info = false;
            
            // 检查输入是否为空
            if (book_id.empty()) {
                show_error = true;
                operation_message = "请输入图书ID";
                return true;
            }
            
            // 每次操作时重新从文件查找图书
            book* bookToQuery = book::findBookById(book_id);
            if (!bookToQuery) {
                show_error = true;
                operation_message = "未找到ID为 " + book_id + " 的图书";
                return true;
            }
            
            // 保存当前图书信息用于显示
            current_book = *bookToQuery;
            show_book_info = true;
            
            try {
                int bookIDInt = std::stoi(book_id);
                
                // 每次操作时重新读取所有记录
                std::vector<record> all_records = record::readFromFile(
                    "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
                
                // 查询图书真实借阅状态（不进行任何修改）
                if (record::isBookBorrowed(bookIDInt, all_records)) {
                    // 查找借阅者信息
                    for (const auto& rec : all_records) {
                        if (rec.getBookID() == bookIDInt && !rec.isReturned()) {
                            is_borrowed = true;
                            current_borrower_id = rec.getBorrowerID();
                            borrow_time = rec.getBorrowTime();
                            due_time = borrow_time + (15 * 24 * 60 * 60); // 15天转为秒
                            overdue_days = rec.getOverdueDays();
                            break;
                        }
                    }
                } else {
                    is_borrowed = false;
                }
                
                // 更新显示状态
                show_success = true;
                operation_message = "查询成功";
                
            } catch (const std::exception& e) {
                show_error = true;
                operation_message = "查询失败: " + std::string(e.what());
            }
            
            return true;
        }
        
        return false;
    });
    
    // 启动交互循环
    screen.Loop(event_handler);
}

void student_register_page() {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 创建账户和密码变量
    std::string username;
    std::string password;
    std::string error_message;
    bool show_error = false;
    bool show_success = false;
    
    // 创建输入框，密码使用*号显示
    InputOption password_option;
    password_option.password = true;
    
    auto username_input = Input(&username, "请输入学号/用户名");
    auto password_input = Input(&password, "请输入密码", password_option);
    
    // 创建确认按钮
    bool register_clicked = false;
    auto confirm_button = Button("确认注册", [&] { register_clicked = true; });
    
    // 创建返回按钮
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        printMenu();
    });
    
    // 组合所有组件
    auto container = Container::Vertical({
        username_input,
        password_input,
        Container::Horizontal({
            confirm_button,
            back_button
        })
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("注册一个学生账号") | bold | hcenter,
            separator(),
            vbox({
                hbox(text(" 学号/用户名: "), username_input->Render()) | hcenter,
                hbox(text(" 密码: "), password_input->Render()) | hcenter,
                show_error ? text(error_message) | color(Color::Red) | hcenter : text(""),
                show_success ? text("注册成功！") | color(Color::Green) | hcenter : text(""),
                separator(),
                hbox(confirm_button->Render(), back_button->Render()) | hcenter,
            }) | border,
        });
    });

    // 处理注册按钮点击事件
    auto register_handler = CatchEvent(renderer, [&](Event event) {
        if (register_clicked) {
            register_clicked = false;
            
            // 基本输入验证
            if (username.empty() || password.empty()) {
                show_error = true;
                show_success = false;
                error_message = "用户名和密码不能为空";
                return true;
            }
            
            // 调用User::registerUser方法创建新用户
            bool success = User::registerUser(username, password, User::STUDENT);
            if (success) {
                show_error = false;
                show_success = true;
                
                // 显示注册成功，并在短暂延迟后自动返回主菜单
                std::thread([&screen]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                    screen.ExitLoopClosure()();
                }).detach();
                
                return true;
            } else {
                show_error = true;
                show_success = false;
                error_message = "注册失败，用户名可能已存在";
                return true;
            }
        }
        
        return false;
    });

    // 注册成功后会调用ExitLoopClosure退出当前页面
    screen.Loop(register_handler);
    
    // 如果是因为注册成功而退出的，则返回主菜单
    if (show_success) {
        clearScreen();
        printMenu();
    }
}

void printMenu() {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 创建登录按钮
    auto admin_button = Button("我是管理员", admin_login_page);
    auto student_button = Button("我是学生", student_login_page);
    
    auto roleForm = Container::Horizontal({
        admin_button,
        student_button
    });

    // 主渲染函数
    auto renderer = Renderer(roleForm, [&] {
        return vbox({
            text("欢迎使用图书管理系统 V0.1") | bold | hcenter,
            vbox({
                text("选择你的角色") | bold | hcenter,
                separator(),
                hbox(admin_button->Render(), student_button->Render()) | hcenter,
                separator()
            }) | border
        });
    });

    // 启动交互循环
    screen.Loop(renderer);

}

// 图书搜索页面
void search_books_page(User* user) {
    clearScreen();
    using namespace ftxui;
    
    auto screen = ScreenInteractive::TerminalOutput();

    // 加载所有图书
    std::vector<book> all_books = book::loadAllBooks();
    std::vector<book> search_results;
    
    // 加载所有借阅记录，用于查询当前借阅者
    std::vector<record> all_records = record::readFromFile("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
    
    // 搜索条件
    std::string search_id;
    std::string search_title;
    bool has_searched = false;
    
    // 当前选中的图书索引
    int selected_book_index = 0;
    std::vector<std::string> books_entries;
    
    // 创建图书项目的显示项
    auto createBookEntry = [&all_records, user](const book& b) -> std::string {
        // 将书籍类型转换为字符串
        std::string typeStr;
        switch(b.getBookType()) {
            case book::FICTION: typeStr = "小说"; break;
            case book::NON_FICTION: typeStr = "非小说"; break;
            case book::SCIENCE: typeStr = "科学"; break;
            case book::HISTORY: typeStr = "历史"; break;
            case book::BIOGRAPHY: typeStr = "传记"; break;
            case book::FANTASY: typeStr = "奇幻"; break;
            case book::MYSTERY: typeStr = "悬疑"; break;
            case book::ROMANCE: typeStr = "爱情"; break;
            default: typeStr = "未知"; break;
        }
        
        // 查找当前借阅者ID
        std::string borrowerId = "-";
        if (!b.getIsAvailable()) {  // 只有当图书不可借阅时才显示借阅者ID
            int bookIdInt;
            try {
                bookIdInt = std::stoi(b.getBookId());
                
                // 使用新方法检查图书是否已归还
                bool isReturned = record::isBookReturned(bookIdInt, all_records);
                
                // 如果图书未归还，找出借阅者
                if (!isReturned) {
                    for (const auto& rec : all_records) {
                        if (rec.getBookID() == bookIdInt && !rec.isReturned()) {
                            borrowerId = std::to_string(rec.getBorrowerID());
                            break;
                        }
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error converting book ID: " << b.getBookId() << std::endl;
            }
        }
        
        // 固定宽度以防止溢出
        const int id_width = 8;
        const int title_width = 20;
        const int author_width = 15;
        const int type_width = 12;
        const int publisher_width = 15;
        const int isbn_width = 15;
        const int borrower_width = 8;
        
        // 截断过长的字符串并确保填充宽度不为负数
        std::string id = b.getBookId().substr(0, id_width);
        std::string title = b.getTitle().substr(0, title_width);
        std::string author = b.getAuthor().substr(0, author_width);
        typeStr = typeStr.substr(0, type_width);
        std::string publisher = b.getPublisher().substr(0, publisher_width);
        std::string isbn = b.getIsbn().substr(0, isbn_width);
        
        // 添加空格填充至固定宽度
        int id_padding = std::max(0, id_width - (int)id.length());
        int title_padding = std::max(0, title_width - (int)title.length());
        int author_padding = std::max(0, author_width - (int)author.length());
        int type_padding = std::max(0, type_width - (int)typeStr.length());
        int publisher_padding = std::max(0, publisher_width - (int)publisher.length());
        int isbn_padding = std::max(0, isbn_width - (int)isbn.length());
        int borrower_padding = std::max(0, borrower_width - (int)borrowerId.length());
        
        // 根据用户角色创建不同的条目字符串
        std::string entry;
        
        if (user->getRole() == User::ADMIN) {
            // 管理员可以看到借阅者信息
            entry = 
                id + std::string(id_padding, ' ') + " | " +
                title + std::string(title_padding, ' ') + " | " +
                author + std::string(author_padding, ' ') + " | " +
                typeStr + std::string(type_padding, ' ') + " | " +
                publisher + std::string(publisher_padding, ' ') + " | " +
                isbn + std::string(isbn_padding, ' ') + " | " +
                (b.getIsAvailable() ? "可借阅" : "已借出") + " | " +
                borrowerId + std::string(borrower_padding, ' ');
        } else {
            // 学生用户看不到借阅者信息
            entry = 
                id + std::string(id_padding, ' ') + " | " +
                title + std::string(title_padding, ' ') + " | " +
                author + std::string(author_padding, ' ') + " | " +
                typeStr + std::string(type_padding, ' ') + " | " +
                publisher + std::string(publisher_padding, ' ') + " | " +
                isbn + std::string(isbn_padding, ' ') + " | " +
                (b.getIsAvailable() ? "可借阅" : "已借出");
        }
        
        return entry;
    };
    
    // 为所有图书生成条目
    for (const auto& b : all_books) {
        books_entries.push_back(createBookEntry(b));
    }
    
    // 创建菜单组件，用于显示和选择图书
    int books_menu_selected = 0;
    
    // 创建图书菜单组件 - 不使用自定义样式选项
    auto books_menu = Menu(&books_entries, &books_menu_selected);
    
    // 搜索结果相关
    std::vector<std::string> search_entries;
    int search_menu_selected = 0;
    auto search_menu = Menu(&search_entries, &search_menu_selected);
    
    // 创建输入框
    auto id_input = Input(&search_id, "输入图书ID搜索");
    auto title_input = Input(&search_title, "输入图书标题搜索");
    
    // 创建搜索按钮
    bool search_clicked = false;
    auto search_button = Button("搜索", [&] { search_clicked = true; });
    
    // 创建返回按钮
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        // 根据用户角色决定返回到哪个页面
        if (user->getRole() == User::ADMIN) {
            manage_books_page(user);
        } else {
            student_dashboard_page(user);
        }
    });
    
    // 搜索框和按钮区域
    auto search_container = Container::Vertical({
        id_input,
        title_input,
        search_button,
        back_button
    });
    
    // TAB导航相关 - 添加焦点控制变量
    bool books_area_focused = true;
    
    // 使用合适的布局容器
    auto main_container = Container::Vertical({
        books_menu,
        Container::Vertical({
            search_container,
            search_menu
        })
    });
    
    // 渲染器
    auto renderer = Renderer(main_container, [&] {
        // 表头 - 根据用户角色显示不同的表头
        Element header;
        if (user->getRole() == User::ADMIN) {
            // 管理员表头包含借阅者列
            header = hbox({
                text("ID") | size(WIDTH, EQUAL, 8),
                text(" | "),
                text("标题") | size(WIDTH, EQUAL, 20),
                text(" | "),
                text("作者") | size(WIDTH, EQUAL, 15),
                text(" | "),
                text("类型") | size(WIDTH, EQUAL, 12),
                text(" | "),
                text("出版社") | size(WIDTH, EQUAL, 15),
                text(" | "),
                text("ISBN") | size(WIDTH, EQUAL, 15),
                text(" | "),
                text("状态") | size(WIDTH, EQUAL, 8),
                text(" | "),
                text("借阅者") | size(WIDTH, EQUAL, 8)
            }) | bold;
        } else {
            // 学生表头不显示借阅者列
            header = hbox({
                text("ID") | size(WIDTH, EQUAL, 8),
                text(" | "),
                text("标题") | size(WIDTH, EQUAL, 20),
                text(" | "),
                text("作者") | size(WIDTH, EQUAL, 15),
                text(" | "),
                text("类型") | size(WIDTH, EQUAL, 12),
                text(" | "),
                text("出版社") | size(WIDTH, EQUAL, 15),
                text(" | "),
                text("ISBN") | size(WIDTH, EQUAL, 15),
                text(" | "),
                text("状态") | size(WIDTH, EQUAL, 8)
            }) | bold;
        }
        
        // 根据焦点状态选择不同的边框风格和文本
        std::string books_title = books_area_focused ? "所有图书 [已选中]" : "所有图书";
        std::string search_title = !books_area_focused ? "搜索区域 [已选中]" : "搜索区域";
        
        // 修改渲染以处理菜单项的样式
        return vbox({
            // 页面标题
            text("图书查询") | bold | hcenter,
            text("用户: " + user->getName()) | hcenter,
            separator(),
            
            // 上半部分：所有图书列表（使用Menu组件）
            vbox({
                text(books_title) | bold | hcenter,
                header,
                separator(),
                // 在这里直接对Menu渲染结果应用样式
                books_menu->Render() | yframe | vscroll_indicator | size(HEIGHT, LESS_THAN, 10)
            }),
            
            // 下半部分：搜索区域
            vbox({
                // 搜索控件
                text(search_title) | bold | hcenter,
                
                // 修改为垂直布局并居中
                vbox({
                    hbox(text("按ID搜索:"), id_input->Render()) | hcenter,
                    hbox(text("按标题搜索:"), title_input->Render()) | hcenter,
                    search_button->Render() | hcenter
                }),
                
                // 搜索结果
                has_searched ? vbox({
                    text("搜索结果") | bold | hcenter,
                    header,
                    separator(),
                    search_entries.empty() 
                        ? text("没有找到图书") | hcenter 
                        : search_menu->Render() | yframe | vscroll_indicator | size(HEIGHT, LESS_THAN, 6)
                }) : text(""),
                
                // 返回按钮
                back_button->Render() | hcenter
            }),
        });
    });
    
    // 事件处理
    auto event_handler = CatchEvent(renderer, [&](Event event) {
        // 处理TAB键 - 在书籍区域和搜索区域之间切换
        if (event == Event::Tab) {
            books_area_focused = !books_area_focused;
            
            // 根据当前焦点区域设置输入焦点到相应组件
            if (books_area_focused) {
                main_container->TakeFocus();
                books_menu->TakeFocus();
            } else {
                search_container->TakeFocus();
                id_input->TakeFocus();
            }
            
            return true;
        }
        
        // 处理搜索按钮点击
        if (search_clicked) {
            search_clicked = false;
            has_searched = true;
            search_results.clear();
            search_entries.clear();
            
            // 根据输入的ID或标题搜索图书
            for (const auto& b : all_books) {
                // ID搜索（精确匹配）
                if (!search_id.empty() && b.getBookId() == search_id) {
                    search_results.push_back(b);
                    search_entries.push_back(createBookEntry(b));
                    continue;
                }
                
                // 标题搜索（模糊匹配）
                if (!search_title.empty() && 
                    b.getTitle().find(search_title) != std::string::npos) {
                    search_results.push_back(b);
                    search_entries.push_back(createBookEntry(b));
                }
            }
            
            // 重置搜索菜单选择
            search_menu_selected = 0;
            
            // 如果有搜索结果，自动切换到搜索结果区域
            if (!search_entries.empty()) {
                books_area_focused = false;
                search_menu->TakeFocus();
            }
            
            return true;
        }
        
        return false;
    });
    
    // 使用屏幕的 Loop 方法运行界面
    screen.Loop(event_handler);
}

// 添加删除图书页面
void delete_book_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 图书ID变量
    std::string book_id;
    std::string error_message;
    bool show_error = false;
    bool show_success = false;
    
    // 创建输入框
    auto id_input = Input(&book_id, "请输入要删除的图书ID") | size(WIDTH, EQUAL, 30);
    
    // 创建删除按钮
    bool delete_clicked = false;
    auto delete_button = Button("删除", [&] { delete_clicked = true; });
    
    // 创建返回按钮
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        manage_books_page(user);
    });
    
    // 主容器
    auto container = Container::Vertical({
        id_input,
        Container::Horizontal({
            delete_button,
            back_button
        })
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("删除图书") | bold | hcenter,
            separator(),
            vbox({
                hbox(text(" 图书ID: "), id_input->Render()) | hcenter,
                show_error ? text(error_message) | color(Color::Red) | hcenter : text(""),
                show_success ? text("图书删除成功！") | color(Color::Green) | hcenter : text(""),
                separator(),
                hbox(delete_button->Render(), back_button->Render()) | hcenter,
            }) | border,
        });
    });

    // 事件处理
    auto event_handler = CatchEvent(renderer, [&](Event event) {
        // 处理删除按钮点击
        if (delete_clicked) {
            delete_clicked = false;
            
            // 基本输入验证
            if (book_id.empty()) {
                show_error = true;
                error_message = "图书ID不能为空";
                return true;
            }
            
            // 查找图书是否存在
            book* bookToDelete = book::findBookById(book_id);
            if (!bookToDelete) {
                show_error = true;
                error_message = "未找到ID为 " + book_id + " 的图书";
                return true;
            }
            
            // 直接删除图书，无需确认
            // 读取所有图书
            std::vector<book> allBooks = book::loadAllBooks();
            bool found = false;
            
            // 找到并删除指定ID的图书
            for (auto it = allBooks.begin(); it != allBooks.end(); ++it) {
                if (it->getBookId() == book_id) {
                    allBooks.erase(it);
                    found = true;
                    break;
                }
            }
            
            if (found) {
                // 保存更新后的图书列表
                bool success = book::saveToFile(allBooks, "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/book.json");
                
                if (success) {
                    show_success = true;
                    show_error = false;
                    book_id.clear(); // 清空输入框，方便继续删除
                    
                    // 显示成功消息，并在短暂延迟后自动返回
                    std::thread([&screen]() {
                        std::this_thread::sleep_for(std::chrono::milliseconds(800));
                        screen.ExitLoopClosure()();
                    }).detach();
                } else {
                    show_error = true;
                    error_message = "保存文件时出错";
                }
            } else {
                show_error = true;
                error_message = "删除图书失败";
            }
            
            return true;
        }
        
        return false;
    });

    // 启动交互循环
    screen.Loop(event_handler);
    
    // 如果是因为删除成功而退出的，则返回图书管理页面
    if (show_success) {
        clearScreen();
        manage_books_page(user);
    }
}

// 用户管理页面
void manage_users_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // 加载所有学生用户
    std::vector<User> all_users = User::loadAllUsers();
    std::vector<User> student_users;
    
    // 过滤出学生用户
    for (const auto& u : all_users) {
        if (u.getRole() == User::STUDENT) {
            student_users.push_back(u);
        }
    }
    
    // 搜索条件
    std::string search_id;
    bool has_searched = false;
    
    // 搜索结果
    std::vector<User> search_results;
    
    // 用户详情相关变量
    bool show_user_detail = false;  // 是否显示用户详情
    User selected_user;  // 当前选中的用户
    std::string new_password;  // 新密码输入
    std::string password_message;  // 密码操作结果消息
    bool password_success = false;  // 密码修改是否成功
    bool password_error = false;  // 密码修改是否有错误
    
    // TAB导航相关 - 添加三区域焦点控制变量
    enum FocusArea {
        USER_LIST,
        SEARCH_AREA,
        PASSWORD_AREA
    };
    FocusArea current_focus = USER_LIST;
    
    // 加载所有图书标题，便于查询
    std::map<std::string, std::string> bookTitles;
    std::ifstream bookFile("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/books/book.json");
    if (bookFile.is_open()) {
        try {
            json booksJson;
            bookFile >> booksJson;
            
            for (const auto& book : booksJson) {
                std::string id = book["bookId"].get<std::string>();
                std::string title = book["title"].get<std::string>();
                bookTitles[id] = title;
            }
        } catch (const std::exception& e) {
            std::cerr << "解析图书数据时出错: " << e.what() << std::endl;
        }
        bookFile.close();
    }
    
    // 模拟的借阅记录类型定义
    struct BorrowRecord {
        int bookID;
        std::string bookTitle;   // 图书标题
        time_t borrowTime;
        time_t returnTime;
        bool isOverdue;          // 是否逾期
        int overdueDays;         // 逾期天数
    };
    
    // 从record.json加载用户的借阅记录
    auto loadUserBorrowRecords = [&bookTitles](int userId) -> std::vector<BorrowRecord> {
        std::vector<BorrowRecord> records;
        
        // 打开并读取record.json文件
        std::ifstream file("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
        if (!file.is_open()) {
            std::cerr << "无法打开借阅记录文件" << std::endl;
            return records;
        }
        
        try {
            // 解析JSON数据
            json recordsJson;
            file >> recordsJson;
            
            // 获取当前时间
            time_t currentTime = time(nullptr);
            
            // 遍历所有记录，找出属于该用户的记录
            for (const auto& record : recordsJson) {
                if (record["borrowerID"].get<int>() == userId) {
                    BorrowRecord borrowRecord;
                    borrowRecord.bookID = record["bookID"].get<int>();
                    
                    // 关联查询图书标题
                    std::string bookIdStr = std::to_string(borrowRecord.bookID);
                    if (bookTitles.find(bookIdStr) != bookTitles.end()) {
                        borrowRecord.bookTitle = bookTitles[bookIdStr];
                    } else {
                        borrowRecord.bookTitle = "未知图书";
                    }
                    
                    borrowRecord.borrowTime = record["borrowTime"].get<time_t>();
                    borrowRecord.returnTime = record["returnTime"].get<time_t>();
                    
                    // 检查是否逾期（假设借阅期限为15天）
                    const time_t borrowPeriod = 15 * 24 * 60 * 60; // 15天（秒）
                    const time_t dueTime = borrowRecord.borrowTime + borrowPeriod;
                    
                    // 计算逾期天数
                    if (borrowRecord.returnTime == 0) { // 未归还
                        if (currentTime > dueTime) { // 已逾期
                            borrowRecord.isOverdue = true;
                            borrowRecord.overdueDays = (currentTime - dueTime) / (24 * 60 * 60);
                        } else {
                            borrowRecord.isOverdue = false;
                            borrowRecord.overdueDays = 0;
                        }
                    } else { // 已归还
                        if (borrowRecord.returnTime > dueTime) { // 归还时已逾期
                            borrowRecord.isOverdue = false; // 已归还，不标红
                            borrowRecord.overdueDays = (borrowRecord.returnTime - dueTime) / (24 * 60 * 60);
                        } else {
                            borrowRecord.isOverdue = false;
                            borrowRecord.overdueDays = 0;
                        }
                    }
                    
                    records.push_back(borrowRecord);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "解析借阅记录出错: " << e.what() << std::endl;
        }
        
        file.close();
        return records;
    };
    
    // 格式化时间函数
    auto formatTime = [](time_t time) -> std::string {
        if (time == 0) return "未归还";
        
        struct tm* timeinfo = localtime(&time);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
        return std::string(buffer);
    };
    
    // 创建用户项目的显示项
    auto createUserEntry = [](const User& u) -> std::string {
        // 固定宽度以防止溢出
        const int id_width = 10;
        const int name_width = 20;
        
        // 截断过长的字符串并确保填充宽度不为负数
        std::string id = std::to_string(u.getId()).substr(0, id_width);
        std::string name = u.getName().substr(0, name_width);
        
        // 添加空格填充至固定宽度
        int id_padding = std::max(0, id_width - (int)id.length());
        int name_padding = std::max(0, name_width - (int)name.length());
        
        // 创建格式化的条目字符串
        std::string entry = 
            id + std::string(id_padding, ' ') + " | " +
            name + std::string(name_padding, ' ');
            
        return entry;
    };
    
    // 检查学生是否有逾期图书的函数
    auto hasOverdueBooks = [](const User& u) -> bool {
        // 加载所有借阅记录
        std::vector<record> allRecords = record::readFromFile("/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
        
        // 筛选出该用户的记录
        std::vector<record> userRecords = record::getUserRecords(u.getId(), allRecords);
        
        // 检查用户的记录中是否有未归还且逾期的
        // 注意：已归还的书籍，即使之前逾期归还，也不会导致用户显示为红色
        for (const auto& rec : userRecords) {
            if (!rec.isReturned() && rec.isOverdue()) {
                return true; // 只有当前有未归还的逾期图书时才返回true
            }
        }
        
        // 没有未归还的逾期记录，或已归还全部图书（即使有些是逾期归还的）
        return false;
    };
    
    // 为所有学生用户生成条目
    std::vector<std::string> users_entries;
    std::vector<bool> is_overdue;
    
    for (const auto& u : student_users) {
        users_entries.push_back(createUserEntry(u));
        is_overdue.push_back(hasOverdueBooks(u));
    }
    
    // 创建菜单组件，用于显示和选择用户
    int users_menu_selected = 0;
    auto users_menu = Menu(&users_entries, &users_menu_selected);
    
    // 自定义菜单样式，使逾期学生显示为红色
    auto users_menu_renderer = Renderer(users_menu, [&] {
        Elements elements;
        for (size_t i = 0; i < users_entries.size(); ++i) {
            Element e = text(users_entries[i]);
            
            // 如果用户有未归还的逾期图书，显示为红色
            // 否则显示为白色（包括已归还全部图书，即使有些是逾期归还的）
            if (is_overdue[i]) {
                e = e | color(Color::Red);
            } else {
                e = e | color(Color::White);
            }
            
            // 如果是被选中的项目，添加高亮
            if ((int)i == users_menu_selected) {
                e = e | bgcolor(Color::Blue);
            }
            
            elements.push_back(e);
        }
        return vbox(elements) | frame;
    });
    
    // 搜索结果相关
    std::vector<std::string> search_entries;
    std::vector<bool> search_is_overdue;
    int search_menu_selected = 0;
    
    // 创建搜索结果菜单的渲染器
    auto search_menu = Menu(&search_entries, &search_menu_selected);
    auto search_menu_renderer = Renderer(search_menu, [&] {
        Elements elements;
        for (size_t i = 0; i < search_entries.size(); ++i) {
            Element e = text(search_entries[i]);
            
            // 如果有逾期图书，使用红色显示
            if (search_is_overdue[i]) {
                e = e | color(Color::Red);
            }
            
            // 如果是被选中的项目，添加高亮
            if ((int)i == search_menu_selected) {
                e = e | bgcolor(Color::Blue);
            }
            
            elements.push_back(e);
        }
        return vbox(elements) | frame;
    });
    
    // 创建输入框
    auto id_input = Input(&search_id, "输入用户ID搜索");
    
    // 创建新密码输入框
    auto password_input = Input(&new_password, "输入新密码") | size(WIDTH, EQUAL, 30);
    
    // 创建搜索按钮
    bool search_clicked = false;
    auto search_button = Button("搜索", [&] { search_clicked = true; });
    
    // 创建修改密码按钮
    bool change_password_clicked = false;
    auto change_password_button = Button("修改密码", [&] { change_password_clicked = true; });
    
    // 创建返回按钮
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        admin_dashboard_page(user);
    });
    
    // 搜索框和按钮区域
    auto search_container = Container::Vertical({
        id_input,
        search_button,
        back_button
    });
    
    // 密码修改区域容器
    auto password_container = Container::Vertical({
        password_input,
        change_password_button
    });
    
    // 使用合适的布局容器
    auto main_container = Container::Horizontal({
        users_menu,
        Container::Vertical({
            search_container
        })
    });
    
    // 渲染器
    auto renderer = Renderer(main_container, [&] -> Element {
        // 表头
        Element header = hbox({
            text("用户ID") | size(WIDTH, EQUAL, 10),
            text(" | "),
            text("用户名") | size(WIDTH, EQUAL, 20)
        }) | bold;
        
        // 根据焦点状态选择不同的边框风格和文本
        std::string users_title, search_title, password_title;
        
        if (current_focus == USER_LIST) {
            users_title = "学生用户列表 [已选中]";
            search_title = "搜索区域";
            password_title = "密码修改区域";
        } else if (current_focus == SEARCH_AREA) {
            users_title = "学生用户列表";
            search_title = "搜索区域 [已选中]";
            password_title = "密码修改区域";
        } else { // PASSWORD_AREA
            users_title = "学生用户列表";
            search_title = "搜索区域";
            password_title = "密码修改区域 [已选中]";
        }
        
        Elements elements;
        
        // 上方区域：用户列表和搜索功能
        elements.push_back(vbox({
            // 页面标题
            text("用户管理") | bold | hcenter,
            text("管理员: " + user->getName()) | hcenter,
            text("注：有逾期图书的学生显示为红色") | color(Color::Red) | hcenter,
            text("选中用户后按回车键查看详情") | color(Color::Blue) | hcenter,
            text("按Tab键可在列表、搜索和密码区域之间切换") | color(Color::Yellow) | hcenter,
            separator(),
            
            // 左侧：所有学生用户列表 + 右侧：搜索区域
            hbox({
                vbox({
                    text(users_title) | bold | hcenter,
                    header,
                    separator(),
                    // 在这里使用自定义渲染器
                    users_menu_renderer->Render() | yframe | vscroll_indicator | size(HEIGHT, LESS_THAN, 10)
                }) | flex,
                
                // 右侧：搜索区域
                vbox({
                    // 搜索控件
                    text(search_title) | bold | hcenter,
                    
                    // 修改为垂直布局并居中
                    vbox({
                        hbox(text("按ID搜索:"), id_input->Render()),
                        search_button->Render(),
                        separator(),
                        
                        // 搜索结果
                        has_searched ? vbox({
                            text("搜索结果") | bold,
                            header,
                            separator(),
                            search_entries.empty() 
                                ? text("没有找到用户") 
                                : search_menu_renderer->Render() | yframe | vscroll_indicator | size(HEIGHT, LESS_THAN, 6)
                        }) : text(""),
                        
                        // 返回按钮
                        back_button->Render()
                    }) | size(WIDTH, GREATER_THAN, 40),
                }) | size(WIDTH, EQUAL, 45),
            }),
        }));
        
        // 下方区域：用户详情区（如果已选择用户）
        if (show_user_detail) {
            // 获取用户的借阅记录
            std::vector<BorrowRecord> borrow_records = loadUserBorrowRecords(selected_user.getId());
            
            // 创建借阅记录表格头部
            Elements borrowed_elements;
            borrowed_elements.push_back(
                hbox({
                    text("图书ID") | size(WIDTH, EQUAL, 8) | bold,
                    text(" | "),
                    text("图书标题") | size(WIDTH, EQUAL, 20) | bold,
                    text(" | "),
                    text("借阅日期") | size(WIDTH, EQUAL, 12) | bold,
                    text(" | "),
                    text("归还日期") | size(WIDTH, EQUAL, 12) | bold,
                    text(" | "),
                    text("状态") | size(WIDTH, EQUAL, 8) | bold,
                    text(" | "),
                    text("逾期天数") | size(WIDTH, EQUAL, 10) | bold,
                }) | hcenter
            );
            borrowed_elements.push_back(separator());
            
            if (borrow_records.empty()) {
                borrowed_elements.push_back(text("该用户没有借阅记录") | hcenter);
            } else {
                for (const auto& record : borrow_records) {
                    // 创建一行记录
                    Element book_entry = hbox({
                        text(std::to_string(record.bookID)) | size(WIDTH, EQUAL, 8),
                        text(" | "),
                        text(record.bookTitle) | size(WIDTH, EQUAL, 20),
                        text(" | "),
                        text(formatTime(record.borrowTime)) | size(WIDTH, EQUAL, 12),
                        text(" | "),
                        text(formatTime(record.returnTime)) | size(WIDTH, EQUAL, 12),
                        text(" | "),
                        text(record.returnTime == 0 ? "借阅中" : "已归还") | size(WIDTH, EQUAL, 8),
                        text(" | "),
                        text(record.overdueDays > 0 ? std::to_string(record.overdueDays) + "天" : "-") | size(WIDTH, EQUAL, 10),
                    });
                    
                    // 如果是未归还且逾期，显示为红色
                    if (record.isOverdue && record.returnTime == 0) {
                        book_entry = book_entry | color(Color::Red);
                    }
                    
                    borrowed_elements.push_back(book_entry);
                }
            }
            
            // 添加用户详情区域
            elements.push_back(separator());
            elements.push_back(
                vbox({
                    text("用户详情：" + selected_user.getName() + " (ID: " + std::to_string(selected_user.getId()) + ")") | bold | hcenter,
                    separator(),
                    
                    // 借阅记录区域
                    vbox({
                        text("借阅记录") | bold | hcenter,
                        vbox(borrowed_elements) | border | size(HEIGHT, LESS_THAN, 8),
                    }),
                    
                    separator(),
                    
                    // 密码修改区域
                    vbox({
                        text(password_title) | bold | hcenter,
                        hbox(text("新密码: "), password_input->Render() | size(WIDTH, EQUAL, 30)) | hcenter,
                        change_password_button->Render() | hcenter,
                        password_error ? text(password_message) | color(Color::Red) | hcenter : text(""),
                        password_success ? text(password_message) | color(Color::Green) | hcenter : text(""),
                    }) | border,
                }) | border
            );
        }
        
        return vbox(elements);
    });
    
    // 事件处理
    auto event_handler = CatchEvent(renderer, [&](Event event) {
        // 处理搜索按钮点击
        if (search_clicked) {
            search_clicked = false;
            has_searched = true;
            search_results.clear();
            search_entries.clear();
            search_is_overdue.clear();
            
            // 查找匹配的用户ID
            for (const auto& u : student_users) {
                if (std::to_string(u.getId()).find(search_id) != std::string::npos) {
                    search_results.push_back(u);
                    search_entries.push_back(createUserEntry(u));
                    search_is_overdue.push_back(hasOverdueBooks(u));
                }
            }
            
            // 重置搜索菜单选择
            search_menu_selected = 0;
            
            // 如果有搜索结果，自动切换到搜索结果区域
            if (!search_entries.empty()) {
                current_focus = SEARCH_AREA;
                search_menu->TakeFocus();
            }
            
            return true;
        }
        
        // 处理修改密码按钮点击
        if (change_password_clicked && show_user_detail) {
            change_password_clicked = false;
            
            // 验证新密码
            if (new_password.empty()) {
                password_error = true;
                password_success = false;
                password_message = "密码不能为空";
                return true;
            }
            
            // 调用管理员修改密码方法
            bool success = User::adminChangeUserPassword(user->getId(), selected_user.getId(), new_password);
            if (success) {
                password_error = false;
                password_success = true;
                password_message = "密码修改成功";
                new_password.clear();  // 清空密码输入
            } else {
                password_error = true;
                password_success = false;
                password_message = "密码修改失败";
            }
            
            return true;
        }
        
        // 处理回车键 - 显示选中用户的详情
        if (event == Event::Return) {
            if (current_focus == USER_LIST && !student_users.empty()) {
                // 从主列表获取选中的用户
                selected_user = student_users[users_menu_selected];
                show_user_detail = true;
                password_error = false;
                password_success = false;
                new_password.clear();
                // 将焦点设置到密码输入框
                current_focus = PASSWORD_AREA;
                password_container->TakeFocus();
                password_input->TakeFocus();
                return true;
            } else if (current_focus == SEARCH_AREA && has_searched && !search_results.empty()) {
                // 从搜索结果获取选中的用户
                selected_user = search_results[search_menu_selected];
                show_user_detail = true;
                password_error = false;
                password_success = false;
                new_password.clear();
                // 将焦点设置到密码输入框
                current_focus = PASSWORD_AREA;
                password_container->TakeFocus();
                password_input->TakeFocus();
                return true;
            } else if (current_focus == PASSWORD_AREA && show_user_detail) {
                // 如果在密码区域按回车，触发修改密码
                if (!new_password.empty()) {
                    bool success = User::adminChangeUserPassword(user->getId(), selected_user.getId(), new_password);
                    if (success) {
                        password_error = false;
                        password_success = true;
                        password_message = "密码修改成功";
                        new_password.clear();  // 清空密码输入
                    } else {
                        password_error = true;
                        password_success = false;
                        password_message = "密码修改失败";
                    }
                } else {
                    password_error = true;
                    password_success = false;
                    password_message = "密码不能为空";
                }
                return true;
            }
        }
        
        // 处理TAB键 - 在用户列表区域、搜索区域和密码修改区域之间切换
        if (event == Event::Tab) {
            if (show_user_detail) {
                // 如果显示用户详情，则在三个区域间循环切换
                if (current_focus == USER_LIST) {
                    current_focus = SEARCH_AREA;
                    search_container->TakeFocus();
                    id_input->TakeFocus();
                } else if (current_focus == SEARCH_AREA) {
                    current_focus = PASSWORD_AREA;
                    password_container->TakeFocus();
                    password_input->TakeFocus();
                } else { // PASSWORD_AREA
                    current_focus = USER_LIST;
                    main_container->TakeFocus();
                    users_menu->TakeFocus();
                }
            } else {
                // 如果未显示用户详情，只在用户列表和搜索区域之间切换
                if (current_focus == USER_LIST) {
                    current_focus = SEARCH_AREA;
                    search_container->TakeFocus();
                    id_input->TakeFocus();
                } else {
                    current_focus = USER_LIST;
                    main_container->TakeFocus();
                    users_menu->TakeFocus();
                }
            }
            
            return true;
        }
        
        return false;
    });
    
    // 使用屏幕的 Loop 方法运行界面
    screen.Loop(event_handler);
}

// 图书统计信息页面
void view_stats_page(User* user) {
    clearScreen();
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();
    
    // 加载所有图书
    std::vector<book> all_books = book::loadAllBooks();
    
    // 获取所有图书借阅次数
    std::map<int, int> borrow_counts = record::getBookBorrowCountsFromFile(
        "/Users/chiyoshi/Documents/CLionOJProject/wang-chongxi-2024-25310619/records/record.json");
    
    // 创建包含借阅次数的图书信息列表
    struct BookWithBorrowCount {
        book bookInfo;
        int borrowCount;
        
        // 构造函数
        BookWithBorrowCount(const book& b, int count) : bookInfo(b), borrowCount(count) {}
    };
    
    std::vector<BookWithBorrowCount> books_with_counts;
    
    // 合并图书信息和借阅次数
    for (const auto& b : all_books) {
        int bookID;
        try {
            bookID = std::stoi(b.getBookId());
        } catch (const std::exception& e) {
            std::cerr << "Error converting book ID: " << b.getBookId() << std::endl;
            continue;
        }
        
        // 查找该书的借阅次数，如果没有记录则为0
        int count = 0;
        if (borrow_counts.find(bookID) != borrow_counts.end()) {
            count = borrow_counts[bookID];
        }
        
        books_with_counts.emplace_back(b, count);
    }
    
    // 按借阅次数降序排序
    std::sort(books_with_counts.begin(), books_with_counts.end(), 
              [](const BookWithBorrowCount& a, const BookWithBorrowCount& b) {
                  return a.borrowCount > b.borrowCount;
              });
    
    // 创建表格条目
    std::vector<std::string> table_entries;
    
    // 为每本书创建表格行
    for (const auto& item : books_with_counts) {
        const book& b = item.bookInfo;
        int count = item.borrowCount;
        
        // 将书籍类型转换为字符串
        std::string typeStr;
        switch(b.getBookType()) {
            case book::FICTION: typeStr = "小说"; break;
            case book::NON_FICTION: typeStr = "非小说"; break;
            case book::SCIENCE: typeStr = "科学"; break;
            case book::HISTORY: typeStr = "历史"; break;
            case book::BIOGRAPHY: typeStr = "传记"; break;
            case book::FANTASY: typeStr = "奇幻"; break;
            case book::MYSTERY: typeStr = "悬疑"; break;
            case book::ROMANCE: typeStr = "爱情"; break;
            default: typeStr = "未知"; break;
        }
        
        // 固定宽度以防止溢出
        const int id_width = 8;
        const int title_width = 20;
        const int author_width = 15;
        const int type_width = 10;
        const int count_width = 10;
        
        // 截断过长的字符串并确保填充宽度不为负数
        std::string id = b.getBookId().substr(0, id_width);
        std::string title = b.getTitle().substr(0, title_width);
        std::string author = b.getAuthor().substr(0, author_width);
        typeStr = typeStr.substr(0, type_width);
        std::string borrowCount = std::to_string(count);
        
        // 添加空格填充至固定宽度
        int id_padding = std::max(0, id_width - (int)id.length());
        int title_padding = std::max(0, title_width - (int)title.length());
        int author_padding = std::max(0, author_width - (int)author.length());
        int type_padding = std::max(0, type_width - (int)typeStr.length());
        int count_padding = std::max(0, count_width - (int)borrowCount.length());
        
        // 创建格式化的条目字符串
        std::string entry = 
            id + std::string(id_padding, ' ') + " | " +
            title + std::string(title_padding, ' ') + " | " +
            author + std::string(author_padding, ' ') + " | " +
            typeStr + std::string(type_padding, ' ') + " | " +
            borrowCount + std::string(count_padding, ' ');
            
        table_entries.push_back(entry);
    }
    
    // 创建菜单组件，用于显示图书借阅统计
    int menu_selected = 0;
    auto table_menu = Menu(&table_entries, &menu_selected);
    
    // 创建返回按钮
    auto back_button = Button("返回", [&] {
        screen.ExitLoopClosure()();
        clearScreen();
        
        // 根据用户角色返回相应页面
        if (user && user->getRole() == User::ADMIN) {
            admin_dashboard_page(user);
        } else {
            student_dashboard_page(user);
        }
    });
    
    // 主容器
    auto container = Container::Vertical({
        table_menu,
        back_button
    });
    
    // 主渲染函数
    auto renderer = Renderer(container, [&] {
        // 表头
        Element header = hbox({
            text("图书ID") | size(WIDTH, EQUAL, 8) | bold,
            text(" | "),
            text("标题") | size(WIDTH, EQUAL, 20) | bold,
            text(" | "),
            text("作者") | size(WIDTH, EQUAL, 15) | bold,
            text(" | "),
            text("类型") | size(WIDTH, EQUAL, 10) | bold,
            text(" | "),
            text("借阅次数") | size(WIDTH, EQUAL, 10) | bold
        });
        
        return vbox({
            text("图书借阅统计 (降序排列)") | bold | hcenter,
            text("用户: " + (user ? user->getName() : "访客")) | hcenter,
            separator(),
            
            vbox({
                header,
                separator(),
                table_menu->Render() | yframe | vscroll_indicator | size(HEIGHT, LESS_THAN, 15)
            }) | border,
            
            separator(),
            back_button->Render() | hcenter
        });
    });
    
    // 启动交互循环
    screen.Loop(renderer);
}