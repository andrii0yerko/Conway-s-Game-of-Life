#include "Conwaysgameoflife.cpp"
//#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
int main()
{
	
	HANDLE consoleHandle = 0;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	Settings_struct set;
	set.heigth = 21;
	set.width = 72;
	
	Field field(consoleHandle,set);
	field.initialize();
	field.consolePrint();

	int Key = getch();
	while (1)
	{
		if (Key!=27)
		{
			field.refresh();
			field.consolePrint();
		}
		
		if (_kbhit()) Key = getch();
	}
	
	return 1;
}
