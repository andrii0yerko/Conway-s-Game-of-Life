#define Cell int
#include <iostream>
//#include "Cell.cpp"
using namespace std;
class Field
{
	private:
		int heigth;
		int width;
		Cell **turn, **nextturn;
		int **status, **nextstatus;
		Cell** createEmptyArray();
		int checkNeighbors(int, int);
		void neighborsstat(int i, int j, int**);
		void del(Cell**);
		
	public:
		Field(int, int);
		~Field();
		void consolePrint();
		void refresh();
		void initialize();
};

Field::Field(int h, int w)
{
	this->heigth = h;
	this->width = w;
	
	turn = createEmptyArray();
	nextturn = createEmptyArray();
	status = createEmptyArray();
}

Field::~Field()
{
	del(turn);
	del(nextturn);
	del(status);
	del(nextstatus);
}

Cell** Field::createEmptyArray()
{
	Cell** arr = new Cell*[this->heigth];
	for (int i=0; i < this->heigth; i++)
	{
		arr[i] = new Cell[this->width];
		for (int j=0; j < this->width; j++)
		{
			arr[i][j] = 0;
		}
	}
	return arr;
}

void Field::del(Cell**arr)
{
	for (int i=0; i < this->heigth; i++)
	{
		delete [] arr[i];
	}
	
	delete [] arr;
}

//void Field::refresh()
//{
//	nextturn = createEmptyArray();
//	for (int i=0; i < this->heigth; i++)
//	{
//		for (int j=0; j < this->width; j++)
//		{
//			nextturn[i][j] = checkNeighbors(i,j);
//		}
//	}
//	del(turn);
//	turn = nextturn;
//}

void Field::refresh()
{
	nextstatus = createEmptyArray();
	for (int i=0; i < this->heigth; i++)
	{
		for (int j=0; j < this->width; j++)
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
}


void Field::initialize()
{
	turn[7][0]=1;
	turn[7][1]=1;
	turn[7][2]=1;
	
	turn[2][7]=1;
	turn[3][7]=1;
	turn[4][7]=1;
	
	turn[3][10]=1;
	turn[3][9]=1;
	turn[3][8]=1;
	
	turn[6][4]=1;
	turn[6][5]=1;
	turn[6][6]=1;
	turn[5][6]=1;
	turn[4][5]=1;
	
	turn[9][4]=1;
	turn[9][5]=1;
	turn[9][6]=1;
	turn[8][6]=1;
	turn[7][5]=1;
	
	for (int i=0; i < this->heigth; i++)
	{
		for (int j=0; j < this->width; j++)
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
			st[(i-n+this->heigth)%this->heigth][(j-m+this->width)%this->width]++;
		}
	}
	st[i][j]--;
}

int Field::checkNeighbors(int n, int m)
{
	int count=0-turn[n][m];
	for (int i=-1; i <= 1; i++)
	{
		
		for (int j=-1; j<=1; j++)
		{
			if (turn[(n-i+this->heigth)%this->heigth][(m-j+this->width)%this->width]) count++;
		}
	}
	
	if (count==3) return 1;
	if (count<2 || count>3) return 0;
	else return turn[n][m];
}

void Field::consolePrint()
{
	for (int i=0; i < this->heigth; i++)
	{
		cout << "|";
		for (int j=0; j < this->width; j++)
		{
			if (turn[i][j]) cout << "#";
			else cout << " ";
		}
		cout << "|";
		cout << "\n";
	}
}
