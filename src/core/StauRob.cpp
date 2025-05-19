#include "C:\Users\artur.hirsch\Desktop\StauRob\include\core\StauRob.h"

StauRob::StauRob(std::shared_ptr<spdlog::logger> logger)
    : localizer(sensors), // Pass vector of raw pointers
      motorControl(),
      mapper(&localizer),
      running(false),
      logger(logger)
{
    // Create sensors and add raw pointers to the vector
    sensors.push_back(new ProximitySensor()); //sensors[0]
    sensors.push_back(new Gyroscope()); //sensors[1]
    sensors.push_back(new BumperSensor()); //sensors[2]
    sensors.push_back(new CliffSensor()); //sensors[3]
    for (size_t i = 0; i < sensors.size(); ++i) {
        logger->info("Sensor {}: {}", i, sensors[i]->getType());
    }

    logger->info("Started Thread Mapper");
    mapperThread = std::thread([this]() {
        while (running) {
            mapper.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    logger->info("Started Thread Localizer");
    localizerThread = std::thread([this]() {
        while (running) {
            localizer.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void StauRob::setValue(int index, bool value)
{
    if(index < sensors.size() && index > 0)
    {
        sensors[index]->setValue(value);
        logger->info("Value change on {}: {}", sensors[index]->getType(), value);
    }
    else {
        logger->info("Called setValue but did not execute");
    }
}

StauRob::~StauRob() {
    for (auto* sensor : sensors) {
        logger->info("Deleting sensor {}", sensor->getType());
        delete sensor;
    }
}

void StauRob::stop() {
    logger->info("Calling stop of StauRob");
    running = false;
    if (mapperThread.joinable()) mapperThread.join();
    if (localizerThread.joinable()) localizerThread.join();
}

void StauRob::run() {
    std::cout << "StauRob is starting...\n";
    logger->info("Calling run of StauRob");
    running = true;
    SensorEvent sensEvnt;

    // Poll sensors
    for (auto* sensor : sensors) {
        sensEvnt = sensor->read();
        std::cout << sensEvnt.toString() << std::endl;
        if(sensEvnt.type == "Bumper" && sensEvnt.value == 1.0)
        {
            this->stop();
            std::cout << "stopped due to bumper sensor" << std::endl;
        }
            
        if(sensEvnt.type == "Cliff" && sensEvnt.value == 1.0)
        {
            this->stop();
            std::cout << "stopped due to cliff sensor" << std::endl;
        }
        logger->info("Executing sensor polling");
    }

    // Example movement
    motorControl.moveForward(10.0);
    motorControl.rotate(90.0);

    std::cout << "Run complete.\n";
}
