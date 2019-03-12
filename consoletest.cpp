#include "Conwaysgameoflife.cpp"
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
int main()
{
	Field field(25,25);
	while (1)
	{
		system("cls");
		field.consolePrint();
		field.refresh();
		Sleep(100);
		//getch();
	}
}
