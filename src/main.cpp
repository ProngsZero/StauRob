#include <iostream>
#include <thread>
#include <string>
#include <sstream>  // for std::istringstream

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "C:\Users\artur.hirsch\Desktop\StauRob\include\core\StauRob.h"

bool running = true;

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
            logger->info("Called start");
        }
        else if (command == "stop") {
            std::cout << "[Command] Stopping robot...\n";
            robot.stop();
            logger->info("Called stop");
        }
        else if (command == "exit") {
            std::cout << "[Command] Exiting program...\n";
            robot.stop();
            running = false;
            logger->info("Called exit");
        }
        else if (command == "log") {
            std::string log_message;
            std::getline(iss, log_message);
            std::cout << "[Command] Logging to file...\n";
            logger->info("{}", log_message);
        }
        else if (command == "bumper") {
            int value;
            if (iss >> value) {
                std::cout << "[Command] Setting BumperSensor to " << value << "\n";
                robot.setValue(2, value > 0);
                logger->info("Changed value of bumper to {}", value);
            } else {
                std::cout << "[Error] No value provided for bumper command.\n";
                logger->warn("Failed to change value of Bumpersensor");
            }
        }
        else if (command == "cliff") {
            int value;
            if (iss >> value) {
                std::cout << "[Command] Setting CliffSensor to " << value << "\n";
                robot.setValue(3, value > 0);
                logger->info("Changed value of cliff to {}", value);
            } else {
                std::cout << "[Error] No value provided for cliff command.\n";
                logger->warn("Failed to change value of Cliffsensor");
            }
        }
        else {
            std::cout << "[Error] Unknown command: " << command << "\n";
            logger->warn("Entered unknown command");
        }
    }
}



int main() {
    auto main_logger = spdlog::basic_logger_mt("main", "main.log");
    auto robot_logger = spdlog::basic_logger_mt("StauRob", "core/StauRob.log");
    StauRob robot(robot_logger);

    std::thread inputThread(commandListener, std::ref(robot), main_logger);

    inputThread.join();  // Wait for input thread to finish

    return 0;
}
