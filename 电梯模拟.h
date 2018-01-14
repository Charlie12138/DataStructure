#ifndef	_HEAD_H 
#define _HEAD_H 
#include<time.h>
#define FALSE 0 
#define TRUE 1
#define ERROR 0
#define Empty 0 
#define OK 1
#define	STACK_INIT_SIZE	10			//存储空间初始分配量
#define	STACKINCREMENT	5			//存储空间分配增量
#define Maxfloor 4            		//最高层
#define Minfloor 0					//最底层
#define StackInitSize 10			//乘客栈存储空间初始分配量
#define StackIncrement 5			//乘客栈存储空间分配增量 
#define CloseTestTime 40			//电梯关门测试时间
#define OverTime 300				//电梯停侯超时时间
#define Accelerate 15				//加速时间
#define UpTime 5					//上升时间
#define DownTime 15					//下降时间
#define UpDecelerate 5				//上升减速
#define DownDecelerate 5			//下降减速
#define DoorTime 20					//开门关门时间
#define InOutTime 25				//进出电梯时间 
long MaxTime;						//系统运行最长时间
int InOutCount=0;					//用于进出计时
int InterTime=0;					//下一乘客进入系统的时间
int ID=0;							//乘客编号
int GiveUpNumber=0;					//乘客放弃的数量 
int TotalTime=0;					//总共等待时间
typedef int Status;    
float Time=0;						// 
enum ClientStatus{					//客户状态 
	New,							
	GiveUp,
	In,
	Out,
	Finish
};  
enum ElevatorStatus{				//电梯状态 
	Opening,						//正在开门 
	Opened,							//已开门 
	Closing,						//正在关门 
	Closed,							//已关门 
	Moving,							//移动 
	Decelerate,   					//减速 
	Idle 							//停侯 
}; 

enum ElevatorStage{					//电梯阶段 
	Up,								//上升 
	Down,							//下降 
	OpenDoor,						//开门 
	Stop 							//停止 
};

enum Action{
	DoorOpened,						//已开门 
	DoorClosed,						//已关门 
	GoingUp,						//上升 
	GoingDown,						//下降 
	Achieved,						//停止 
	None
};

 
typedef struct{				//乘客类型 
	int ClientID;         	//乘客编号 
	int InTime;				//乘客进入时间 
	int Infloor;        	//乘客进入的楼层
	int Outfloor;       	//乘客要去的楼层 
	int GiveUpTime;     	//所能容忍的最大等候时间  
}Client;

typedef Client *QElemType;
typedef struct QNode{ 		//等候队列 
	QElemType data;			//数据域 
	struct QNode *next;		//指针域 
}QNode,*QueuePtr;
typedef struct{
	QueuePtr front;			//队头指针
	QueuePtr rear;			//队尾指针 
}Queue;

typedef Client *SElemType;
typedef struct{				//乘客栈 
	SElemType *base;		//栈底指针
	SElemType *top;			//栈顶指针
	int stacksize;			//当前已分配的存储空间 
}ClientStack; 

typedef struct{
	int floor;   						//电梯当前所在层
	int ClientNumber;					//电梯内人数
	ElevatorStatus status;				//电梯当前状态
	ElevatorStage Stage;				//电梯运行阶段
	int Count;							//用于电梯计时
	int CallUp[Maxfloor+1];				//每层的Up按钮
	int CallDown[Maxfloor+1];			//每层的Down按钮
	int CallCar[Maxfloor+1];			//电梯内目标层按钮
	ClientStack S[Maxfloor+1];			//乘客栈，要去不同楼层的人放在不同的栈中  
}Elevator;

#endif
