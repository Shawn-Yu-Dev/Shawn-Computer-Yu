
markdown
# Shawn_Yu OS 🖥️

A sophisticated terminal-based "Operating System" simulation built in C++. This project features a secure login system, real-time utilities, and a clean command-line interface using low-level terminal manipulation.

## ✨ Key Features

- **Secure Login System:** 
  - Implementation of **Silent Input** (Echo off) for password settings and verification.
  - Length validation and confirmation logic to ensure user security.
- **Real-time Clock (Module 1):**
  - Live digital clock display using `std::chrono`.
  - Supports **non-blocking** keyboard interrupts (`kbhit`) to pause/resume the time.
- **Interactive Calculator (Module 3):**
  - Supports floating-point arithmetic.
  - Features a specialized "Overwrite UI" that uses ANSI escape codes to keep the terminal tidy.
  - Robust error handling for invalid characters and division-by-zero.
- **Low-level Terminal Control:** 
  - Custom POSIX `termios` integration to manage terminal echo and canonical modes.
  - Non-blocking input detection for a responsive "OS" feel.

## 🚀 Getting Started

### Prerequisites
- A C++17 compatible compiler (GCC/Clang recommended).
- A POSIX-compliant terminal (Linux, macOS, or WSL on Windows).
- `ncurses` library (though currently using custom POSIX headers for core logic).

### Compilation
Use the following command to compile the system:
```bash
g++ -std=c++17 -o shawn_os main.cpp
Use code with caution.

Running the System
bash
./shawn_os
Use code with caution.

🛠️ Controls
Main Menu: Enter the corresponding number (0-7) to launch an app.
Clock: Press [Space] to pause/resume, [Q] to return to menu.
Calculator: Follow the [number][operator][number] format. Type 0 q 0 to exit.
📈 Roadmap
Secure Login & User Setup
Real-time Clock with Pause/Resume
Robust Calculator with Overwrite UI
Stopwatch Mode (Case 2 - In Progress)
System Shutdown Sequence (Case 0)
Mini-Games (Future Updates)
Developed with ❤️ by Shawn-Yu-Dev
