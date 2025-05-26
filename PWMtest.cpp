#include <pigpio.h>
#include <iostream>
#include <unistd.h>  // for sleep
#include <string>

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed!" << std::endl;
        return 1;
    }

    const int PWM_A = 7; // PWM udgang A (fx: frem)
    const int PWM_B = 8; // PWM udgang B (fx: tilbage)

    gpioSetMode(PWM_A, PI_OUTPUT);
    gpioSetMode(PWM_B, PI_OUTPUT);

    gpioPWM(PWM_A, 0);
    gpioPWM(PWM_B, 0);

    std::string input;

    std::cout << "Gripper styring:\n";
    std::cout << "1 = Åbn (fremad)\n";
    std::cout << "2 = Luk (tilbage)\n";
    std::cout << "3 = Stop\n";

    while (true) {
        std::cout << "Indtast kommando (1/2/3): ";
        std::getline(std::cin, input);

        if (input == "1") {
            std::cout << "➡️  Gripper åbner (fremad)" << std::endl;
            gpioPWM(PWM_A, 0);
            gpioPWM(PWM_B, 50);  // Duty cycle (0–255)
        } else if (input == "2") {
            std::cout << "⬅️  Gripper lukker (tilbage)" << std::endl;
            gpioPWM(PWM_B, 0);
            gpioPWM(PWM_A, 50);
        } else if (input == "3") {
            std::cout << "⏹️  Stopper gripper" << std::endl;
            gpioPWM(PWM_A, 0);
            gpioPWM(PWM_B, 0);
        } else {
            std::cout << "Ugyldig input. Brug 1, 2 eller 3." << std::endl;
        }

        usleep(100000); // 100 ms pause (valgfri)
    }

    gpioTerminate();
    return 0;
}
