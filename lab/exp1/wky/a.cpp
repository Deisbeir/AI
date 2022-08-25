#include<iostream>
#include<fstream>
#include<list>
using namespace std;

class state
{
public:
    int map[5][5];
    int final[5][5];
	int  vessel_x,vessel_y;
	int f,g,h;
	char road[40];
public:
    void exchange(int &a,int &b);
    void getchild(state pint,state last);

};

std::list<state> values;

int my_h(state point)
{
    int count=0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if(point.map[i][j]!=point.final[i][j] && point.map[i][j]>0)
				count++;
		}
	}
	return count;
}


void state::exchange(int &a,int &b)
{
    int c;
    c=a;
    a=b;
    b=c;
}

void state::getchild(state point,state last)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			point.map[i][j] = point.final[i][j];
		}
	}
	point.vessel_x=last.vessel_x;
    point.vessel_y=last.vessel_y;
    point.g = last.g + 1;
    point.h = my_h(point);
    point.f = point.g + point.h;
    for(int i=0;i<40;i++)
    {
        point.road[i]=last.road[i];
    }
    values.push_back(point);
    values.sort();
}

void A(state point)
{
	while(my_h(point)==0)
	{
		if((point.vessel_x>0&&point.vessel_x<4)&&(point.vessel_y>0&&point.vessel_y<4))
		{
			exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x+1][point.vessel_y]);
			point.road[point.vessel_x+1]='D';
			getchild(point);
			exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x+1][point.vessel_y]);
			point.road[point.vessel_x+1]='0';
            exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x-1][point.vessel_y]);
			point.road[point.vessel_x-1]='u';
			getchild(point);.
			exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x-1][point.vessel_y]);
			point.road[point.vessel_x-1]='0';
            exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x][point.vessel_y+1]);
			point.road[point.vessel_y+1]='R';
			getchild(point);
			exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x][point.vessel_y+1]);
			point.road[point.vessel_y+1]='0';
            exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x][point.vessel_y-1]);
			point.road[point.vessel_y-1]='L';
			getchild(point);
			exchange(point.map[point.vessel_x][point.vessel_y],point.map[point.vessel_x][point.vessel_y-1]);
			point.road[point.vessel_y-1]='0';
		}
		point = values.back();
		values.pop_back();
	}
}

int main()
{
    state point;
    ifstream in,last;
    in.open("..\\data\\input00.txt", ios::in);
	if (!in.is_open())
		return;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			in>>point.map[i][j];
		}
	}
	in.close();
	last.open("..\\data\\target10.txt", ios::in);
	if (!last.is_open())
		return;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			last>>point.final[i][j];
		}
	}
	last.close();
    point.g=0;
    point.h=my_h(point);
    point.f=point.g+point.h;
    for(int i=0;i<40;i++)
    {
        point.road[i]='0';
    }
    for(int i=0;i<40&&point.road[i]!='0';i++)
    {
        
        printf("%c ",point.road[i]);
    }
    return 0;
}