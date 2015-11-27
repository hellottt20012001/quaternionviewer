#include <graphics.h>
#include <quatview.h>

#include <stdio.h>

using namespace std;

Graphics graphics;

void printInfo()
{
	std::cout << "RED = TOP \t CYAN = BOTTOM\n";
	std::cout << "GREEN = RIGHT \t MAGENTA = LEFT\n";
	std::cout << "BLUE = FRONT \t YELLOW = BACK\n\n";

	std::cout << "Press A/Z to move forward/backward between frames.\n";
	std::cout << "Hold right mouse button to rotate view.\n";
	std::cout << "Press ENTER = TOP view.\n";
	std::cout << "Press DOWN = FRONT view.\n";
	std::cout << "Press UP = BACK view.\n";
	std::cout << "Press LEFT = LEFT view.\n";
	std::cout << "Press RIGHT = RIGHT view.\n\n";
}

int readSettings()
{
	std::ifstream settingsStream("QuaternionViewer.ini", std::ios::in);
	if(settingsStream.is_open())
	{
		std::cout << "Settings loaded:\n";
		std::string line;
		while(getline(settingsStream, line))
		{
			std::stringstream lineStream(line);
			std::string key, equal, val;
			std::getline(lineStream, key, '=');
			std::getline(lineStream, val);

			if(key.compare("frameSpeed"))
			{
				float frameSpeed;
				std::stringstream(val) >> frameSpeed;
				graphics.frameSpeed = 1.0f/frameSpeed;
				std::cout << "\tframeSpeed = " << frameSpeed << "\n";
			}
		}
		std::cout << "\n";
		settingsStream.close(); return 1;
	} else return 0;
}

int main()
{
	std::cout << "##### Quaterinon Viewer\n" << "##### 2015/11/28\n" << "##### by En Shih (seanstone5923@gmail.com)\n\n";

	if (graphics.init())
	{
		readSettings();
		printInfo();
		graphics.loop();
	}
	else system("PAUSE");

    return 0;
}
