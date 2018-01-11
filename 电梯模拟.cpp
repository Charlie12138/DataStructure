#include<stdio.h>
#include<iostream>
#include<iomanip> 
#include<math.h>
#include<time.h> 
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include"HEAD.h"
using namespace std;

void PrintClientInfo(Client p,ClientStatus s){
//输出乘客信息
	switch(s) {
		case New:cout<<Time<<'\t'<<p.ClientID<<"号乘客进入第"<<p.Infloor<<"层"<<endl;break;
		case GiveUp:cout<<Time<<"\t"<<p.ClientID<<"号乘客放弃等待, 等待了"<<Time-p.InTime<<"/0.1s"<<endl;break;
		case Out:cout<<Time<<'\t'<<p.ClientID<<"号乘客走出电梯"<<endl;break;
		case In:cout<<Time<<'\t'<<p.ClientID<<"号乘客走进电梯，要去第"<<p.Outfloor<<"层"<<endl;break;
		default:break;
	};

}
Status CreatClient(Client *&p){
	//生成新的乘客
	p=new Client;							//新建乘客类型	 
	if(!p) 									//新建乘客失败 
		return OVERFLOW;
	p->ClientID=ID++;						//产生乘客编号 
	p->GiveUpTime=rand()%120+30;			//产生乘客所能容忍的等待时间
	p->InTime=Time;							//产生乘客进入电梯系统的时间 
	InterTime=rand()%300;					//产生下一乘客要到达的时间
	p->Outfloor=rand()%(Maxfloor+1);		//产生乘客所要到达的楼层
	//该乘客出现的楼层
	while((p->Infloor=rand()%(Maxfloor+1))==p->Outfloor);
	//cin>>p->Infloor>>p->Outfloor>>InterTime;
	PrintClientInfo(*p,New);
	return OK;
}
Status DestoryClient(Client *&p) {
	//该乘客离开系统
	delete p;
	p=NULL;
	return OK;
}
Status GoAbove(Client const &e) {
	//判断该乘客是否去往高层
	if(e.Outfloor>e.Infloor) return TRUE;
	else return FALSE;
}
int CInfloor(Client const &e) {
	//返回乘客进入的楼层
	return e.Infloor;
}
int CInTime(Client const &e) {
	//返回乘客进入时间
	return e.InTime;
}
int COutfloor(Client const &e) {
	//返回乘客进入时间
	return e.Outfloor;
}

Status InitQueue(Queue &Q) {
	//构造一个空队列Q
	Q.front=Q.rear=new QNode;
	if(!Q.front) return OVERFLOW;//分配存储失败
	Q.front->next=NULL;
	Q.front->data=NULL;
	return OK;
}

Status DestroyQueue(Queue &Q) {
	//销毁队列Q
	while(Q.front) {
		Q.rear=Q.front->next;
		if(Q.front->data) DestoryClient(Q.front->data);
		delete Q.front;
		Q.front=Q.rear;
	}
	return OK;
}

Status EnQueue(Queue &Q,QElemType e) {
	//插入元素e为Q的新的队尾元素
	QueuePtr p;
	p=new QNode;
	if(!p) return OVERFLOW;
	p->data=e;p->next=NULL;
	Q.rear->next=p;
	Q.rear=p;
	return OK;
}
Status DeQueue(Queue &Q,QElemType &e) {
	//若队列不空,则删除Q的队头元素,用e返回其值,并返回OK;
	//否则返回ERROR
	QueuePtr p;
	if(Q.front==Q.rear) return ERROR;
	p=Q.front->next;
	e=p->data;
	Q.front->next=p->next;
	if(Q.rear==p) Q.rear=Q.front;
	delete p;
	return OK;
}

Status QueueEmpty(Queue Q) {
	//判断队列是否为空
	if(Q.front==Q.rear) return TRUE;
	else	return FALSE;
}

Status QDelNode(Queue &Q,QueuePtr p) {
	//删除队列中p指向的结点的下一个结点
	QueuePtr q;
	if(p==NULL||p->next==NULL) return ERROR;
	q=p->next;
	p->next=q->next;
	if(p->next==NULL) Q.rear=p;
	DestoryClient(q->data);
	delete q;
	return OK;
}

Status CGiveUp(Queue &Q,int floor) {
	//删除放弃等待的乘客
	QueuePtr p;
	p=Q.front;
	if(p->next!=NULL) 
		if(p->next->data->GiveUpTime==0&&floor!=p->next->data->Infloor) {
			PrintClientInfo(*(p->next->data),GiveUp);
			TotalTime+=Time-CInTime(*(p->next->data));
			QDelNode(Q,p);//将放弃等待的人删除
			GiveUpNumber++;
		}
		else	p->next->data->GiveUpTime--;

	return OK;
}

void PrintQueue(Queue Q) {
	//输出队列
	QueuePtr q;
	int count=0;
	if(Q.front->next==NULL) goto end;
	q=Q.front->next;
	while(q!=NULL) {
		cout<<setw(3)<<q->data->ClientID<<' ';
		q=q->next;
		count++;
	}
end:
	while(count++<=4) cout<<"    ";
}

Status InitStack(ClientStack &S) {
	//构造一个空栈
	S.base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!S.base) return OVERFLOW;
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
	return OK;
}//InitStack
Status DestroyStack(ClientStack &S) {
	//销毁栈S
	SElemType	*p;
	if(S.base) {
		for(p=S.base;p<S.top;p++)
			DestoryClient(*p);
		free(S.base);
	}
	return OK;
}
Status ClearStack(ClientStack &S) {
	//把S置为空
	if(!S.base) return ERROR; 
	S.top=S.base;
	return OK;
}
Status StackEmpty(ClientStack S) {
	//若栈S为空，则返回TRUE，否则返回FALSE
	if(S.top==S.base) return TRUE;
	else			  return FALSE;
}
int StackLength(ClientStack S) {
	//返回栈S的长度
	return S.top-S.base;
}
Status GetTop(ClientStack S,SElemType &e) {
	//返回栈顶元素
	if(!S.base) return ERROR;
	e=*(S.top-1);
	return OK;
}//GetTop
Status Push(ClientStack &S,SElemType e) {
	//入栈
	if(!S.base) return ERROR;
	if(S.top-S.base>=S.stacksize) {
		S.base=(SElemType *)realloc(S.base,(S.stacksize+STACKINCREMENT)*sizeof(SElemType));
		if(!S.base) return OVERFLOW;
		S.top=S.base+S.stacksize;
		S.stacksize+=STACKINCREMENT;
	}
	*S.top++=e;
	return OK;
}//Push
Status Pop(ClientStack &S,SElemType &e) {
	//出栈
	if(S.top==S.base) return ERROR;
	e=*(--S.top);
	return OK;
}//Pop

void PrintStack(ClientStack &S) {
	//输出栈
	SElemType *i;
	i=S.base;
	while(i<S.top)
		cout<<(*i++)->ClientID<<' ';
}

void InitEle(Elevator &E) {
	//初始化电梯类型
	int i;
	E.floor=1;//电梯初始停在第一层
	E.status=Idle;E.Count=OverTime;
	E.Stage=Down;
	E.ClientNumber=0;
	for(i=0;i<=Maxfloor;i++) {
		E.CallUp[i]=0;E.CallDown[i]=0;E.CallCar[i]=0;
	}
	for(i=0;i<=Maxfloor;i++) InitStack(E.S[i]);
}
Status CountOver(Elevator &E) {
	//判断电梯计时是否完成
	if(E.Count) {
		E.Count--;return FALSE;
	}
	return TRUE;
}
void DestoryEle(Elevator &E) {
	//销毁电梯类型
	int i;
	for(i=0;i<=Maxfloor;i++) DestroyStack(E.S[i]);
}
int EleFloor(Elevator const &E) {
	//返回电梯所在的层
	return E.floor;
}
ElevatorStatus EleStatus(Elevator const &E) {
	//返回电梯状态
	return E.status;
}
Status RequireAbove(Elevator const &E) {
	//判断是否有高层请求
	for(int i=E.floor+1;i<=Maxfloor;i++)
		if(E.CallCar[i]||E.CallDown[i]||E.CallUp[i]) return TRUE;
	return FALSE;
}
Status RequireBelow(Elevator const &E) {
	//判断是否有低层请求
	for(int i=E.floor-1;i>=Minfloor;i--)
		if(E.CallCar[i]||E.CallDown[i]||E.CallUp[i]) return TRUE;
	return FALSE;
}
Status EleAchieved(Elevator &E) {
	//判断电梯是否要停于当前层
	if(E.CallCar[E.floor]) return TRUE;
	if(E.Stage==Up&&E.CallUp[E.floor]||E.Stage==Down&&E.CallDown[E.floor])
		return TRUE;
	if(E.Stage==Up&&E.CallDown[E.floor]&&!RequireAbove(E)) {
		E.Stage=Down;return TRUE;
	}
	if(E.Stage==Down&&E.CallUp[E.floor]&&!RequireBelow(E)) {
		E.Stage=Up;return TRUE;
	}
	return FALSE;
}
Status EleOpenDoor(Elevator &E) {
	//判断电梯是否要开门
	if(E.CallCar[E.floor]||E.CallDown[E.floor]&&E.Stage==Down||E.CallUp[E.floor]&&E.Stage==Up)
		return TRUE;
	if(E.status==Idle) {
		if(E.CallDown[E.floor]) {E.Stage=Down;return TRUE;}
		if(E.CallUp[E.floor]) {E.Stage=Up;return TRUE;}
	}
	return FALSE;
}
//*******************************************************************
ElevatorStage EleDecide(Elevator &E) {
	//判断电梯动作
	int Above,Below;
	Above=RequireAbove(E);
	Below=RequireBelow(E);
	//无请求则停止
	if(Above==0&&Below==0) return Stop;
	//有请求则按请求移动
	else {
		if(E.Stage==Up) {
			if(Above!=0) return Up;
			else {
				E.Stage=Down;return Down;
			}
		}//if
		else {
			if(Below!=0) return Down;
			else {
				E.Stage=Up;return Up;
			}
		}//if
	}
}
Action ElevatorRun(Elevator &E){
	//电梯状态转换
	switch(E.status) {
		case Opening:
			//完成开门则转入Opened状态
			E.status=Opened;E.Count=CloseTestTime;
			return DoorOpened;
		case Opened:
			//进行关门测试
			if(E.Stage==Down&&!E.CallCar[E.floor]&&!E.CallDown[E.floor]||
				E.Stage==Up&&!E.CallCar[E.floor]&&!E.CallUp[E.floor]) {//无人进出，关门
				E.status=Closing;E.Count=DoorTime;
			}//if
			break;
		case Closing:
			//完成关门则转入Closed状态
			E.status=Closed;
			return DoorClosed;
		case Idle:
			//不在第一层且超出所规定的停候时间,电梯向第一层移动
			if(E.Count==0) {
				if(E.floor!=1) E.CallCar[1]=1;
			}
			else E.Count--;
			//如果有人可以进入，则开门
			if(EleOpenDoor(E)) {
				E.status=Opening;E.Count=DoorTime;break;
			}
		case Closed:
			//根据EleDecide的返回值设定电梯状态
			switch(EleDecide(E)) {
				case Up: E.status=Moving;E.Count=UpTime+Accelerate;return GoingUp;
				case Down: E.status=Moving;E.Count=DownTime+Accelerate;return GoingDown;
				case Stop:if(E.status!=Idle) {E.status=Idle;E.Count=OverTime;}
			};//switch
			break;
		case Moving:
			//完成移动
			if(E.Stage==Up) E.floor++;
			else			E.floor--;
			if(EleAchieved(E)) {//到达目标层，转入减速状态
				E.status=Decelerate;
				E.Count=DownDecelerate;
			}
			else E.Count+=DownTime;//未到达目标层，继续下降
			return Achieved;
		case Decelerate:
			//完成减速
			//确定正确的电梯时期
			if(E.Stage==Up&&!E.CallUp[E.floor]&&!RequireAbove(E)) E.Stage=Down;
			else if(E.Stage==Down&&!E.CallDown[E.floor]&&!RequireBelow(E)) E.Stage=Up;
			//转到开门状态
			E.status=Opening;E.Count=DoorTime;
			break;
	};//switch
	return None;
}//ElevatorRun

void InOut(Elevator &E,Queue w[Maxfloor+1][2]) {
	//进行乘客的进出电梯活动
	//注意：电梯时期要正确，否则乘客无法进入。
	Client *p;
	if(E.CallCar[E.floor]) //人要从电梯中走出
		if(StackEmpty(E.S[E.floor])) E.CallCar[E.floor]=0;
		else {//当前层的乘客栈非空，出电梯
			Pop(E.S[E.floor],p);E.ClientNumber--;
			InOutCount=InOutTime;
			PrintClientInfo(*p,Out);
			TotalTime+=Time-CInTime(*p);
			DestoryClient(p);
		}//else
	if(E.CallCar[E.floor]==0) //有人要走入电梯
		if(!QueueEmpty(w[E.floor][E.Stage])) {//若队列不空，继续进电梯
			DeQueue(w[E.floor][E.Stage],p);
			Push(E.S[COutfloor(*p)],p);
			if(E.CallCar[COutfloor(*p)]!=1) {
				//按下要去楼层的按钮
				E.CallCar[COutfloor(*p)]=1;
			}
			E.ClientNumber++;
			InOutCount=InOutTime;
			PrintClientInfo(*p,In);
		}//if
		else {//乘客的进出活动已完成
			if(E.Stage==Down) E.CallDown[E.floor]=0;//将相应的下降按钮取消
			else E.CallUp[E.floor]=0;//将相应的上升按钮取消
		}
}

void NewClient(Elevator &E,Queue w[5][2]) {
	//进入新乘客
	Client *p;
	CreatClient(p);//新的乘客
	//将该乘客插入相应队列并按下相应按钮(Up/Down)
	if(GoAbove(*p)) {
		EnQueue(w[CInfloor(*p)][Up],p);E.CallUp[CInfloor(*p)]=1;
	}
	else {
		EnQueue(w[CInfloor(*p)][Down],p);E.CallDown[CInfloor(*p)]=1;
	}//else
}
/**********************************************************/
void PrintStatus(Elevator &E,Queue w[5][2]) {
	//输出当前状态
	for(int i=Maxfloor;i>=0;i--){
		cout<<"第"<<setw(2)<<i<<"层";
		if(i==Maxfloor) cout<<"↓";
		else	cout<<"↑";
		if(i==Maxfloor) {
			PrintQueue(w[i][Down]);
			cout<<"‖";
			PrintStack(E.S[i]);
			cout<<endl;
		}
		if(i==0) {
			PrintQueue(w[i][Up]);
			cout<<"‖";
			PrintStack(E.S[i]);
			cout<<endl;
		}
		if(i!=Maxfloor&&i!=0) {
			PrintQueue(w[i][Up]);
			cout<<"‖";
			PrintStack(E.S[i]);
			cout<<endl<<"      ↓";
			PrintQueue(w[i][Down]);
			cout<<"‖";
			cout<<endl;
		}
	}
}

void Print(Elevator &E,Action a) {
	//输出电梯动作信息
	switch(a) {
		case DoorOpened:
			cout<<Time<<"\t电梯门已打开"<<endl;break;
		case DoorClosed:
			cout<<Time<<"\t电梯门已关闭"<<endl;break;
		case Achieved:
			cout<<Time<<"\t电梯已到达第"<<E.floor<<"层。"<<endl;break;
		case GoingUp:
			cout<<Time<<"\t电梯上升中..."<<endl;break;
		case GoingDown:
			cout<<Time<<"\t电梯下降中..."<<endl;break;
		default:break;
	};//switch
}

int main() {
	Elevator E;//电梯
	Queue	w[Maxfloor+1][2];//每层都有两个等待队列：上楼队列和下楼队列
	//初始化
	InitEle(E);
	srand( (unsigned)time( NULL ));
	for(int i=0;i<=Maxfloor;i++) {
		InitQueue(w[i][Up]);
		InitQueue(w[i][Down]);
	}
#if test
	MaxTime=10000;//12099999
#else
	cout<<"请输入电梯的运行时间：";
	cin>>MaxTime;
#endif
	while(Time++<MaxTime) {
		//---------------乘客事件----------------
		//新乘客进入事件
		if(InterTime==0) NewClient(E,w);
		else InterTime--;
		//乘客放弃事件放弃
		for(int i=0;i<=Maxfloor;i++)
			for(int j=0;j<2;j++) {
				CGiveUp(w[i][j],EleFloor(E));
			}
		//乘客进出事件
		if(InOutCount==0) {
			if(EleStatus(E)==Opened) InOut(E,w);
		}
		else InOutCount--;
		//---------------电梯事件----------------
		if(CountOver(E)||EleStatus(E)==Closed||EleStatus(E)==Idle) {
			Action a;
			a=ElevatorRun(E);

			Print(E,a);
			if(EleStatus(E)==Decelerate) PrintStatus(E,w);

		}

		Sleep(100);//延迟0.1s
	}
	cout<<"共"<<ID<<"人进入系统，";
	cout<<"共"<<GiveUpNumber<<"人放弃。"<<endl;
	cout<<"平均等待时间为："<<TotalTime/(ID)<<endl;
	DestoryEle(E);
	for(int i=0;i<=Maxfloor;i++) {
		DestroyQueue(w[i][Up]);
		DestroyQueue(w[i][Down]);
	}
	return 0;
}
