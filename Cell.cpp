
class Cell
{
	private:
		int aliveness;
		int aliveNeighbors;
		int x,y;
		
	public:
		Cell(int,int);
		bool isAlive();
}

Cell::Cell(int x, int y)
{
	this->x = x;
	this->y = y;
	this->aliveness = 0;
	this->aliveNeighbors = 0;
}

