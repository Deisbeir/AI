#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<cmath>
#include<queue>
#include<map>
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define LEFT_THROUGH 4
#define RIGHT_THROUGH 5
#define UP_THROUGH 6
#define DOWN_THROUGH 7
using namespace std;

typedef struct Node
{
	vector<vector <int> > status;
	vector<int>  spacecraft;
	int f;
	int g;
	int h;
	string path;
	string path2;
}Node;

vector<vector <int> > targetmap;

void getinput(string inputpath, string targetpath, vector<vector <int> > &inputmap)
{
	vector<int> value;
    ifstream inputfile,targetfile;
    inputfile.open(inputpath, ios::in);
	if (!inputfile.is_open())
	{
		cout<<"can't open file!"<<endl;
		return;
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			int a;
			inputfile>>a;
			value.push_back(a);
		}
		inputmap.push_back(value);
		value.clear();
	}
	inputfile.close();
	targetfile.open(targetpath, ios::in);
	if (!targetfile.is_open())
	{
		cout<<"can't open file!"<<endl;
		return;
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			int a;
			targetfile>>a;
			value.push_back(a);
		}
		targetmap.push_back(value);
		value.clear();
	}
	targetfile.close();
}

class cmp{
public:
    bool operator()(Node &a,Node &b){
        return a.f>b.f; 
    }
};
priority_queue<Node, vector<Node>, cmp> Open;//小顶堆
map<vector<vector <int> >,bool> mymap;

int h1(vector<vector <int> > nowmap)
{
    int noms=0;
	for (int i = 0; i < nowmap.size(); i++)
	{
		for (int j = 0; j < nowmap[i].size(); j++)
		{
			if((nowmap[i][j]!=targetmap[i][j]) && nowmap[i][j]!=0)
				noms++;
		}
	}
	return noms;
}

int h2(vector<vector <int> > nowmap)
{
	int x=0,y=0;
	int num = 0;
	for(int i=0; i<nowmap.size(); i++)
		for(int j=0; j<nowmap[i].size(); j++)
		{
			num = nowmap[i][j];
			if(num > 0  && targetmap[i][j] != num)
			{
				for(int m=0; m<targetmap.size(); m++)
				{
					for(int n=0; n<5; n++)
					{
						if(targetmap[m][n]==num)
						{
							if((i>=2&&m==4)||(m>=2&&i==4))
								x+=fabs(m-i);
							else
								x+= fabs((m+1)%5-(i+1)%5);
							if((j>=2&&n==4)||(n>=2&&j==4))
								y+=fabs(n-j);
							else
								y+= fabs((n+1)%5-(j+1)%5);
							n=m=5;
						}
					}
				}
			}
		}
	return (x+y);
}

void generateChild(Node& cur_stat)
{
    if(mymap.count(cur_stat.status)==0)
    {
        Node node;
		node.status = cur_stat.status;
		node.spacecraft = cur_stat.spacecraft;
        node.g = cur_stat.g + 1;
        //node.h= h1(cur_stat.status);
        node.h= h2(cur_stat.status);
        node.f = node.g + node.h;
        node.path = cur_stat.path2;
        Open.push(node);
        mymap[cur_stat.status]=1;
	}
}

void move_state(Node& p, int dir)
{
	int x = p.spacecraft[0]; 
	int y = p.spacecraft[1];
	switch(dir)
	{
		case DOWN:
				p.status[x][y] = p.status[x+1][y];
				p.status[x+1][y] = 0;
				p.spacecraft[0] = x+1;
			break;
		case UP:
				p.status[x][y] = p.status[x-1][y];
				p.status[x-1][y] = 0;
				p.spacecraft[0] = x-1;
			break;
		case LEFT:
				p.status[x][y] = p.status[x][y-1];
				p.status[x][y-1] = 0;
				p.spacecraft[1] = y-1 ;
			break;
		case RIGHT:
				p.status[x][y] = p.status[x][y+1];
				p.status[x][y+1] = 0;
				p.spacecraft[1] = y+1;
			break;
		case DOWN_THROUGH:
				p.status[4][2] = p.status[0][2];
				p.status[0][2] = 0;
				p.spacecraft[0] = 0;
			break;
		case UP_THROUGH:
				p.status[0][2] = p.status[4][2];
				p.status[4][2] = 0;
				p.spacecraft[0] = 4;
			break;
		case LEFT_THROUGH:
				p.status[2][0] = p.status[2][4];
				p.status[2][4] = 0;
				p.spacecraft[1] = 4 ;
			break;
		case RIGHT_THROUGH:
				p.status[2][4] = p.status[2][0];
				p.status[2][0] = 0;
				p.spacecraft[1] = 0;
			break;
		default:
			break;
	}
}

void A_star_h2(Node& source)
{
	int x,y;
	while(source.h != 0)
	{
		x = source.spacecraft[0], y = source.spacecraft[1];
		if(x<4 && source.status[x+1][y] > 0 )
		{
			move_state(source, DOWN);
			source.path2 = source.path + "D";
			generateChild(source);
			move_state(source, UP);
		}
		if(x>0 && source.status[x-1][y] > 0 )
		{
			move_state(source,UP);
			source.path2 = source.path + "U";
			generateChild(source);
			move_state(source,DOWN);
		}
		if(y>0 && source.status[x][y-1] > 0 )
		{
			move_state(source,LEFT);
			source.path2 = source.path + "L";
			generateChild(source);
			move_state(source,RIGHT);
		}
		if(y<4 && source.status[x][y+1] > 0 )
		{
			move_state(source,RIGHT);
			source.path2 = source.path + "R";
			generateChild(source);
			move_state(source,LEFT);
		}
		if((x==0 && y==2) && source.status[4][2] > 0 )
		{
			move_state(source,UP_THROUGH);
			source.path2 = source.path + "U";
			generateChild(source);
			move_state(source,DOWN_THROUGH);
		}
		if((x==4 && y==2) && source.status[0][2] > 0 )
		{
			move_state(source,DOWN_THROUGH);
			source.path2 = source.path + "D";
			generateChild(source);
			move_state(source,UP_THROUGH);
		}
		if((x==2 && y==0) && source.status[2][4] > 0 )
		{
			move_state(source,LEFT_THROUGH);
			source.path2 = source.path + "L";
			generateChild(source);
			move_state(source,RIGHT_THROUGH);
		}
		if((x==2 && y==4) && source.status[2][0] > 0 )
		{
			move_state(source,RIGHT_THROUGH);
			source.path2 = source.path + "R";
			generateChild(source);
			move_state(source,LEFT_THROUGH);
		}
		source = Open.top();
		Open.pop();
	}
}

void IDA_h1(Node& source)
{
	int x,y;
	int limit = source.f;
	int next_limit;
	while(limit < 100)
	{
		next_limit = 100;
		Open.push(source);
		while(!Open.empty())
		{
			source = Open.top();
			Open.pop();
			source.f = source.h + source.g;
			if(source.h == 0)
				return;
			else if(source.f > limit)
				next_limit = min(next_limit,source.f);
			else
			{
                x = source.spacecraft[0], y = source.spacecraft[1];
				if(x<4 && source.status[x+1][y] > 0 )
				{
					move_state(source, DOWN);
					source.path2 = source.path + "D";
					generateChild(source);
					move_state(source, UP);
				}
				if(x>0 && source.status[x-1][y] > 0 )
				{
					move_state(source,UP);
					source.path2 = source.path + "U";
					generateChild(source);
					move_state(source,DOWN);
				}
				if(y>0 && source.status[x][y-1] > 0 )
				{
					move_state(source,LEFT);
					source.path2 = source.path + "L";
					generateChild(source);
					move_state(source,RIGHT);
				}
				if(y<4 && source.status[x][y+1] > 0 )
				{
					move_state(source,RIGHT);
					source.path2 = source.path + "R";
					generateChild(source);
					move_state(source,LEFT);
				}
				if((x==0 && y==2) && source.status[4][2] > 0 )
				{
					move_state(source,UP_THROUGH);
					source.path2 = source.path + "U";
					generateChild(source);
					move_state(source,DOWN_THROUGH);
				}
				if((x==4 && y==2) && source.status[0][2] > 0 )
				{
					move_state(source,DOWN_THROUGH);
					source.path2 = source.path + "D";
					generateChild(source);
					move_state(source,UP_THROUGH);
				}
				if((x==2 && y==0) && source.status[2][4] > 0 )
				{
					move_state(source,LEFT_THROUGH);
					source.path2 = source.path + "L";
					generateChild(source);
					move_state(source,RIGHT_THROUGH);
				}
				if((x==2 && y==4) && source.status[2][0] > 0 )
				{
					move_state(source,RIGHT_THROUGH);
					source.path2 = source.path + "R";
					generateChild(source);
					move_state(source,LEFT_THROUGH);
				}
            }
		}
        limit = next_limit;
	}
	cout<<"No solution!" << endl; 	
}

int main(void) 
{
    string inputpath,targetpath;
	vector<vector <int> > inputmap;
    inputpath="..\\data\\input00.txt";
    targetpath="..\\data\\target00.txt";
    getinput(inputpath,targetpath,inputmap);
	Node source;
	source.status=inputmap;
	for(int i=0; i<source.status.size(); i++)
		for(int j=0; j<source.status[i].size(); j++)
		{	
			if(source.status[i][j] == 0)
			{
				source.spacecraft.push_back(i);
				source.spacecraft.push_back(j);
			}
		}
	source.g = 0;
	//source.h = h1(source.status);
	source.h=h2(source.status);
	source.f = source.g + source.h;
	source.path = "";
	source.path2 = "";
	clock_t start, finish;
	double totaltime;
	start = clock();
	IDA_h1(source);
	//A_star_h2(source);
	finish = clock();
	totaltime = (double)(finish - start)/CLOCKS_PER_SEC;
	cout << totaltime << endl;
	cout << source.path << endl;
    return 0;
}