#include <iostream>
using namespace std;

#define PNUM 50
#define SRC 3

int Max[PNUM][SRC];
int Allocation[PNUM][SRC];
int Need[PNUM][SRC];
int safeQueue[PNUM];
int r[SRC];
bool finish[PNUM];

int p;

void getInput()
{
	cout<<"Please input the sum of process:";
	cin>>p;
	cout<<"Please input the 3 kind of resource number:";
	for (int i = 0; i < SRC; i++)
	{
		cin>>r[i];
	}
	cout<<"Please input Max each resource of each process:"<<endl;
	for (int i = 0; i < p; i++)
	{
		for (int j = 0; j < SRC; j++)
		{
			cin>>Max[i][j];
		}
	}
	
	cout<<"Please input the Allocation of each resource of each process:"<<endl;
	for (int i = 0; i < p; i++)
	{
		for (int j = 0; j < SRC; j++)
		{
			cin>>Allocation[i][j];
			Need[i][j] = Max[i][j] - Allocation[i][j];
		}
	}
}

void printMatrix()
{
	cout<<"Need matrix: "<<endl;
	for (int i = 0; i < p; ++i)
	{
		cout<<"process "<<i<<": ";
		for (int j = 0; j < SRC; ++j)
		{
			cout<<Need[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout<<"Resource matrix: ";
	for (int i = 0; i < SRC; ++i)
	{
		cout<<r[i]<<"  ";
	}
	cout<<endl<<endl<<endl;
}

void Bank(int &count, int &s)
{
	while(s < p && count <= p)
	{
		for (int i = 0; i < p && !finish[i]; i++)
		{
			finish[i] = true;
			for (int j = 0; j < SRC && finish[i]; j++)
			{
				if (Need[i][j] > r[j])
					finish[i] = false;
			}
			if (finish[i])
			{
				safeQueue[s] = i;
				s++;
				cout<<"process "<<i<<" success"<<endl;
			}else{
				cout<<"process "<<i<<" failed"<<endl;
			}
			for (int k = 0; k < SRC && finish[i]; k++)
			{
				r[k]+=Allocation[i][k];
				Need[i][k] = 0;
			}
			printMatrix();
		}
		count ++;
	}
}

void safe(int &count, int &s)
{
	if (count <= p)
	{
		cout<<"safe queue: "<<endl;
		for (int i = 0; i < s; ++i)
		{
			cout<<"process "<<safeQueue[i]<<endl;
		}
	}else{
		cout<<"deadlock state!!"<<endl;
	}
}
int main(int argc, char const *argv[])
{
	int count = 0;
	int s = 0;
	getInput();
	Bank(count,s);
	safe(count,s);
	return 0;
}