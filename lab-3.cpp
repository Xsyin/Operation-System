#include <iostream>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define WEISHU 64
#define N 65535
#define MAX(h) (1<<h)

int pageSize, itemSize;  //页面大小，每项大小
int rankNum;             //页表级数
string address;          //逻辑地址
int ranks[WEISHU];       //每级页表在逻辑地址中的位数
unsigned long addr[WEISHU]; //项在每级页表中的偏移量

struct item     //页表项
{
  int value;
};

struct page    //页表
{
  int num;
  struct item item[N];
};

void getRank();
void getInput();
void interpretAddress();
void pageTranslate();

int main(int argc, char* argv[])
{
   
   getInput();
   getRank();
   interpretAddress();
   pageTranslate();
   return 0;
}

void getRank()
{
	int rankSum = WEISHU - pageSize;
	int itemNum = pageSize - itemSize;
  if (rankSum % itemNum == 0)
    rankNum = rankSum / itemNum;
  else
	  rankNum = rankSum / itemNum + 1;	
  cout<<"page rank:"<<rankNum<<endl;

}

void getInput()
{
	cout<<"Input the page size n(2^n Byte):";
	cin>>pageSize;
	cout<<"Input the item size n(2^n Byte):";
  cin>>itemSize;
  cout<<"Input the logical address(eg: 0xffff ffff ffff ffff): "<<endl;
  
  int a[4];
  for (int i = 0; i < 4; ++i)
  {
    cin>>hex>>a[i];
    bitset<16> bit16(a[i]);
    address += bit16.to_string();
  }
}

void interpretAddress()
{
  cout<<"logical address:"<<endl<<address<<endl;
  int items = pageSize - itemSize;
  ranks[rankNum] = pageSize;
  ranks[0] = WEISHU - pageSize - (rankNum - 1)* items;
  for (int i = 1; i < rankNum; ++i)
  {
    ranks[i] = items;
  }
  int sum = 0;
  cout<<"every rank in bits: "<<endl;
  for (int i = 0; i < rankNum+1; ++i)
  {
    cout<<ranks[i]<<"  ";
    int t = ranks[i];
    bitset<32> bitt(address,sum,t);
    addr[i] = bitt.to_ulong();
    sum += t;
  }
  cout<<endl;
  cout<<"every rank in offset: "<<endl;
  for (int i = 0; i < rankNum+1; ++i)
  {
    cout<<addr[i]<<"  ";
  }
  cout<<endl;
}

void pageTranslate()
{
  srand((unsigned)time(NULL));
  struct page pg[rankNum+1];
  int pageNum;
  for (int i = 0; i < rankNum+1; ++i)
  {
    int temp = ranks[i];
    pg[i].item[temp].value = rand()%MAX(temp);
    pageNum = pg[i].item[temp].value;
    if (i < rankNum)
    {
      pg[i+1].num = pageNum;
    }    
  }
  for (int i = 0; i < rankNum; ++i)
  {
    cout<<"Page rank "<<i+1<<": ("<<pg[i].num<<", "<<addr[i]<<")"<<endl;
  }
  cout<<"Physical address: ("<<pg[rankNum].num<<", "<<addr[rankNum]<<")"<<endl;
}
