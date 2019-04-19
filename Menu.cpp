#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "constants.h"
#include "Field.cpp"


char logo[17][28] = { 
" XXX     XXX  X   X  XXXXX",
"X       X  X  XX XX  X    ",
"X  XX  XXXXX  X X X  XXXXX",
"X   X  X   X  X   X  X    ",
" XXX   X   X  X   X  XXXXX",
"                          ",
"        XXX   XXXXX       ",
"       X   X  X           ",
"       X   X  XXXXX       ",
"       X   X  X           ",
"        XXX   X           ",
"                          ",
"X       XXX   XXXXX  XXXXX",
"X        X    X      X    ",
"X        X    XXXXX  XXXXX",
"X        X    X      X    ",
"XXXXX   XXX   X      XXXXX",};


#define num_of_settings_elements 5

//////TO DO LIST
//gameResponse
//settings
//render
//Help
//About
//fix random

class Menu
{
	public:
		Menu(HANDLE, int*);
		~Menu();
		
		void Restart();
		void Game();
		void Settings();
		void Help();
		void About();
		void Exit();
		void mainMenuDraw();
		void mainMenuUpdate(int prev);
		void mainMenuResponse(int);
		void gameResponse(int);
		void settingsResponse(int);
		void settingsDraw();
		void pauseMenu();
		Settings_struct& getSettings();
		
		void (Menu::*response)(int) = &Menu::mainMenuResponse;
		
		
	private:
		char element_names[6][10] = {"Game", "Settings", "Help", "About", "Exit"};
		char setting_elements[num_of_settings_elements][16] = {"Heigth", "Width", "Initial Pattern", "Save", "Cancel"};
		char pattern_names[5][13] = {"R-pentomino", "Acorn", "Game of Life", "Random", "Custom"};
		
		void (Menu::*element_functions[6])() = {&Menu::Restart, &Menu::Settings, &Menu::Help, &Menu::About, &Menu::Exit};
		int num_of_elements = 5;
		
		int selected_element = 0;
		int *feedback;
		HANDLE h;
		Settings_struct settings = {45, 74, Random};
		Settings_struct tmp_settings;
				
};


Menu::Menu(HANDLE hndl, int *a)
{
	h = hndl;
	feedback = a;
}
Menu::~Menu(){}


void Menu::pauseMenu()
{
	if (6==num_of_elements) return;
	num_of_elements++;
	char tmp_names[6][10] = {"Continue", "Restart", "Settings", "Help", "About", "Exit"};

	
	void (Menu::*tmp_functions[6])() = {&Menu::Game, &Menu::Restart, &Menu::Settings, &Menu::Help, &Menu::About, &Menu::Exit};
	for(int i=0; i<num_of_elements;i++)
	{
		strcpy(element_names[i],tmp_names[i]);
		element_functions[i] = tmp_functions[i];
	}
}


void Menu::mainMenuDraw() 
{
	system("cls");
	SetConsoleTextAttribute(h, ATTR1);
	COORD cc;
	int margin = CONSOLE_HEIGTH / (2*num_of_elements);
	for (int i = 0; i < num_of_elements; i++) 
	{
		if (i == selected_element) continue;
		cc.Y = margin + (i * CONSOLE_HEIGTH / num_of_elements);
		cc.X = margin*2;
		SetConsoleCursorPosition(h, cc);
		printf("%s", element_names[i]);
	}
	
	SetConsoleTextAttribute(h, ATTR3);
	for (int i = 0; i < 17; i++)
	{
		cc.X = 19+14;
		cc.Y = (CONSOLE_HEIGTH-17)/2+i;
		SetConsoleCursorPosition(h, cc);
		printf("%s", logo[i]);
	}
	
	cc.Y = margin + (selected_element * CONSOLE_HEIGTH / num_of_elements);
	cc.X = margin*2;
	SetConsoleTextAttribute(h, ATTR2);
	SetConsoleCursorPosition(h, cc);
	printf("%s%s","> ",element_names[selected_element]);
};


void Menu::mainMenuUpdate(int prev)
{
	SetConsoleTextAttribute(h, ATTR1);
	COORD cc;
	prev = (num_of_elements+selected_element+prev) % (num_of_elements);
	int margin = CONSOLE_HEIGTH / (2*num_of_elements);
	
	cc.Y = margin + (prev * CONSOLE_HEIGTH / num_of_elements);
	cc.X = margin*2;
	SetConsoleTextAttribute(h, ATTR1);
	SetConsoleCursorPosition(h, cc);
	printf("%s", "               ");
	SetConsoleCursorPosition(h, cc);
	printf("%s", element_names[prev]);
	
	cc.Y = margin + (selected_element * CONSOLE_HEIGTH / num_of_elements);
	cc.X = margin*2;
	SetConsoleTextAttribute(h, ATTR2);
	SetConsoleCursorPosition(h, cc);
	printf("%s%s","> ",element_names[selected_element]);
}

void Menu::mainMenuResponse(int Key)
{
	switch (Key)
	{
		case KEY_ESCAPE: {Exit(); break;}
		case KEY_ENTER: 
		{
			(this->*this->element_functions[selected_element])();
			break;
		}
				
		case KEY_ARROW_DOWN:
		case KEY_ARROW_RIGHT:
		{
			if (selected_element == num_of_elements-1) selected_element = 0;
			else selected_element++;
			mainMenuUpdate(-1);
			break; 
		}
				
		case KEY_ARROW_UP:
		case KEY_ARROW_LEFT:
		{
			if (selected_element == 0) selected_element = num_of_elements-1;
			else selected_element--;
			mainMenuUpdate(1);
			break;
		}
	}	
}



void Menu::settingsDraw()
{
	
	system("cls");
	SetConsoleTextAttribute(h, ATTR1);
	COORD cc;
	int margin = CONSOLE_HEIGTH / (2*num_of_settings_elements);
	for (int i = 0; i < num_of_settings_elements; i++) 
	{
		if (i == selected_element) continue;
		cc.Y = margin + (i * CONSOLE_HEIGTH / num_of_settings_elements);
		cc.X = margin * 2;
		SetConsoleCursorPosition(h, cc);
		printf("%s", setting_elements[i]);
		
	}

	cc.Y = margin + (selected_element * CONSOLE_HEIGTH / num_of_settings_elements);
	cc.X = margin*2;
	SetConsoleTextAttribute(h, ATTR2);
	SetConsoleCursorPosition(h, cc);
	printf("%s",setting_elements[selected_element]);
	
	
}

void Menu::settingsResponse(int Key)
{
	switch (Key)
	{
		case KEY_ENTER: 
		{
			if ((selected_element!=num_of_settings_elements-1)&&(selected_element!=num_of_settings_elements-2))break;
		}
		case KEY_ESCAPE: 
		{ 
			selected_element = 0;
			mainMenuDraw();
			response = &Menu::mainMenuResponse;
			if (selected_element!=num_of_settings_elements-1) break;
			settings = tmp_settings;
			break;
		}
		
		case KEY_ARROW_DOWN:
		{
			if (selected_element == num_of_settings_elements-1) selected_element = 0;
			else selected_element++;
			settingsDraw();
			break; 
		}
		case KEY_ARROW_UP:
		{
			if (selected_element == 0) selected_element = num_of_settings_elements-1;
			else selected_element--;
			settingsDraw();
			break;
		}
	}
}


void Menu::Restart()
{
	SetConsoleTextAttribute(h, ATTR4);
	system("cls");
	response = &Menu::gameResponse;
	*feedback = gameUpdate;
}

void Menu::Game() 
{
	SetConsoleTextAttribute(h, ATTR4);
	system("cls");
	response = &Menu::gameResponse;
	*feedback = gameStart;
};
void Menu::Help() 
{
	system("cls");
	printf("Help");
	system("pause");
	mainMenuDraw();
};
void Menu::About() 
{
	system("cls");
	printf("About");
	system("pause");
	mainMenuDraw();
};

void Menu::Settings() 
{
	tmp_settings = settings;
	settingsDraw();
	response = &Menu::settingsResponse;
};

void Menu::Exit()
{
	exit(0);
}


void Menu::gameResponse(int Key)
{
	if (*feedback!=gameActive)
	{
		*feedback = gameActive;
	}
	switch (Key)
	{
		case KEY_ESCAPE:
			
			COORD crd = {CONSOLE_WIDTH-1, CONSOLE_HEIGTH-1};
			SMALL_RECT src = {0, 0, crd.X, crd.Y};
			SetConsoleScreenBufferSize (h, crd);
			SetConsoleWindowInfo (h, TRUE, &src);
			
			pauseMenu();
			SetConsoleTextAttribute(h, ATTR2);
			system("cls");
			mainMenuDraw();
			response = &Menu::mainMenuResponse;
			*feedback = gameInactive;	
	}
}

Settings_struct& Menu::getSettings()
{
	return settings;
}


HANDLE setup()
{
	srand(time(0));
	SetConsoleTitle("Game of Life");
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(h,&structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo( h, &structCursorInfo );
	
	SetConsoleTextAttribute(h, ATTR2);
	
	COORD crd = {CONSOLE_WIDTH-1, CONSOLE_HEIGTH-1};
	SMALL_RECT src = {0, 0, crd.X, crd.Y};
	SetConsoleScreenBufferSize (h, crd);
	SetConsoleWindowInfo (h, TRUE, &src);
	return h;
}


int main()
{
	SMALL_RECT src;
	COORD crd;
	int flag = gameInactive;
	HANDLE h = setup();
	Menu menu(h, &flag);
	Field field(h,menu.getSettings());

	menu.mainMenuDraw();
	while (1)
	{
		if (_kbhit())
		{
			(menu.*menu.response)(_getch());
		}
		
		switch (flag)
		{	
		
			case gameUpdate:
				field.setSettings(menu.getSettings());
			case gameStart:
				
				crd = {CONSOLE_WIDTH-1, menu.getSettings().heigth};
				src = {0, 0, crd.X, crd.Y};
				SetConsoleScreenBufferSize (h, crd);
				SetConsoleWindowInfo (h, TRUE, &src);
				
				field.consolePrint();
				flag = gameInactive;
				break;
			case gameActive:
				if (field.refresh()) field.consolePrint();
				else flag = gameInactive;
				break;
			//system("pause");
		}
	}
}
