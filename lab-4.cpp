#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define PAGESIZE (1<<10)
#define BLOCK 4
#define DIRECT 2

struct item
{
	int value;
};

struct page
{
	int num;
	struct item p_item[PAGESIZE];
};

struct iNode   
{
	int i_val[DIRECT];
	struct item i_item[3];
}inode;

int random(int b)
{
	srand((unsigned)time(NULL));
	int ret = rand()%b;
	return ret;
}

void printINode()
{
	cout<<"iNode:"<<endl;
	cout<<"direct: "<<inode.i_val[0]<<endl;
	cout<<"direct: "<<inode.i_val[1]<<endl;
	for (int i = 0; i < 3; ++i)
	{
		cout<<i+1<<" indirect: "<<inode.i_item[i].value<<endl;
	}
}
int main(int argc, char const *argv[])
{
	struct page s_p;
	struct item s_i;
	/*初始化iNode*/
	inode.i_val[0] = 23;
	inode.i_val[1] = 34;
	for (int i = 0; i < 3; ++i)
	{
		inode.i_item[i].value = (i+1)*random(PAGESIZE);
	}

	printINode();  //输出iNode详细信息
	int physicalBlock = 0;
	int N_item = PAGESIZE/BLOCK;
	long address;
	cout<<"Please input the file logical address(decimal:0-17247242240): ";
	cin>>address;
	long n = address / PAGESIZE;
	long left = address - n * PAGESIZE;
	if (n < DIRECT)   //直接块
	{
		physicalBlock = inode.i_val[n];
		cout<<"In direct block "<<n<<": ("<<physicalBlock<<", "<<left<<")"<<endl;
	}else if (n < (DIRECT+N_item))  //一次间接块
	{
		int offset = n - DIRECT;
		s_i = inode.i_item[0];
		s_p.num = s_i.value;
		s_p.p_item[offset].value = random(PAGESIZE);
		physicalBlock = s_p.p_item[offset].value;
		cout<<"In single indirect block "<<": ("<<physicalBlock<<", "<<left<<")"<<endl;
	}else if (n < (DIRECT+N_item+N_item*N_item))  //二次间接块
	{
		int lt = n - DIRECT - N_item;
		int offset = lt / N_item;
		int offset1 = lt - offset * N_item;
		s_i = inode.i_item[1];
		s_p.num = s_i.value;
		s_p.p_item[offset].value = random(PAGESIZE);
		s_i = s_p.p_item[offset];
		s_p.num = s_i.value;
		s_p.p_item[offset1].value = random(PAGESIZE);
		physicalBlock = s_p.p_item[offset1].value;
		cout<<"In double indirect block "<<": ("<<physicalBlock<<", "<<left<<")"<<endl;
	}else                           //三次间接块
	{
		long lt = n - DIRECT - N_item - N_item*N_item;
		long offset = lt / (N_item*N_item);
		long offset1 = lt - offset * (N_item*N_item);
		int offset2 = offset1 / N_item;
		int offset3 = offset1 - offset2 * N_item;
		s_i = inode.i_item[2];
		s_p.num = s_i.value;
		s_p.p_item[offset].value = random(PAGESIZE);
		s_i = s_p.p_item[offset];
		s_p.num = s_i.value;
		s_p.p_item[offset3].value = random(PAGESIZE);
		physicalBlock = s_p.p_item[offset3].value;
		cout<<"In triple indirect block "<<": ("<<physicalBlock<<", "<<left<<")"<<endl;
	}	
	return 0;

}