#include <string>
#include "upm/jhd1313m1.hpp"
#include "mraa.hpp"
#include <signal.h>
#include <unistd.h>

#define I2C_BUS  0
bool running = true;
using namespace std;
upm::Jhd1313m1* lcd;


void sig_handler(int signo)
{
    if (signo == SIGINT)
        running = false;
}

int main(int argc, char* argv[])
{

	lcd = new upm::Jhd1313m1(I2C_BUS, 0x3e, 0x62);
	mraa::Gpio* touch_gpio = new mraa::Gpio(29);
	mraa::Result response;
	int touch;
	int last_touch = 0;
	signal(SIGINT, sig_handler);

	response = touch_gpio->dir(mraa::DIR_IN);
	if (response != mraa::SUCCESS)
			return 1;

	bool changeLane = true;
	int bombLane = 0;
	int lane = 0;
	int time = 0;

	while (true) {
		time++;
		bombLane = rand() % 2;
		if(time>100) break;
		for(int i = 14; i>=0; i--){
			touch = touch_gpio->read();
			if (touch == 1 && last_touch == 0) {
				if(changeLane){
					lane = 1;
					 changeLane = false;
				}
				else{
					lane = 0;
					changeLane = true;
				}
			}
			lcd->clear();
			lcd->setCursor(bombLane,i);
			if(bombLane == lane && i == 0){
				lcd->clear();
				lcd->setColor(0xff,0x00,0x00);
				lcd->setCursor(0,0);
				lcd->write("Sorry, You Lost");
				lcd->setCursor(1,0);
				lcd->write("SCR: ");
				lcd->setCursor(1,5);
				lcd->write(std::to_string(time));
				sleep(2);
				return response;
			}
			lcd->write("O");
			lcd->setCursor(lane,0);
			lcd->write(">");

			usleep(100000);
			last_touch = touch;
		}
	}
	delete lcd;
	delete touch_gpio;
	return response;
}
