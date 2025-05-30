
# StauRob

This is an example Project developed for Verifysoft Technology GmbH. The goal is to have an easy to understand and fast to edit Project on which static analysis tools can be demonstrated.

**Many iterations of this Project are not ment for execution and will cause errors.**

## Currently Implemented Errors
- `Buffer Overrun:` StauRob.cpp -> StauRob::checkSensors()
- `Data Race:` StepperMotor.cpp -> StepperMotor::update()
- `Deadlock:` StauRob.cpp -> StauRob::mapperThreadFunc() / StauRob::localizerThreadFunc()
- `Naming Style violation:` CliffSensor.h -> Class name not aligned with input naming style
- `Nullpointer dereference:` StauRob.cpp -> StauRob::StauRob()
- `Potential free of nullpointer:` main.cpp -> main()
- `Faulty Third Party Software with entries in the CVE database:` A wrapper lib StaticLib1 uses OpenSSL version 3.0.7. Currently there is no function of this wrapper in use.

