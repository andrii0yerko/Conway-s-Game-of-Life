#ifndef _DEFINES_
#define _DEFINES_

#define KEY_ARROW_UP 72
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13
#define KEY_ESCAPE 27
#define KEY_SPACE 32

#define ATTR1 0x1A
#define ATTR2 0x1E
#define ATTR3 0x16
#define ATTR4 0x0A

#define SCREEN_UPDATE -1

int MIN_CONSOLE_WIDTH = 60;
int MIN_CONSOLE_HEIGHT = 25;


int CONSOLE_WIDTH = 80;
int CONSOLE_HEIGHT = 31;

enum gameStatus
{
	gameActive, gameInactive, gameUpdate, gameStart, gamePaused, gameSkip
};

enum initialPattern
{
	Rpentomino=0, Acorn=1, GameofLife=2, Glidergun=3, Random=4, Custom=5
};

struct Settings_struct
{
	int height, width;
	int pattern;
};

#endif
