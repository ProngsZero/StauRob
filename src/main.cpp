#include <iostream>
#include <windows.h>
#include <string>
#include <sstream> 

#include "core/StauRob.h"
#include "utils/OpenSSLWrapper.h"

bool running = true;

DWORD WINAPI commandListener(LPVOID lpParam) 
{
    StauRob* rob = static_cast<StauRob*>(lpParam);
    std::string input;

    while (running) 
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "check") 
        {
            std::cout << "[Command] Checking sensors...\n";
            rob->checkSensors();
        }
        else if (command == "stop") 
        {
            std::cout << "[Command] Stopping rob...\n";
            rob->stop();
        }
        else if (command == "exit") 
        {
            std::cout << "[Command] Exiting program...\n";
            rob->stop();
            running = false;
        }
        else if (command == "move") 
        {
            double value = 0;
            if (iss >> value) 
            {
                std::cout << "[Command] Moving... " << std::endl;
                rob->move(value);
            }
        }
        else if (command == "steps") 
        {
            rob->printStepsTaken();
        }
        else if (command == "bumper") 
        {
            int value = 0;
            if (iss >> value) 
            {
                std::cout << "[Command] Setting BumperSensor to " << value << "\n";
                rob->setValue(2, value > 0);
            }
        }
        else if (command == "cliff") 
        {
            int value = 0;
            if (iss >> value) 
            {
                std::cout << "[Command] Setting CliffSensor to " << value << "\n";
                rob->setValue(5, value > 0);
            }
        }
        else 
        {
            std::cout << "[Error] Unknown command: " << command << "\n";
        }
    }
    return 0;
}


int main() {

    char* name = (char*)malloc(6);
    ISensor* sensorsArr[4] = {
        new ProximitySensor(),
        new Gyroscope(),
        new BumperSensor(),
        new cliffSensor()
    };
    StauRob* rob;
    DWORD threadId;
    HANDLE hThread;

    rob = new StauRob(sensorsArr, name);
    hThread = CreateThread(
        NULL,
        0,
        commandListener,
        rob,
        0,
        &threadId
    );

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    delete rob;
    return 0;
}