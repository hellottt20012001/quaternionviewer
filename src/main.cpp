#include <stdio.h>
#include <thread>

#include <graphics.h>
#include <tcp.h>
#include <quatview.h>

using namespace std;

void printInfo()
{
	std::cout << "TOP = WHITE\t BOTTOM = YELLOW\n";
	std::cout << "RIGHT = RED\t LEFT = ORANGE\n";
	std::cout << "FRONT = GREEN\t BACK = BLUE\n";
	std::cout << "\n";

	/*std::cout << "Press A/Z to play forward/backward.\n";
	std::cout << "Press S/X to adjust playback speed.\n";*/
	std::cout << "Hold RIGHT mouse button to rotate view.\n";
	std::cout << "Press DOWN = FRONT view.\n";
	std::cout << "Press UP = BACK view.\n";
	std::cout << "Press LEFT = LEFT view.\n";
	std::cout << "Press RIGHT = RIGHT view.\n";
	std::cout << "Press ENTER = TOP view.\n";
	std::cout << "\n";
}

int main(int argc, char* argv[])
{
	std::cout << "##### Quaterinon Viewer\n";
	std::cout << "##### 2016/1/8\n";
	std::cout << "##### by En Shih (seanstone5923@gmail.com)\n\n";

	Graphics graphics;
	if(argc > 1) { if(!graphics.data.readFromCSV(argv[1])) return 0; }
	else { if(!graphics.data.readFromCSV("AS.csv")) return 0; }

	printInfo();

	//TCPserver tcpServer(1234, &graphics);
	//std::thread tcpSeverThread(std::ref(tcpServer));
	//std::thread graphicsThread(std::ref(graphics));
	//graphicsThread.join();
	//tcpServer.stop();
	//tcpSeverThread.join();
	if(graphics.init()) graphics.loop();

    return 0;
}
