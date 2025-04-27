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
    
    auto manage_users_button = Button("用户管理", [] {
        // 用户管理功能待实现
        std::cout << "进入用户管理页面" << std::endl;
    });
    
    auto view_stats_button = Button("统计信息", [] {
        // 统计信息功能待实现
        std::cout << "查看统计信息" << std::endl;
    });
    
    auto query_borrows_button = Button("查询借阅", [] {
        // 查询借阅功能待实现
        std::cout << "查询借阅信息" << std::endl;
    });
    
    auto logout_button = Button("退出登录", screen.ExitLoopClosure());
    
    auto container = Container::Vertical({
        manage_books_button,
        manage_users_button,
        view_stats_button,
        query_borrows_button,
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
                query_borrows_button->Render(),
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
    
    auto add_books_button = Button("录入图书", [] {
        // 录入图书功能待实现
        std::cout << "进入录入图书页面" << std::endl;
    });
    
    auto delete_books_button = Button("删除图书", [] {
        // 删除图书功能待实现
        std::cout << "进入删除图书页面" << std::endl;
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
    
    auto my_borrows_button = Button("我的借阅", [] {
        // 我的借阅功能待实现
        std::cout << "查看我的借阅记录" << std::endl;
    });
    
    auto account_button = Button("账户信息", [] {
        // 账户信息功能待实现
        std::cout << "查看账户信息" << std::endl;
    });
    
    auto logout_button = Button("退出登录", screen.ExitLoopClosure());
    
    auto container = Container::Vertical({
        search_books_button,
        my_borrows_button,
        account_button,
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
                account_button->Render(),
                separator(),
                logout_button->Render(),
            }) | border,
        });
    });
    
    screen.Loop(renderer);
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
    
    // 搜索条件
    std::string search_id;
    std::string search_title;
    bool has_searched = false;
    
    // 当前选中的图书索引
    int selected_book_index = 0;
    std::vector<std::string> books_entries;
    
    // 创建图书项目的显示项
    auto createBookEntry = [](const book& b) -> std::string {
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
        const int type_width = 12;
        const int publisher_width = 15;
        const int isbn_width = 15;
        
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
        
        // 创建格式化的条目字符串
        std::string entry = 
            id + std::string(id_padding, ' ') + " | " +
            title + std::string(title_padding, ' ') + " | " +
            author + std::string(author_padding, ' ') + " | " +
            typeStr + std::string(type_padding, ' ') + " | " +
            publisher + std::string(publisher_padding, ' ') + " | " +
            isbn + std::string(isbn_padding, ' ') + " | " +
            (b.getIsAvailable() ? "可借阅" : "已借出");
            
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
        Container::Horizontal({
            id_input,
            title_input,
            search_button
        }),
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
        // 表头
        Element header = hbox({
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
                hbox({
                    vbox({
                        text("按ID搜索:"),
                        id_input->Render()
                    }),
                    vbox({
                        text("按标题搜索:"),
                        title_input->Render()
                    }),
                    vbox({
                        text(" "),
                        search_button->Render()
                    })
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