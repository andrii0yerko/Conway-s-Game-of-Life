#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include "constants.h"
using namespace std;


class Field
{
	private:
		HANDLE h;
		COORD corner;
		int turnNumber = 0;
		int heigth;
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
		void consoleInitialize();
		void consolePrint();
		bool refresh();
};

void Field::arrcpy(int** a,int** b)
{
	for (int i=0; i < this->width; i++)
		for (int j=0; j < this->heigth; j++)
		a[i][j] = b[i][j];
}

Field::Field(HANDLE hndl, Settings_struct set)
{
	h = hndl;
	setSettings(set);
}

Field& Field::setSettings(Settings_struct set)
{
	heigth     = set.heigth;
	width      = set.width;
	corner.X   = (CONSOLE_WIDTH - width)/2;
	corner.Y   = 1;
	
	turnNumber = 0;
	
	status     = createEmptyArray();
	nextstatus = createEmptyArray();
	turn       = createEmptyArray();
	
	
	if (set.pattern==Random)
	{
		for (int i=0; i < this->width; i++)
			for (int j=0; j < this->heigth; j++)
			turn[i][j] = rand() % 2;
	}
	else if (set.pattern == Rpentomino)		
	{
		int i = (width - 3)/2;
		int j = (heigth - 3)/2;
		turn[i+1][j] = turn[i+2][j] = turn[i][j+1] = turn [i+1][j+1] = turn[i+1][j+2] = 1;
	}
	else if (set.pattern == Acorn)		
	{
		int i = (width - 7)/2;
		int j = (heigth - 3)/2;
		turn[i+1][j] = turn[i+3][j+1] = turn[i][j+2] = turn[i+1][j+2] = turn[i+4][j+2] = turn[i+5][j+2] = turn[i+6][j+2] = 1;
	}
	else if (set.pattern == Glidergun)
	{
		int i = (width - 36)/2;
		int j = (heigth - 9)/2;
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
	
	for (int i=0; i < this->width; i++)
	{
		for (int j=0; j < this->heigth; j++)
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
	
	return *this;
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
		arr[i] = new int[this->heigth];
		for (int j=0; j < this->heigth; j++)
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
		for (int j=0; j < this->heigth; j++)
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
			for (int j=0; j < this->heigth; j++)
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
			st[(i-n+this->width)%this->width][(j-m+this->heigth)%this->heigth]++;
		}
	}
	st[i][j]--;
}


void Field::consoleInitialize()
{
	COORD cc = corner;
	cc.Y--;
	SetConsoleTextAttribute(h, ATTR4);
	SetConsoleCursorPosition(h,cc);
	cout << "Turn:" << turnNumber;
	cc = corner;
	SetConsoleTextAttribute(h, ATTR1);
	for (int i=0; i < this->heigth; i++)
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

void Field::consolePrint()
{
	COORD cc = corner;
	cc.Y--;
	SetConsoleTextAttribute(h, ATTR4);
	SetConsoleCursorPosition(h,cc);
	cout << "Turn:" << turnNumber;
	cc = corner;
	SetConsoleTextAttribute(h, ATTR1);
	for (int i=0; i < this->heigth; i++)
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

