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
#define num_of_patterns 6

//////TO DO LIST
//gameResponse
//render
//Help                                         
//About

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
		char* pause_menu[10] = {"Continue", "Restart", "Settings", "Help", "About", "Exit"};
		char* default_menu[10] = {"Game", "Settings", "Help", "About", "Exit"};
		char** menu_elements = default_menu;
		
		void (Menu::*default_menu_functions[6])() = {&Menu::Restart, &Menu::Settings, &Menu::Help, &Menu::About, &Menu::Exit};
		void (Menu::*pause_menu_functions[6])() = {&Menu::Game, &Menu::Restart, &Menu::Settings, &Menu::Help, &Menu::About, &Menu::Exit};
		
		void (Menu::**menu_functions)() = default_menu_functions;
		
		
		char setting_elements[num_of_settings_elements][16] = {"Heigth", "Width", "Initial Pattern", "Save", "Cancel"};
		char *pattern_names[num_of_patterns]  = {"R-pentomino", "Acorn", "Game of Life", "Gosper`s glider gun", "Random", "Custom"};
		
		
		int num_of_elements = 5;
		
		int selected_element = 0;
		int *feedback;
		HANDLE h;
		Settings_struct settings = {25, 80, Random};
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
	menu_elements = pause_menu;
	menu_functions = pause_menu_functions;
}



void Menu::mainMenuDraw() 
{
	COORD crd = {CONSOLE_WIDTH-1, CONSOLE_HEIGTH-1};
	SMALL_RECT src = {0, 0, crd.X, crd.Y};
	SetConsoleScreenBufferSize (h, crd);
	SetConsoleWindowInfo (h, TRUE, &src);	
	
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
		printf("%s", menu_elements[i]);
	}
	
	SetConsoleTextAttribute(h, ATTR3);
	for (int i = 0; i < 17; i++)
	{
		cc.X = margin*2+10 + (CONSOLE_WIDTH -(margin*2+10)-27)/2;
		cc.Y = (CONSOLE_HEIGTH-17)/2+i;
		SetConsoleCursorPosition(h, cc);
		printf("%s", logo[i]);
	}
	mainMenuUpdate(0);
//	cc.Y = margin + (selected_element * CONSOLE_HEIGTH / num_of_elements);
//	cc.X = margin*2;
//	SetConsoleTextAttribute(h, ATTR2);
//	SetConsoleCursorPosition(h, cc);
//	printf("%s%s","> ",menu_elements[selected_element]);
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
	printf("%s", menu_elements[prev]);
	
	cc.Y = margin + (selected_element * CONSOLE_HEIGTH / num_of_elements);
	cc.X = margin*2;
	SetConsoleTextAttribute(h, ATTR2);
	SetConsoleCursorPosition(h, cc);
	printf("%c %s",16,menu_elements[selected_element]);
}

void Menu::mainMenuResponse(int Key)
{
	switch (Key)
	{
		case KEY_ESCAPE: {Exit(); break;}
		case KEY_ENTER: 
		{
			(this->*this->menu_functions[selected_element])();
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
	int margin = CONSOLE_HEIGTH / (2*num_of_settings_elements);
	
//	char *outp[num_of_settings_elements][CONSOLE_WIDTH-2*margin];
//	strcat(outp[0])
	
	system("cls");
	
	COORD cc;
	for (int i = 0; i < num_of_settings_elements; i++) 
	{
		if (i == selected_element) SetConsoleTextAttribute(h, ATTR2);
		else SetConsoleTextAttribute(h, ATTR1);
		cc.Y = margin + (i * CONSOLE_HEIGTH / num_of_settings_elements);
		cc.X = margin * 2;
		SetConsoleCursorPosition(h, cc);

		char str[CONSOLE_WIDTH - 2*margin];
		char param[20];
		switch (i)
		{
			case 0: itoa(settings.heigth,param,10); break;
			case 1: itoa(settings.width,param,10); break;
			case 2: strcpy(param,pattern_names[settings.pattern]); break;
			default: param[0] = '\0';
		}
		
		int wid = CONSOLE_WIDTH - 4*margin - strlen(param);
		if ((i==num_of_settings_elements-1)||(i==num_of_settings_elements-2)) 
		{
			if (i==selected_element) sprintf(str,"%c %s",16,setting_elements[i]);
			else sprintf(str,"%s",setting_elements[i]);
		}
		else if (i == selected_element) sprintf(str,"%-*s%c%s%c",wid-2,setting_elements[i],17,param,16);
		else sprintf(str,"%-*s%s",wid-1,setting_elements[i],param);
		cout << str;
	
	}
	
}

void Menu::settingsResponse(int Key)
{
	switch (Key)
	{
		case KEY_ENTER: 
		{
			if ((selected_element!=num_of_settings_elements-1)&&(selected_element!=num_of_settings_elements-2))break;
			if (selected_element==num_of_settings_elements-1) settings = tmp_settings;
		}
		case KEY_ESCAPE: 
		{ 
			selected_element = 0;
			mainMenuDraw();
			response = &Menu::mainMenuResponse;
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
		case KEY_ARROW_RIGHT:
		{
			switch (selected_element)
			{
				case 0: if (settings.heigth<60) settings.heigth+=5;break;
				case 1: settings.width = 10+(settings.width-10+5)%(CONSOLE_WIDTH-5);break;
				case 2: settings.pattern=(settings.pattern+1)%num_of_patterns;
			}
			settingsDraw();
			break;
		}
		case KEY_ARROW_LEFT:
		{
			switch (selected_element)
			{
				case 0: if (settings.heigth>10) settings.heigth-=5;break;
				case 1: settings.width = 10+(settings.width-10-5+(CONSOLE_WIDTH-5))%(CONSOLE_WIDTH-5);break;
				case 2: settings.pattern=(settings.pattern-1+num_of_patterns)%num_of_patterns;
			}
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
	
	if ((Key == KEY_ESCAPE)||(*feedback==gameInactive))
	{
		pauseMenu();
		
		mainMenuDraw();
		response = &Menu::mainMenuResponse;
		*feedback = gameInactive;	
	}
	
	if (*feedback==gamePaused)
	{
		*feedback = gameActive;
	}
	else if (Key == KEY_SPACE)
	{
		*feedback = gamePaused;
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
	/*
	COORD crd = {CONSOLE_WIDTH-1, CONSOLE_HEIGTH-1};
	SMALL_RECT src = {0, 0, crd.X, crd.Y};
	SetConsoleScreenBufferSize (h, crd);
	SetConsoleWindowInfo (h, TRUE, &src);
	*/
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
				src = {0, 0, crd.X, crd.Y+1};
				SetConsoleScreenBufferSize (h, crd);
				SetConsoleWindowInfo (h, TRUE, &src);
				
				field.consoleInitialize();
				flag = gamePaused;
				break;
			case gameActive:
				if (field.refresh()) field.consolePrint();
				else flag = gameInactive;
				break;
			//system("pause");
		}
	}
}
