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
// custom initial pattern

class Menu
{
	public:
		Menu(HANDLE, int*);
		~Menu();
		
		void Restart();
		void Game();
		void gameResponse(int);
		
		void Help();
		void helpDraw();
		void helpResponse(int);
		
		void About();
		void Exit();
		void mainMenuDraw();
		void mainMenuUpdate(int prev);
		void mainMenuResponse(int);
		
		void Settings();
		void settingsResponse(int);
		void settingsDraw();
		
		void pauseMenu();
		void unpauseMenu();
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
		char *pattern_names[num_of_patterns]  = {"R-pentomino", "Acorn", "Die Hard", "Gosper`s glider gun", "Random", "Custom"};
		
		
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
	
	FILE* f;
	if(f=fopen("settingsfile","r"))
	{
		fscanf(f,"%d%d%d",&settings.height,&settings.width,&settings.pattern);
	}
	
//	if ( settings.height > (CONSOLE_HEIGHT/5 -1)*5) settings.height -= settings.height - (CONSOLE_HEIGHT/5 -1)*5;
//	if ( settings.width > (CONSOLE_WIDTH/5-1)*5) settings.width  -= settings.width - (CONSOLE_WIDTH/5-1)*5;
}
Menu::~Menu(){}


void Menu::pauseMenu()
{
	if (6==num_of_elements) return;
	num_of_elements++;
	menu_elements = pause_menu;
	menu_functions = pause_menu_functions;
}

void Menu::unpauseMenu()
{
	num_of_elements = 5;
	menu_elements = default_menu;
	menu_functions = default_menu_functions;
}

void Menu::mainMenuDraw() 
{
	/*
	COORD crd = {CONSOLE_WIDTH-1, CONSOLE_HEIGTH-1};
	SMALL_RECT src = {0, 0, crd.X, crd.Y};
	SetConsoleScreenBufferSize (h, crd);
	SetConsoleWindowInfo (h, TRUE, &src);	
	*/
	
	system("cls");
	SetConsoleTextAttribute(h, ATTR1);
	COORD cc;
	int margin = CONSOLE_HEIGHT / (2*num_of_elements);
	for (int i = 0; i < num_of_elements; i++) 
	{
		if (i == selected_element) continue;
		cc.Y = margin + (i * CONSOLE_HEIGHT / num_of_elements);
		cc.X = margin*2;
		SetConsoleCursorPosition(h, cc);
		printf("%s", menu_elements[i]);
	}
	
	SetConsoleTextAttribute(h, ATTR3);
	for (int i = 0; i < 17; i++)
	{
		cc.X = margin*2+10 + (CONSOLE_WIDTH -(margin*2+10)-27)/2;
		cc.Y = (CONSOLE_HEIGHT-17)/2+i;
		SetConsoleCursorPosition(h, cc);
		printf("%s", logo[i]);
	}
	mainMenuUpdate(0);
};

void Menu::mainMenuUpdate(int prev)
{
	SetConsoleTextAttribute(h, ATTR1);
	COORD cc;
	prev = (num_of_elements+selected_element+prev) % (num_of_elements);
	int margin = CONSOLE_HEIGHT / (2*num_of_elements);
	
	cc.Y = margin + (prev * CONSOLE_HEIGHT / num_of_elements);
	cc.X = margin*2;
	SetConsoleTextAttribute(h, ATTR1);
	SetConsoleCursorPosition(h, cc);
	printf("%s", "               ");
	SetConsoleCursorPosition(h, cc);
	printf("%s", menu_elements[prev]);
	
	cc.Y = margin + (selected_element * CONSOLE_HEIGHT / num_of_elements);
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
		case SCREEN_UPDATE:
		{
			mainMenuDraw();
			break;
		}
	}	
}


void Menu::Settings() 
{
	tmp_settings = settings;
	settingsDraw();
	response = &Menu::settingsResponse;
};

void Menu::settingsDraw()
{
	int margin = CONSOLE_HEIGHT / (2*num_of_settings_elements);
	
	system("cls");
	COORD cc;
	for (int i = 0; i < num_of_settings_elements; i++) 
	{
		if (i == selected_element) SetConsoleTextAttribute(h, ATTR2);
		else SetConsoleTextAttribute(h, ATTR1);
		cc.Y = margin + (i * CONSOLE_HEIGHT / num_of_settings_elements);
		cc.X = margin * 2;
		SetConsoleCursorPosition(h, cc);

		char str[CONSOLE_WIDTH - 2*margin];
		char param[20];
		switch (i)
		{
			case 0: itoa(settings.height,param,10); break;
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
			if (selected_element==num_of_settings_elements-2)
			{
				FILE* f;
				if(f=fopen("settingsfile","w"))
				{
					fprintf(f,"%d %d %d",settings.height,settings.width,settings.pattern);
					fclose(f);
				}
			}
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
				case 0: settings.height  = 10+(settings.height-10+5)%(((CONSOLE_HEIGHT-2)/5 -1)*5);break;
				case 1: settings.width   = 10+(settings.width-10+5)%((CONSOLE_WIDTH/5-1)*5);break;
				case 2: settings.pattern = (settings.pattern+1)%num_of_patterns;
			}
			settingsDraw();
			break;
		}
		case KEY_ARROW_LEFT:
		{
			switch (selected_element)
			{
				case 0: settings.height  = 10+(settings.height-10-5+((CONSOLE_HEIGHT-2)/5 -1)*5)%( ((CONSOLE_HEIGHT-2)/5 -1)*5);break;
				case 1: settings.width   = 10+(settings.width-10-5+(CONSOLE_WIDTH/5-1)*5)%( (CONSOLE_WIDTH/5-1)*5);break;
				case 2: settings.pattern = (settings.pattern-1+num_of_patterns)%num_of_patterns;
			}
			settingsDraw();
			break;
		}	
		case SCREEN_UPDATE:
		{
			if ( settings.height > (CONSOLE_HEIGHT/5 -1)*5) settings.height -= settings.height - (CONSOLE_HEIGHT/5 -1)*5;
			if ( settings.width > (CONSOLE_WIDTH/5-1)*5) settings.width  -= settings.width - (CONSOLE_WIDTH/5-1)*5;
			settingsDraw();
			break;
		}
	}
}


void Menu::Restart()
{
	response = &Menu::gameResponse;
	*feedback = gameUpdate;
}

void Menu::Game() 
{
	response = &Menu::gameResponse;
	*feedback = gameStart;
};

void Menu::gameResponse(int Key)
{
	if (toupper(Key) == 'R')
	{
		*feedback = gameUpdate;
		return;
	}
	
	if (toupper(Key) == 'E')
	{
		*feedback = gameFilling;
		return;
	}
	
	if (Key == SCREEN_UPDATE)
	{
		*feedback = gameStart;	
		return;
	}
	
	if ((Key==224)||(Key==0)) return;
	
	if ((Key == KEY_ESCAPE)||((*feedback==gameInactive)&&(Key!=KEY_ARROW_RIGHT)))
	{
		if(*feedback==gameInactive) unpauseMenu();
		else pauseMenu();
		mainMenuDraw();
		response = &Menu::mainMenuResponse;
		*feedback = gameInactive;	
	}
	
	if (Key == KEY_SPACE)
	{
		if (*feedback==gamePaused) *feedback = gameActive;
		else *feedback = gamePaused;
	}

	if ((Key==KEY_ARROW_RIGHT)&&(*feedback==gameActive))
	{
		*feedback = gameSkip;
	}
}


void Menu::Help() 
{
	selected_element = 0;
	system("cls");
	helpDraw();
	response = &Menu::helpResponse;
};

void Menu::helpDraw()
{
	
	char *text[][150]={
	{
		"The Game of Life is a cellular automaton devised by the British mathematician John Horton Conway in 1970.",
		"",
		"The game is a zero-player game, meaning that its evolution is determined by its initial state,",
		"requiring no further input. One interacts with the Game of Life by creating an initial configuration",
		"and observing how it evolves, or, for advanced players, by creating patterns with particular properties.",
		"",
		"The Game of Life is a two-dimensional orthogonal grid of cells, each of which is in one of two possible states, alive or dead.",
		"Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent.", 
		"",
		"At each step in time, the following transitions occur:",
		"1.Any live cell with fewer than two live neighbours dies, as if by underpopulation.",
		"2.Any live cell with two or three live neighbours lives on to the next generation.",
		"3.Any live cell with more than three live neighbours dies, as if by overpopulation.",
		"4.Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction."
	},
	{
		"Controls are simple",
		"",
		"In-game controls:",
		"SPACE       - pause/unpause",
		"RIGHT ARROW - speed up",
		"ESCAPE      - main menu",
		"R           - restart",
		"E           - edit initial generation",
		"",
		"In filling mode",
		"X           - change cell state",
		"BACKSPACE   - ",
		"",
		""
	}};
	
	
	int m = CONSOLE_HEIGHT / (2*num_of_elements);
	
	for(int i = 0; i<2; i++)
	{
		if (selected_element==i) SetConsoleTextAttribute(h, ATTR2);
		else SetConsoleTextAttribute(h,ATTR1);
		if(i==0)
		{
			SetConsoleCursorPosition(h,{CONSOLE_WIDTH/4-strlen("Game rules")/2,m});
			cout << "Game rules";
		}
		else
		{
			SetConsoleCursorPosition(h,{3*CONSOLE_WIDTH/4-strlen("Controls")/2,m});
			cout << "Controls";
		}
	}
	
	COORD cc = {0,m+2};
	SetConsoleTextAttribute(h,ATTR1);
	SetConsoleCursorPosition(h,cc);
	for(int i = 0; i<CONSOLE_WIDTH; i++)
	{
		printf ("%c",461);
	}
	
	cc.Y+=2;
	cc.X=m;
	for(int i = 0; i<14; i++)
	{
		SetConsoleCursorPosition(h,cc);
		printf("%-150s",text[selected_element][i]);
		cc.Y++;
	}
	
	
	
}

void Menu::helpResponse(int Key)
{
	switch(Key)
	{
		case KEY_ESCAPE:
			selected_element = 0;
			mainMenuDraw();
			response = &Menu::mainMenuResponse;
			break;
		case KEY_ARROW_UP:
		case KEY_ARROW_DOWN:
		case KEY_ARROW_LEFT:
		case KEY_ARROW_RIGHT:
			selected_element = (selected_element+1)%2;
			helpDraw();break;
		case SCREEN_UPDATE:
			system("cls");
			helpDraw();break;
	}
}

void Menu::About() 
{
	static char*about[4]=
	{
		"Conway's game of life",
		"Made by IASA student",
		"Andrii Yerko",
		"2019"
	};
	system("cls");
	
	COORD cc;
	cc.Y = (CONSOLE_HEIGHT - 4)/2;
	
	SetConsoleTextAttribute(h, ATTR2);
	for(int i=0; i<4; i++)
	{
		cc.X = (CONSOLE_WIDTH - strlen(about[i]))/2;
		SetConsoleCursorPosition(h, cc);
		cout << about[i];
		cc.Y++;
	}
	
	getch();
	mainMenuDraw();
};



void Menu::Exit()
{
	exit(0);
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

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SetConsoleScreenBufferSize (h, {GetSystemMetrics(SM_CXMAXIMIZED), GetSystemMetrics(SM_CYMAXIMIZED)});
	GetConsoleScreenBufferInfo(h, &csbi);
	SetConsoleScreenBufferSize (h, csbi.dwMaximumWindowSize);
	
	SetConsoleTextAttribute(h, ATTR2);
	/*
	COORD crd = {CONSOLE_WIDTH-1, CONSOLE_HEIGTH-1};
	SMALL_RECT src = {0, 0, crd.X, crd.Y};
	SetConsoleScreenBufferSize (h, crd);
	SetConsoleWindowInfo (h, TRUE, &src);
	*/
	return h;
}


bool ScreenChanges(HANDLE h)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

   	GetConsoleScreenBufferInfo(h, &csbi);
    columns        = CONSOLE_WIDTH;
    rows           = CONSOLE_HEIGHT;
   	CONSOLE_WIDTH  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    CONSOLE_HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
   	
    if (CONSOLE_WIDTH<MIN_CONSOLE_WIDTH)
	{
		CONSOLE_WIDTH = MIN_CONSOLE_WIDTH;
		SMALL_RECT r = {0,0,CONSOLE_WIDTH-1,CONSOLE_HEIGHT-1};
		SetConsoleWindowInfo (h, TRUE, &r);
	} 
	if (CONSOLE_HEIGHT<MIN_CONSOLE_HEIGHT) 
	{
		CONSOLE_HEIGHT = MIN_CONSOLE_HEIGHT;
		SMALL_RECT r = {0,0,CONSOLE_WIDTH-1,CONSOLE_HEIGHT-1};
		SetConsoleWindowInfo (h, TRUE, &r);
	}
	
	return ((columns!=CONSOLE_WIDTH)||(rows!=CONSOLE_HEIGHT)) ;
}



void filling(HANDLE h,Field &field, Menu &menu)
{
	int shift=0;
	COORD corner={(CONSOLE_WIDTH - menu.getSettings().width)/2, (CONSOLE_HEIGHT - menu.getSettings().height)/2};
	COORD cc ={0,0};
	field.consolePrint("Set first generation then press enter");
	
	while(1) if(kbhit())
	{
		SetConsoleTextAttribute(h,ATTR1);
		SetConsoleCursorPosition(h,{cc.X+corner.X,cc.Y+corner.Y});
		if (field.getCell(cc.X,cc.Y)) cout << "X";
		else cout << " ";
		char Key = getch();
		switch(Key)
		{
			case KEY_ARROW_DOWN:
				cc.Y = (cc.Y+1)%menu.getSettings().height; 
				if (shift) field.setCell(cc.X,cc.Y);       break;
			case KEY_ARROW_UP:
				cc.Y = (cc.Y-1+menu.getSettings().height)%menu.getSettings().height;
				if (shift) field.setCell(cc.X,cc.Y); break;
			case KEY_ARROW_RIGHT:
				cc.X = (cc.X+1)%menu.getSettings().width;
				if (shift) field.setCell(cc.X,cc.Y);		   break;
			case KEY_ARROW_LEFT:
				cc.X = (cc.X-1+menu.getSettings().width)%menu.getSettings().width; 
				if (shift) field.setCell(cc.X,cc.Y); break;
			case KEY_SHIFT: shift = !shift; break;
			case 'X':
			case 'x':
				field.setCell(cc.X,cc.Y);
				break;
			case KEY_ENTER:
				field.initialize();
				return;
			case KEY_ESCAPE: menu.gameResponse(KEY_ESCAPE); return;
		}
		SetConsoleTextAttribute(h,ATTR5);
		SetConsoleCursorPosition(h,{cc.X+corner.X,cc.Y+corner.Y});
		if (field.getCell(cc.X,cc.Y)) cout << "X";
		else cout << " ";
	}
	
}


int main()
{

	SMALL_RECT src;
	COORD crd, corner,cc;
    int columns, rows;
    int lasttime;
	HANDLE h = setup();
	
	
	int flag = gameInactive;

	Menu menu(h, &flag);
	Field field(h,menu.getSettings());

	menu.mainMenuDraw();
	while (1)
	{
    	
    	if (ScreenChanges(h)) 
		{
			(menu.*menu.response)(SCREEN_UPDATE);
		}
		
		
		if (_kbhit())
		{
			(menu.*menu.response)(getch());
		}
		
		switch (flag)
		{	
			case gameFilling:
				if (field.getTurn()==0)
				{
					flag = gamePaused;
					filling(h,field,menu);
					field.consolePrint("Press space to start   ");
				}
				else flag = gamePaused;
				break;
				
			case gameUpdate:
				field.setSettings(menu.getSettings());
				if (menu.getSettings().pattern==Custom) flag = gameFilling;
				else 									flag = gameStart; 
				
			case gameStart:
				
				//if  (menu.getSettings().height > ((CONSOLE_HEIGHT/5 -1)*5)) crd.Y = menu.getSettings().height+2;
				//if  (menu.getSettings().width > ((CONSOLE_WIDTH/5 -1)*5)) crd.X = menu.getSettings().width;
				//src = {0, 0, crd.X-1, crd.Y-1};
				//SetConsoleWindowInfo (h, TRUE, &src);
				
				SetConsoleTextAttribute(h, ATTR4);
				system("cls");
				field.consoleInitialize("Press space to start");
				if (flag==gameStart) flag = gamePaused;
				lasttime = clock();
				break;
			
			case gameActive:
				if (clock() - lasttime < 50) break;
				lasttime = clock();
				if (field.refresh()) field.consolePrint();
				else 
				{
					flag = gameInactive;
					field.consolePrint("Game over! Press any key");
				}
				break;
				
			case gameSkip:
				for(int i=0;i<4;i++)
					if (!field.refresh()) 
					{
						flag = gameInactive;
						field.consolePrint("Game over! Press any key");
						break;
					}
				if (flag==gameSkip) flag = gameActive;
				break;
		}
	}
}
