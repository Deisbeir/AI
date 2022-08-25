#include<iostream>
#include<vector>
using namespace std;

struct worker
{
    bool senior;
    vector<int> dislike;
    vector<bool> workday;
    vector<bool> assign;
};

vector<worker> workers;
int need_num;

void get_info()
{
    int num,dislike_num;
    worker represent;
    cout<<"请输入总工人数："<<endl;
    cin>>num;
    for(int i=0;i<num;i++)
    {
        cout<<"请输入第"<<i+1<<"个工人信息："<<endl;
        cout<<"他是否是senior(是的话请输入 1 否则输入 0 )"<<endl;
        cin>>represent.senior;
        cout<<"他不喜欢几个人"<<endl;
        cin>>dislike_num;
        for(int j=0;j<dislike_num;j++)
        {
            if(j==0)
                cout<<"他不喜欢的是第几个工人"<<endl;
            else
                cout<<"他还不喜欢第几个工人"<<endl;
            int id;
            cin>>id;
            represent.dislike.push_back(id-1);
        }
        for(int j=0;j<7;j++)
            represent.assign.push_back(false);
        for(int j=0;j<7;j++)
            represent.workday.push_back(true);
        workers.push_back(represent);
        represent.dislike.clear();
        represent.workday.clear();
        represent.assign.clear();
    }
    cout<<"请输入每天需要几个人值班："<<endl;
    cin>>need_num;
}

bool workday_check()    //检查是否有工人休息不足
{
    for(int i=0;i<workers.size();i++)
    {
        int days=0;
        for(int j=0;workers[i].assign[j]&&j<7;j++)
        {
            days+=workers[i].workday[j];
            if(days>5)
                return false;
        }
    }
    return true;
}

bool day_off_check()    //检查是否有工人过的太舒服
{
    for(int i=0;i<workers.size();i++)
    {
        for(int j=0;(workers[i].assign[j+2]!=0)&&j<5;j++)
        {
            if((workers[i].workday[j]+workers[i].workday[j+1]+workers[i].workday[j+2])==0)
                return false;
        }
    }
    return true;
}

bool num_enough_check()     //检查每天人手是否充足
{
    for(int i=0;i<7;i++)
    {
        int today_num = 0;
        for(int j=0;j<workers.size();j++)
        {
            today_num+=workers[j].workday[i];
        }
        if(today_num<need_num)
            return false;
    }
    return true;
}

bool senior_check(int y)     //是否每天都有老师傅镇场
{
    int senior_num = 0;
    bool flag=true;
    for(int j=0;j<workers.size();j++)
    {
        senior_num+=workers[j].senior&&workers[j].assign[y]&&workers[j].workday[y];
    }
    for(int j=0;j<workers.size();j++)
    {
        flag&=workers[j].assign[y];
    }
    if(senior_num==0 && flag==true)
        return false;
    return true;
}

bool dislike_check()        //不和不喜欢的人呆在一起
{
    for(int i=0;i<workers.size();i++)
    {
        for(int j=0;j<workers[i].dislike.size();j++)
        {
            for(int k=0;k<7;k++)
            {
                if(workers[i].assign[k] && workers[workers[i].dislike[j]].assign[k])
                    if(workers[i].workday[k] && workers[workers[i].dislike[j]].workday[k])
                        return false;
            }
        }
    }
    return true;
}

bool all_check(int y)    //一次性检查是否有条件不满足
{
    if(!workday_check())
        return false;
    if(!day_off_check())
        return false;
    if(!dislike_check())
        return false;
    if(!num_enough_check())
        return false;
    if(!senior_check(y))
        return false;
    return true;
}

bool unassinged_variable(int &x,int &y)     //未赋值的变量
{
    for (int i=0;i<workers.size();i++) 
    {
        for (int j= 0;j<workers[i].assign.size();j++) 
        {
            if (workers[i].assign[j]==false) 
            {
                x = i;
                y = j;
                return false;
            }
        }
    }
    return true;
}

void print(vector<worker> result)
{
    for (int i=0;i<7;i++) 
    {
        for (int j=0;j<result.size();j++)
        {
            if (result[j].workday[i])
                cout<<j+1<<" ";
        }
        cout<<endl;
    }
}

void csp()
{
    int x,y;
    if(unassinged_variable(x,y))
    {
        print(workers);
        exit(0);
    }
    else
        for (int i = 0; i < 2; i++)
        {
            workers[x].assign[y]=true;
            workers[x].workday[y]=i;
            if(all_check(y)) {
                csp();
            }   
        }
    workers[x].assign[y]=false;
}

int main(void)
{
    get_info();
    csp();
    return 0;
}
