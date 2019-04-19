#ifndef _DEFINES_
#define _DEFINES_

#define KEY_ARROW_UP 72
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13
#define KEY_ESCAPE 27
#define ATTR1 0x1A
#define ATTR2 0x1E
#define ATTR3 0x16
#define ATTR4 0x0A

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGTH 31

enum gameStatus
{
	gameActive, gameInactive, gameUpdate, gameStart
};

enum initialPattern
{
	Rpentomino=0, Acorn=1, GameofLife=2, Random=3, Custom=4
};

struct Settings_struct
{
	int heigth, width;
	initialPattern pattern;
};

#endif
