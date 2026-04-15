#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h> 
#include <limits>
#include <ctime>

using namespace std;
using namespace chrono;
using namespace this_thread;

// --- 全局配置 ---
const int REFRESH_RATE = 100;  // 毫秒

// --- 终端控制工具 ---
void clearScreen() { 
    system("clear");
}

void setEcho(bool enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable) tty.c_lflag |= ECHO;
    else tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

int kbhit() {
    struct termios oldt, newt;
    int ch, oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void loadingBar(const string& msg) {
    cout << msg << " [" << flush;
    for (int i = 0; i < 20; ++i) {
        cout << "=" << flush;
        sleep_for(milliseconds(50));
    }
    cout << "] Done!\n";
}

// --- 时钟功能 ---
void showtime() {
    clearScreen();
    cout << "======= SYSTEM CLOCK =======\n";
    cout << "[SPACE] Pause/Resume  |  [Q] Back\n";
    cout << "=============================\n\n";
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);           
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);         
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  

    bool paused = false;
    
    while (true) {
        if (kbhit()) {
            char key = getchar();
            if (key == ' ') {
                paused = !paused;
            } else if (tolower(key) == 'q') {
                break;
            }
        }
        
        auto now = system_clock::to_time_t(system_clock::now());
        cout << "\r   Time: " << put_time(localtime(&now), "%H:%M:%S") 
             << "  |  " << (paused ? "⏸ PAUSED" : "▶ RUNNING") 
             << "         " << flush;
        
        sleep_for(milliseconds(REFRESH_RATE));
    }
    
    cout << "\n";
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
}

// --- 计算器功能 ---
void calculator() {
    clearScreen();
    cout << "======= CALCULATOR =======\n";
    cout << "Usage: [number] [operator] [number]\n";
    cout << "Enter '0 q 0' to exit\n";
    cout << "==========================\n\n";
    
    while (true) {
        long double num1, num2, result;
        char op;
        
        cout << "> ";
        if (!(cin >> num1 >> op >> num2)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid input format\n";
            continue;
        }
        
        if (tolower(op) == 'q') break;
        
        bool valid = true;
        
        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    cout << "❌ Division by zero error\n";
                    valid = false;
                } else {
                    result = num1 / num2;
                }
                break;
            default:
                cout << "❌ Unknown operator: " << op << "\n";
                valid = false;
        }
        
        if (valid) {
            cout << fixed << setprecision(6) << num1 << " " << op << " " 
                 << num2 << " = " << result << "\n";
        }
        cout << "\n";
    }
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- 菜单显示 ---
void displayMenu(const string& username) {
    cout << "\n╔════════════════════════════════╗\n";
    cout << "║  Welcome, " << setw(19) << left << username << "║\n";
    cout << "╚════════════════════════════════╝\n";
    cout << "=============================\n";
    cout << "  [1] Clock      \n";
    cout << "  [2] Calculator \n";
    cout << "  [0] Screen Off  \n";
    cout << "  [Q] Logout      \n";
    cout << "=============================\n";
}
// --- 用户认证 ---
string authenticate() {
    clearScreen();
    string username;
    
    cout << "\n╔════════════════════════════════╗\n";
    cout << "║   USER REGISTRATION             ║\n";
    cout << "╚════════════════════════════════╝\n\n";
    
    cout << "Username: ";
    getline(cin, username);
    
    if (username.empty()) {
        cout << "❌ Username cannot be empty!\n";
        sleep_for(seconds(1));
        return authenticate();
    }
    
    string password, password_confirm;
    
    while (true) {
        setEcho(false);
        cout << "Password (min 6 chars): ";
        getline(cin, password);
        cout << "\nConfirm Password: ";
        getline(cin, password_confirm);
        setEcho(true);
        cout << "\n";
        
        if (password.length() < 6) {
            cout << "❌ Password too short (min 6 characters)\n\n";
            continue;
        }
        
        if (password != password_confirm) {
            cout << "❌ Passwords do not match\n\n";
            continue;
        }
        
        cout << "✅ Account created successfully!\n";
        sleep_for(seconds(1));
        return username;
    }
}

// --- 主函数 ---
int main() {
    clearScreen();
    loadingBar("System Booting");
    
    string username = authenticate();
    clearScreen();
    
    while (true) {
        displayMenu(username);
        string input;
        getline(cin, input);
        
        if (input.empty()) continue;
        
        char cmd = tolower(input[0]);
        
        if (cmd == 'q') {
            cout << "\nGoodbye, " << username << "!\n";
            break;
        }
        
        clearScreen();
        
        switch (cmd) {
            case '1':
                showtime();
                break;
            case '2':
                calculator();
                break;
            case '0':
                loadingBar("System Shutdown");
                return 0;
            default:
                cout << "⏳ Feature coming soon...\n";
                sleep_for(seconds(2));
                clearScreen();
        }
    }
    
    return 0;
}
