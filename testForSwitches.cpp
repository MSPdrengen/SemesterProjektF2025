#include <pigpio.h>
#include <iostream>
#include <unistd.h>  // for sleep

int main() {
    // Initialize the pigpio library
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed!" << std::endl;
        return 1;
    }

    //const int PWM_GPIO = 13;
    const int GPIO_24 = 24;
    const int GPIO_26 = 26;
    const int GPIO_8 = 8;
    const int GPIO_7 = 7;

    // Set GPIOs to input mode
    gpioSetMode(GPIO_24, PI_INPUT); //åben gripper
    gpioSetMode(GPIO_26, PI_INPUT); // lukket gripper
    
    // Set GPIOs to output mode
    gpioSetMode(GPIO_8, PI_OUTPUT);
    gpioSetMode(GPIO_7, PI_OUTPUT);

    while (true) {
        int val24 = gpioRead(GPIO_24);
        int val26 = gpioRead(GPIO_26);
        int direction = 0;

	gpioPWM(12, 0);
	gpioPWM(13, 0);
    
    int modBusRecieve;
    int modBusSend;
	std::cin >> direction;
	//modBusRecive  = 0, åbner gripper
	//modBusRecive = 1, lukker gripper
	if (direction == 0){
        //std::cout << "åbner gripper" << std::endl;
	    gpioPWM(GPIO_8, 0);
	    gpioPWM(GPIO_7, 50);
	} else if (direction == 1){
	    //std::cout<< "lukke gripper" << std::endl;
	    gpioPWM(GPIO_8, 50);
	    gpioPWM(GPIO_7, 0);
	}
    }


    gpioTerminate();  // Never reached in this loop, but good practice
    return 0;
}
