#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
#include <fcntl.h> 
#include <limits>
#include <atomic> 
using namespace std;
using namespace chrono;
using namespace chrono_literals;
using namespace this_thread;
void setEcho(bool enable) 
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable) tty.c_lflag &= ~ECHO;
    else tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
int kbhit() 
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) 
    {
        ungetc(ch, stdin); 
        return 1;
    }
    return 0;
}
atomic<bool> Run1(true);
void showtime() 
{
    bool paused = false;
    cout << "\n--- Clock Started ---" << endl;
    cout << "Controls: [Space] Pause/Resume" << endl;
    while (true) {
        if (kbhit()) {
            char key = getchar();
            if (key == ' ') 
            {
                paused = !paused; 
            } 
            else if (key == 'q' || key == 'Q') 
            {
                cout << "\nExiting Clock...\n";break; 
            }
        }
        if (!paused) {
            auto now = system_clock::now();
            time_t now_c = system_clock::to_time_t(now);
            cout << "\rCurrent Time:" << put_time(localtime(&now_c), "%H:%M:%S") 
                 << " [Running]" << flush;
        } else {cout << "\rCurrent Time: [ Paused ]" << flush;}
        sleep_for(100ms); 
    }
}
void Calculator() 
{
    char a;
    cout << "Hello! Welcome to this calculator. Would you like a tutorial? (Y/n)\n";
    cin >> a;
    if (a=='Y' || a=='y')
    {
        cout << "1. Please do not enter any letters (otherwise it will crash).\n";
        cout << "2. Please use the format [number][operator][number].\n";
        cout << "3. Calculation (Type '0 q 0' to exit)\n";
    }
    cout << "So, press any key to start the calculation!   ";
    string b;
    cin >> b;
    while (true)
    {
        cout << "Calculation:";
        long double num1,num2 ;
        char op;
        if (!(cin >> num1 >> op >> num2)) 
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input format. Please try again.\n" ;
            continue;
        }
        if (op=='q' || op=='Q')
        {
            cout << "Returning to menu...\n";
            break;
        }
        cout << "\033[A\rCalculation:" << num1 << " " << op << " " << num2 << " = ";
        switch (op) 
        {
            case '+': cout << num1 + num2; break;
            case '-': cout << num1 - num2; break;
            case '*': cout << num1 * num2; break;
            case '/': 
                if (num2 == 0) cout << "Error: Division by zero!";
                else cout << num1 / num2;
                break;
            default: cout << "Error: Unknown operator '" << op << "'";
        }
        cout << "\n";
    }
}
void menu()
{
    cout << "1. Clock (1)\n";
    cout << "2. Stopwatch (2)\n";
    cout << "3. Calculator (3)\n";
    cout << "4. Stay tuned.\n";
    cout << "5. Stay tuned.\n";
    cout << "6. Stay tuned.\n";
    cout << "7. Stay tuned.\n";
}
int main() 
{
    cout << "Booting up...\n" ;
    sleep_for(1s);
    string name;
    cout << "\033[2J\033[1;1H" << flush 
         <<"Welcome to the computer system developed by Shawn_Yu.\n " 
         << " I will guide you through its use.\nI will guide you through its use.";
    cin >> name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string password,passwords;
    cout << "OK," << name << ".\n";
    while (true) 
    {
        setEcho(false); // 关闭回显
        cout << "Please set your login password (at least 6 characters):";
        getline(cin, password);
        setEcho(true); // 恢复回显
        cout << "\n";
    if (password.length() >= 6) break;
    cout << "[Error] Password is too short. Please enter it again.\n" ;
    }
    while (true) 
    {
        setEcho(false);
        cout << "Please enter again to confirm:";
        getline(cin, passwords);
        setEcho(true);
        cout << endl;
        if (password == passwords) {
            cout << "✅ Password set successfully!\n";
            break;
        } else {
            cout << "[Error] The two inputs do not match! Please verify again.\n";  
        }
    }
    cout << "Next, proceed to download the software; "
         << "simply select the version you need, enter the code, and you are ready to run it.\n";
    sleep_for(500ms);
    cout << "[Q] Exit to Main Menu   [0] Shut Down\n";
    while (true){
        int a;
        menu();
        if (!(cin >> a)) 
        { 
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number (0-7).\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        while (a<0 || a>7){cout << "Incorrect. Please try again.";cin >> a;}
        printf("Getting ready...");
        sleep_for(1s);
        switch(a) 
        {
            case 0:cout << "The shutdown feature is still under development.";break;
            case 1:showtime();break;
            case 2:cout << "Stopwatch feature under development...\n";break;
            case 3:Calculator();break;
            default:cout << "This feature has not yet been installed.\n";break;
        }
    }
    return 0;
}
