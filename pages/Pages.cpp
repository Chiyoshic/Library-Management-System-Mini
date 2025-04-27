//
// Created by Chiyoshi on 2025/4/9.
//
#include <iostream>
#include "Pages.h"
#include <User.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
using namespace std;

void printIndex() {
    system("clear");
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
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();
    
    // 创建功能按钮
    auto manage_books_button = Button("图书管理", [&] {
        screen.ExitLoopClosure()();
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
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();
    
    // 创建图书管理功能按钮
    auto search_books_button = Button("查询图书", [] {
        // 查询图书功能待实现
        std::cout << "进入查询图书页面" << std::endl;
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
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();
    
    // 创建功能按钮
    auto search_books_button = Button("搜索图书", [] {
        // 搜索图书功能待实现
        std::cout << "进入图书搜索页面" << std::endl;
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
                
                // 短暂显示成功消息，然后返回主菜单
                screen.PostEvent(Event::Custom);
                return true;
            } else {
                show_error = true;
                show_success = false;
                error_message = "注册失败，用户名可能已存在";
                return true;
            }
        }
        
        // 用于处理注册成功后的返回
        if (event == Event::Custom && show_success) {
            // 延迟一小段时间显示成功消息
            static int delay_count = 0;
            if (++delay_count > 2) {
                delay_count = 0;
                screen.ExitLoopClosure()();
                printMenu();
            }
            // screen.ExitLoopClosure()();
            // this_thread::sleep_for(chrono::seconds(3));
            // printMenu();
            // return true;
        }
        
        return false;
    });

    screen.Loop(register_handler);
}

void printMenu() {
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