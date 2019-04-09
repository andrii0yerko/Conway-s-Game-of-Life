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
		int** createEmptyArray();
		void arrcpy(int**,int**);
		int checkNeighbors(int, int);
		void neighborsstat(int i, int j, int**);
		void del(int**);
		
	public:
		Field& setSettings(Settings_struct);
		Field(HANDLE, Settings_struct);
		~Field();
		void consolePrint();
		bool refresh();
		void initialize();
		
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
	heigth = set.heigth;
	width = set.width;
	corner.X = (CONSOLE_WIDTH - width)/2;
	corner.Y = (CONSOLE_HEIGTH - heigth)/2;
	
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


void Field::initialize()
{
	/*
	turn[8][7]=1;
	turn[7][7]=1;
	turn[6][7]=1;
	turn[6][8]=1;
	turn[7][6]=1;
	
	turn[8][12]=1;
	turn[7][12]=1;
	turn[6][12]=1;
	turn[6][13]=1;
	turn[7][11]=1;
	*/
	
	turn[6][11]=1;
	turn[6][12]=1;
	turn[6][15]=1;
	turn[6][16]=1;
	turn[6][17]=1;
	turn[7][14]=1;
	turn[8][12]=1;
	
	
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
		SetConsoleCursorPosition(h,cc);
		for (int j=0; j < this->width; j++)
		{
			if (turn[j][i]) cout << "X";
			else cout << " ";
		}
		cc.Y++;
	}
}
