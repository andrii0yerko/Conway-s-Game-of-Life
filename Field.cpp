#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include "constants.h"
using namespace std;

class Menu;

class Field
{
	private:
		HANDLE h;
		int turnNumber = 0;
		int height;
		int width;
		int **turn, **buffer;
		int **status, **nextstatus;
		int **consoleBuffer;
		
		int checkNeighbors(int, int);
		void neighborsstat(int i, int j, int**);
		
		
		int** createEmptyArray();
		void arrcpy(int**,int**);
		void del(int**);
		
	public:
		Field& setSettings(Settings_struct);
		Field(HANDLE, Settings_struct);
		~Field();
		void consoleInitialize(char*);
		void consolePrint(char*);
		bool refresh();
		Field& setCell(int,int);
		int getCell(int,int);
		int getTurn(){return turnNumber;}
		void initialize();
};

void Field::arrcpy(int** a,int** b)
{
	for (int i=0; i < this->width; i++)
		for (int j=0; j < this->height; j++)
		a[i][j] = b[i][j];
}

Field::Field(HANDLE hndl, Settings_struct set)
{
	h = hndl;
	setSettings(set);
}

Field& Field::setSettings(Settings_struct set)
{
	height     = set.height;
	width      = set.width;
	
	turnNumber = 0;
	
	turn       = createEmptyArray();
	
	
	if (set.pattern==Random)
	{
		for (int i=0; i < this->width; i++)
			for (int j=0; j < this->height; j++)
			turn[i][j] = rand() % 2;
	}
	else if (set.pattern == Rpentomino)		
	{
		int i = (width - 3)/2;
		int j = (height - 3)/2;
		turn[i+1][j] = turn[i+2][j] = turn[i][j+1] = turn [i+1][j+1] = turn[i+1][j+2] = 1;
	}
	else if (set.pattern == Acorn)		
	{
		int i = (width - 7)/2;
		int j = (height - 3)/2;
		turn[i+1][j] = turn[i+3][j+1] = turn[i][j+2] = turn[i+1][j+2] = turn[i+4][j+2] = turn[i+5][j+2] = turn[i+6][j+2] = 1;
	}
	else if (set.pattern == Glidergun)
	{
		int i = (width - 36)/2;
		int j = (height - 9)/2;
		turn[i+24][j]=turn[i+22][j+1]=turn[i+24][j+1]
		=turn[i+12][j+2]=turn[i+13][j+2]=turn[i+20][j+2]=turn[i+21][j+2]=turn[i+34][j+2]=turn[i+35][j+2]
		=turn[i+11][j+3]=turn[i+15][j+3]=turn[i+20][j+3]=turn[i+21][j+3]=turn[i+34][j+3]=turn[i+35][j+3]
		=turn[i][j+4]=turn[i+1][j+4]=turn[i+10][j+4]=turn[i+16][j+4]=turn[i+20][j+4]=turn[i+21][j+4]
		=turn[i][j+5]=turn[i+1][j+5]=turn[i+10][j+5]=turn[i+14][j+5]=turn[i+16][j+5]=turn[i+17][j+5]=turn[i+22][j+5]=turn[i+24][j+5]
		=turn[i+10][j+6]=turn[i+16][j+6]=turn[i+24][j+6]
		=turn[i+11][j+7]=turn[i+15][j+7]
		=turn[i+12][j+8]=turn[i+13][j+8]
		=1;	
	}
	else if (set.pattern == DieHard)
	{
		int i = (width - 8)/2;
		int j = (height - 3)/2;
		turn[i+6][j]=turn[i][j+1]=turn[i+1][j+1]=turn[i+1][j+2]=turn[i+5][j+2]=turn[i+6][j+2]=turn[i+7][j+2]=1;
	}
	
	initialize();
	return *this;
}

void Field::initialize()
{
	status     = createEmptyArray();
	nextstatus = createEmptyArray();
	
	for (int i=0; i < this->width; i++)
	{
		for (int j=0; j < this->height; j++)
		{
			if (turn[i][j])
			{
				neighborsstat(i,j,status);
			}
		}
	}
	
	buffer     = createEmptyArray();
	arrcpy(buffer,turn);
	consoleBuffer     = createEmptyArray();
	arrcpy(consoleBuffer,turn);
}

Field::~Field()
{
	del(turn);
	del(status);
	del(nextstatus);
}

int** Field::createEmptyArray()
{
	int** arr = new int*[this->width];
	for (int i=0; i < this->width; i++)
	{
		arr[i] = new int[this->height];
		for (int j=0; j < this->height; j++)
		{
			arr[i][j] = 0;
		}
	}
	return arr;
}

void Field::del(int**arr)
{
	for (int i=0; i < this->width; i++)
	{
		delete [] arr[i];
	}
	
	delete [] arr;
}

bool Field::refresh()
{
	nextstatus = createEmptyArray();
	for (int i=0; i < this->width; i++)
	{
		for (int j=0; j < this->height; j++)
		{
			if ((status[i][j]==3)||(status[i][j]==2 && turn[i][j]))
			{
				turn[i][j]=1;
				neighborsstat(i,j,nextstatus);
			}
			else turn[i][j]=0;
		}
	}
	del(status);
	status = nextstatus;
	turnNumber++;
	
	if (turnNumber % 10)
	{
		for (int i=0; i < this->width; i++) 
			for (int j=0; j < this->height; j++)
				if (buffer[i][j]!=turn[i][j])
				{
					arrcpy(buffer,turn);
					return true;
				}
		return false;
	}
	
	return true;
}



void Field::neighborsstat(int i, int j, int** st)
{	
	for (int n=-1; n <= 1; n++)
	{
		for (int m=-1; m<=1; m++)
		{
			st[(i-n+this->width)%this->width][(j-m+this->height)%this->height]++;
		}
	}
	st[i][j]--;
}


Field& Field::setCell(int i, int j)
{
	turn[i][j] = !turn[i][j];
	return*this;
}

int Field::getCell(int i, int j)
{
	return turn[i][j];
}

void Field::consoleInitialize(char* message=NULL)
{
	SetConsoleTextAttribute(h, ATTR4);
	system("cls");
	
	COORD corner={(CONSOLE_WIDTH - width)/2, (CONSOLE_HEIGHT - height)/2};
	COORD cc = corner;
	cc.Y--;
	SetConsoleTextAttribute(h, ATTR4);
	SetConsoleCursorPosition(h,cc);
	if (message) cout<<message;
	else cout << "Turn:" << turnNumber;
	cc = corner;
	SetConsoleTextAttribute(h, ATTR1);
	for (int i=0; i < this->height; i++)
	{
		SetConsoleCursorPosition(h,cc);
		for (int j=0; j < this->width; j++)
		{
			if (turn[j][i]) cout << "X";
			else cout << " ";
		}
		cc.Y++;
	}
}

void Field::consolePrint(char* message="")
{
	COORD corner={(CONSOLE_WIDTH - width)/2, (CONSOLE_HEIGHT - height)/2};
	COORD cc = corner;
	cc.Y--;
	SetConsoleTextAttribute(h, ATTR4);
	SetConsoleCursorPosition(h,cc);
	cout << "Turn:" << turnNumber <<" "<< message <<"                   ";
	cc = corner;
	SetConsoleTextAttribute(h, ATTR1);
	for (int i=0; i < this->height; i++)
	{
		for (int j=0; j < this->width; j++)
		{
			if (turn[j][i] != consoleBuffer[j][i])
			{
				SetConsoleCursorPosition(h,{cc.X+j,cc.Y+i});
				if (turn[j][i]) cout << "X";
				else cout << " ";
				consoleBuffer[j][i] = turn[j][i];
			}
		}
	}
}

