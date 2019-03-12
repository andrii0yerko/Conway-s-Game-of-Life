#include "Conwaysgameoflife.cpp"
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
int main()
{
	Field field(25,40);
	field.initialize();
	/*
	field.consolePrint();
	getch();
	while (1)
	{
		system("cls");
		field.refresh();
		field.consolePrint();
		Sleep(50);
	}
	*/
	for(int i=0; i<1000; i++)
	{
		field.refresh();
		field.consolePrint();
		system("cls");
	}
}
