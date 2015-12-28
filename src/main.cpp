#include <stdio.h>

#include <graphics.h>
#include <quatview.h>

using namespace std;

Graphics graphics;

void printInfo()
{
	std::cout << "TOP = WHITE\t BOTTOM = YELLOW\n";
	std::cout << "RIGHT = RED\t LEFT = ORANGE\n";
	std::cout << "FRONT = GREEN\t BACK = BLUE\n";
	std::cout << "\n";

	std::cout << "Press A/Z to play forward/backward.\n";
	std::cout << "Press S/X to adjust playback speed.\n";
	std::cout << "Hold RIGHT mouse button to rotate view.\n";
	std::cout << "Press DOWN = FRONT view.\n";
	std::cout << "Press UP = BACK view.\n";
	std::cout << "Press LEFT = LEFT view.\n";
	std::cout << "Press RIGHT = RIGHT view.\n";
	std::cout << "Press ENTER = TOP view.\n";
	std::cout << "\n";
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
				graphics.frameSpeed = frameSpeed;
				std::cout << "\tframeSpeed = " << frameSpeed << "\n";
			}
		}
		std::cout << "\n";
		settingsStream.close(); return 1;
	} else return 0;
}

int main(int argc, char* argv[])
{
	std::cout << "##### Quaterinon Viewer\n" << "##### 2015/12/28\n" << "##### by En Shih (seanstone5923@gmail.com)\n\n";

	if(argc > 1) { if(!graphics.data.readFromCSV(argv[1])) return 0; }
	else { if(!graphics.data.readFromCSV("AS.csv")) return 0; }

	if(graphics.init())
	{
		//readSettings();
		printInfo();
		graphics.loop();
	}
	else system("PAUSE");

    return 0;
}
