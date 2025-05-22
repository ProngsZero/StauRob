#include <iostream>
//#include <thread>
#include <windows.h>
#include <string>
#include <sstream>  // for std::istringstream

#ifdef USELOG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#endif //USELOG
#include "core/StauRob.h"
#include "utils/OpenSSLWrapper.h"

bool running = true;
#ifdef USELOG
void commandListener(StauRob& robot, std::shared_ptr<spdlog::logger> logger) {
    std::string input;

    while (running) {
        std::cout << "> ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "start") {
            std::cout << "[Command] Starting robot...\n";
            robot.run();
            LOG_INFO("Called start");
        }
        else if (command == "stop") {
            std::cout << "[Command] Stopping robot...\n";
            robot.stop();
            LOG_INFO("Called stop");
        }
        else if (command == "exit") {
            std::cout << "[Command] Exiting program...\n";
            robot.stop();
            running = false;
            LOG_INFO("Called exit");
        }
        else if (command == "log") {
            std::string log_message;
            std::getline(iss, log_message);
            std::cout << "[Command] Logging to file...\n";
            LOG_INFO("{}", log_message);
        }
        else if (command == "bumper") {
            int value;
            if (iss >> value) {
                std::cout << "[Command] Setting BumperSensor to " << value << "\n";
                robot.setValue(2, value > 0);
                LOG_INFO("Changed value of bumper to {}", value);
            } else {
                std::cout << "[Error] No value provided for bumper command.\n";
                LOG_INFO("Failed to change value of Bumpersensor");
            }
        }
        else if (command == "cliff") {
            int value;
            if (iss >> value) {
                std::cout << "[Command] Setting CliffSensor to " << value << "\n";
                robot.setValue(3, value > 0);
                LOG_INFO("Changed value of cliff to {}", value);
            } else {
                std::cout << "[Error] No value provided for cliff command.\n";
                LOG_INFO("Failed to change value of Cliffsensor");
            }
        }
        else {
            std::cout << "[Error] Unknown command: " << command << "\n";
            LOG_INFO("Entered unknown command");
        }
    }
}
#else

DWORD WINAPI commandListener(LPVOID lpParam) {
    StauRob* robot = static_cast<StauRob*>(lpParam);
    std::string input;

    while (running) {
        std::cout << "> ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "start") {
            std::cout << "[Command] Starting robot...\n";
            robot->run();
        }
        else if (command == "stop") {
            std::cout << "[Command] Stopping robot...\n";
            robot->stop();
        }
        else if (command == "exit") {
            std::cout << "[Command] Exiting program...\n";
            robot->stop();
            running = false;
        }
        else if (command == "log") {
            std::string log_message;
            std::getline(iss, log_message);
            std::cout << "[Command] Logging to file...\n";
        }
        else if (command == "bumper") {
            int value = 0;
            if (iss >> value) {
                std::cout << "[Command] Setting BumperSensor to " << value << "\n";
                robot->setValue(2, value > 0);
            }
            else {
                std::cout << "[Error] No value provided for bumper command.\n";
            }
        }
        else if (command == "cliff") {
            int value = 0;
            if (iss >> value) {
                std::cout << "[Command] Setting CliffSensor to " << value << "\n";
                robot->setValue(3, value > 0);
            }
            else {
                std::cout << "[Error] No value provided for cliff command.\n";
            }
        }
        else {
            std::cout << "[Error] Unknown command: " << command << "\n";
        }
    }
    return 0;
}
#endif

int main() {
#ifdef USELOG
    auto main_logger = spdlog::basic_logger_mt("main", "main.log");
    auto robot_logger = spdlog::basic_logger_mt("StauRob", "core/StauRob.log");
    StauRob robot(robot_logger);

    std::thread inputThread(commandListener, std::ref(robot), main_logger);
#else
    DWORD threadId;
    HANDLE hThread;
    //StauRob robot;
    StauRob* robot = (StauRob*)malloc(sizeof(StauRob)); //malloc skips constructor and therefore init of loc
    hThread = CreateThread(
        NULL,             // default security attributes
        0,                // default stack size
        commandListener, // thread function
        &robot,             // argument to thread function
        0,                // default creation flags
        &threadId         // receive thread identifier
    );
#endif
    std::cout << "OpenSSL Version: " << GetOpenSSLVersion() << std::endl;
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return 0;
}

/*int main() {
#ifdef USELOG
    auto main_logger = spdlog::basic_logger_mt("main", "main.log");
    auto robot_logger = spdlog::basic_logger_mt("StauRob", "core/StauRob.log");
    StauRob robot(robot_logger);

    std::thread inputThread(commandListener, std::ref(robot), main_logger);
#else
    StauRob robot;
    std::thread inputThread(commandListener, std::ref(robot));
#endif
    inputThread.join();  // Wait for input thread to finish
    return 0;
}*/