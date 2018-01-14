#ifndef _Bank_H_
#define _Bank_H_
#include <string>
using namespace std;

/*客户结点类型*/
struct CustNode{  
    int num; 						//客户号  
    string Type; 					//到达或离开  
    int BeginTime;					//到达时间  
    int EndTime;					//离开时间  
    int Amount; 					//正数为存款，负数为取款  
    CustNode *next;					//指针域  
};  

/*等待队列类型*/
struct Queue{ 						//队列  
    CustNode *front;				//队列头指针  
    CustNode *rear;					//队列尾指针  
};

/*常用变量的定义*/  
int BankAmount; 					//初始时银行现存资金总额  
int CloseTime; 						//营业结束时间  
int ClientArriveMaxTime; 			//两个到达事件之间的间隔上限  
int ClientArriveMinTime; 			//两个到达事件之间的间隔下限  
int DealMaxTime; 					//客户之间交易的时间上限  
int DealMinTime; 					//客户之间交易的时间下限  
int MaxAmount; 						//交易额上限  
int NeedIn=0;						//需要存款的人数  
int NeedOut=0;						//需要取款的人数  
int SuccessIn=0;					//成功存款的人数  
int SuccessOut=0;					//成功取款的人数  
int CurrentTime=0; 					//当前时间  
int BankAmountTime=0; 				//客户逗留总时间  
int counter=0; 						//客户总数  
int number=1; 						//初始客户序列号  
bool state=1; 						//用于判断是否有窗口在处理  
int DealTime=0;						//交易时间  
int MaxTime=0;						//最大到达时间  
Queue Event; 						//事件队列  
Queue Q1; 							//队列一  
Queue Q2; 							//队列二 

#endif
