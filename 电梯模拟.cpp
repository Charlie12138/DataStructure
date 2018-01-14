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
//����˿���Ϣ
	switch(s) {
		case New:cout<<Time<<'\t'<<p.ClientID<<"�ų˿ͽ����"<<p.Infloor<<"��"<<endl;break;
		case GiveUp:cout<<Time<<"\t"<<p.ClientID<<"�ų˿ͷ����ȴ�, �ȴ���"<<Time-p.InTime<<"/0.1s"<<endl;break;
		case Out:cout<<Time<<'\t'<<p.ClientID<<"�ų˿��߳�����"<<endl;break;
		case In:cout<<Time<<'\t'<<p.ClientID<<"�ų˿��߽����ݣ�Ҫȥ��"<<p.Outfloor<<"��"<<endl;break;
		default:break;
	};

}
Status CreatClient(Client *&p){
	//�����µĳ˿�
	p=new Client;							//�½��˿�����	 
	if(!p) 									//�½��˿�ʧ�� 
		return OVERFLOW;
	p->ClientID=ID++;						//�����˿ͱ�� 
	p->GiveUpTime=rand()%120+30;			//�����˿��������̵ĵȴ�ʱ��
	p->InTime=Time;							//�����˿ͽ������ϵͳ��ʱ�� 
	InterTime=rand()%300;					//������һ�˿�Ҫ�����ʱ��
	p->Outfloor=rand()%(Maxfloor+1);		//�����˿���Ҫ�����¥��
	//�ó˿ͳ��ֵ�¥��
	while((p->Infloor=rand()%(Maxfloor+1))==p->Outfloor);
	//cin>>p->Infloor>>p->Outfloor>>InterTime;
	PrintClientInfo(*p,New);
	return OK;
}
Status DestoryClient(Client *&p) {
	//�ó˿��뿪ϵͳ
	delete p;
	p=NULL;
	return OK;
}
Status GoAbove(Client const &e) {
	//�жϸó˿��Ƿ�ȥ���߲�
	if(e.Outfloor>e.Infloor) return TRUE;
	else return FALSE;
}
int CInfloor(Client const &e) {
	//���س˿ͽ����¥��
	return e.Infloor;
}
int CInTime(Client const &e) {
	//���س˿ͽ���ʱ��
	return e.InTime;
}
int COutfloor(Client const &e) {
	//���س˿ͽ���ʱ��
	return e.Outfloor;
}

Status InitQueue(Queue &Q) {
	//����һ���ն���Q
	Q.front=Q.rear=new QNode;
	if(!Q.front) return OVERFLOW;//����洢ʧ��
	Q.front->next=NULL;
	Q.front->data=NULL;
	return OK;
}

Status DestroyQueue(Queue &Q) {
	//���ٶ���Q
	while(Q.front) {
		Q.rear=Q.front->next;
		if(Q.front->data) DestoryClient(Q.front->data);
		delete Q.front;
		Q.front=Q.rear;
	}
	return OK;
}

Status EnQueue(Queue &Q,QElemType e) {
	//����Ԫ��eΪQ���µĶ�βԪ��
	QueuePtr p;
	p=new QNode;
	if(!p) return OVERFLOW;
	p->data=e;p->next=NULL;
	Q.rear->next=p;
	Q.rear=p;
	return OK;
}
Status DeQueue(Queue &Q,QElemType &e) {
	//�����в���,��ɾ��Q�Ķ�ͷԪ��,��e������ֵ,������OK;
	//���򷵻�ERROR
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
	//�ж϶����Ƿ�Ϊ��
	if(Q.front==Q.rear) return TRUE;
	else	return FALSE;
}

Status QDelNode(Queue &Q,QueuePtr p) {
	//ɾ��������pָ��Ľ�����һ�����
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
	//ɾ�������ȴ��ĳ˿�
	QueuePtr p;
	p=Q.front;
	if(p->next!=NULL) 
		if(p->next->data->GiveUpTime==0&&floor!=p->next->data->Infloor) {
			PrintClientInfo(*(p->next->data),GiveUp);
			TotalTime+=Time-CInTime(*(p->next->data));
			QDelNode(Q,p);//�������ȴ�����ɾ��
			GiveUpNumber++;
		}
		else	p->next->data->GiveUpTime--;

	return OK;
}

void PrintQueue(Queue Q) {
	//�������
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
	//����һ����ջ
	S.base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!S.base) return OVERFLOW;
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
	return OK;
}//InitStack
Status DestroyStack(ClientStack &S) {
	//����ջS
	SElemType	*p;
	if(S.base) {
		for(p=S.base;p<S.top;p++)
			DestoryClient(*p);
		free(S.base);
	}
	return OK;
}
Status ClearStack(ClientStack &S) {
	//��S��Ϊ��
	if(!S.base) return ERROR; 
	S.top=S.base;
	return OK;
}
Status StackEmpty(ClientStack S) {
	//��ջSΪ�գ��򷵻�TRUE�����򷵻�FALSE
	if(S.top==S.base) return TRUE;
	else			  return FALSE;
}
int StackLength(ClientStack S) {
	//����ջS�ĳ���
	return S.top-S.base;
}
Status GetTop(ClientStack S,SElemType &e) {
	//����ջ��Ԫ��
	if(!S.base) return ERROR;
	e=*(S.top-1);
	return OK;
}//GetTop
Status Push(ClientStack &S,SElemType e) {
	//��ջ
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
	//��ջ
	if(S.top==S.base) return ERROR;
	e=*(--S.top);
	return OK;
}//Pop

void PrintStack(ClientStack &S) {
	//���ջ
	SElemType *i;
	i=S.base;
	while(i<S.top)
		cout<<(*i++)->ClientID<<' ';
}

void InitEle(Elevator &E) {
	//��ʼ����������
	int i;
	E.floor=1;//���ݳ�ʼͣ�ڵ�һ��
	E.status=Idle;E.Count=OverTime;
	E.Stage=Down;
	E.ClientNumber=0;
	for(i=0;i<=Maxfloor;i++) {
		E.CallUp[i]=0;E.CallDown[i]=0;E.CallCar[i]=0;
	}
	for(i=0;i<=Maxfloor;i++) InitStack(E.S[i]);
}
Status CountOver(Elevator &E) {
	//�жϵ��ݼ�ʱ�Ƿ����
	if(E.Count) {
		E.Count--;return FALSE;
	}
	return TRUE;
}
void DestoryEle(Elevator &E) {
	//���ٵ�������
	int i;
	for(i=0;i<=Maxfloor;i++) DestroyStack(E.S[i]);
}
int EleFloor(Elevator const &E) {
	//���ص������ڵĲ�
	return E.floor;
}
ElevatorStatus EleStatus(Elevator const &E) {
	//���ص���״̬
	return E.status;
}
Status RequireAbove(Elevator const &E) {
	//�ж��Ƿ��и߲�����
	for(int i=E.floor+1;i<=Maxfloor;i++)
		if(E.CallCar[i]||E.CallDown[i]||E.CallUp[i]) return TRUE;
	return FALSE;
}
Status RequireBelow(Elevator const &E) {
	//�ж��Ƿ��еͲ�����
	for(int i=E.floor-1;i>=Minfloor;i--)
		if(E.CallCar[i]||E.CallDown[i]||E.CallUp[i]) return TRUE;
	return FALSE;
}
Status EleAchieved(Elevator &E) {
	//�жϵ����Ƿ�Ҫͣ�ڵ�ǰ��
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
	//�жϵ����Ƿ�Ҫ����
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
	//�жϵ��ݶ���
	int Above,Below;
	Above=RequireAbove(E);
	Below=RequireBelow(E);
	//��������ֹͣ
	if(Above==0&&Below==0) return Stop;
	//�������������ƶ�
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
	//����״̬ת��
	switch(E.status) {
		case Opening:
			//��ɿ�����ת��Opened״̬
			E.status=Opened;E.Count=CloseTestTime;
			return DoorOpened;
		case Opened:
			//���й��Ų���
			if(E.Stage==Down&&!E.CallCar[E.floor]&&!E.CallDown[E.floor]||
				E.Stage==Up&&!E.CallCar[E.floor]&&!E.CallUp[E.floor]) {//���˽���������
				E.status=Closing;E.Count=DoorTime;
			}//if
			break;
		case Closing:
			//��ɹ�����ת��Closed״̬
			E.status=Closed;
			return DoorClosed;
		case Idle:
			//���ڵ�һ���ҳ������涨��ͣ��ʱ��,�������һ���ƶ�
			if(E.Count==0) {
				if(E.floor!=1) E.CallCar[1]=1;
			}
			else E.Count--;
			//������˿��Խ��룬����
			if(EleOpenDoor(E)) {
				E.status=Opening;E.Count=DoorTime;break;
			}
		case Closed:
			//����EleDecide�ķ���ֵ�趨����״̬
			switch(EleDecide(E)) {
				case Up: E.status=Moving;E.Count=UpTime+Accelerate;return GoingUp;
				case Down: E.status=Moving;E.Count=DownTime+Accelerate;return GoingDown;
				case Stop:if(E.status!=Idle) {E.status=Idle;E.Count=OverTime;}
			};//switch
			break;
		case Moving:
			//����ƶ�
			if(E.Stage==Up) E.floor++;
			else			E.floor--;
			if(EleAchieved(E)) {//����Ŀ��㣬ת�����״̬
				E.status=Decelerate;
				E.Count=DownDecelerate;
			}
			else E.Count+=DownTime;//δ����Ŀ��㣬�����½�
			return Achieved;
		case Decelerate:
			//��ɼ���
			//ȷ����ȷ�ĵ���ʱ��
			if(E.Stage==Up&&!E.CallUp[E.floor]&&!RequireAbove(E)) E.Stage=Down;
			else if(E.Stage==Down&&!E.CallDown[E.floor]&&!RequireBelow(E)) E.Stage=Up;
			//ת������״̬
			E.status=Opening;E.Count=DoorTime;
			break;
	};//switch
	return None;
}//ElevatorRun

void InOut(Elevator &E,Queue w[Maxfloor+1][2]) {
	//���г˿͵Ľ������ݻ
	//ע�⣺����ʱ��Ҫ��ȷ������˿��޷����롣
	Client *p;
	if(E.CallCar[E.floor]) //��Ҫ�ӵ������߳�
		if(StackEmpty(E.S[E.floor])) E.CallCar[E.floor]=0;
		else {//��ǰ��ĳ˿�ջ�ǿգ�������
			Pop(E.S[E.floor],p);E.ClientNumber--;
			InOutCount=InOutTime;
			PrintClientInfo(*p,Out);
			TotalTime+=Time-CInTime(*p);
			DestoryClient(p);
		}//else
	if(E.CallCar[E.floor]==0) //����Ҫ�������
		if(!QueueEmpty(w[E.floor][E.Stage])) {//�����в��գ�����������
			DeQueue(w[E.floor][E.Stage],p);
			Push(E.S[COutfloor(*p)],p);
			if(E.CallCar[COutfloor(*p)]!=1) {
				//����Ҫȥ¥��İ�ť
				E.CallCar[COutfloor(*p)]=1;
			}
			E.ClientNumber++;
			InOutCount=InOutTime;
			PrintClientInfo(*p,In);
		}//if
		else {//�˿͵Ľ���������
			if(E.Stage==Down) E.CallDown[E.floor]=0;//����Ӧ���½���ťȡ��
			else E.CallUp[E.floor]=0;//����Ӧ��������ťȡ��
		}
}

void NewClient(Elevator &E,Queue w[5][2]) {
	//�����³˿�
	Client *p;
	CreatClient(p);//�µĳ˿�
	//���ó˿Ͳ�����Ӧ���в�������Ӧ��ť(Up/Down)
	if(GoAbove(*p)) {
		EnQueue(w[CInfloor(*p)][Up],p);E.CallUp[CInfloor(*p)]=1;
	}
	else {
		EnQueue(w[CInfloor(*p)][Down],p);E.CallDown[CInfloor(*p)]=1;
	}//else
}
/**********************************************************/
void PrintStatus(Elevator &E,Queue w[5][2]) {
	//�����ǰ״̬
	for(int i=Maxfloor;i>=0;i--){
		cout<<"��"<<setw(2)<<i<<"��";
		if(i==Maxfloor) cout<<"��";
		else	cout<<"��";
		if(i==Maxfloor) {
			PrintQueue(w[i][Down]);
			cout<<"��";
			PrintStack(E.S[i]);
			cout<<endl;
		}
		if(i==0) {
			PrintQueue(w[i][Up]);
			cout<<"��";
			PrintStack(E.S[i]);
			cout<<endl;
		}
		if(i!=Maxfloor&&i!=0) {
			PrintQueue(w[i][Up]);
			cout<<"��";
			PrintStack(E.S[i]);
			cout<<endl<<"      ��";
			PrintQueue(w[i][Down]);
			cout<<"��";
			cout<<endl;
		}
	}
}

void Print(Elevator &E,Action a) {
	//������ݶ�����Ϣ
	switch(a) {
		case DoorOpened:
			cout<<Time<<"\t�������Ѵ�"<<endl;break;
		case DoorClosed:
			cout<<Time<<"\t�������ѹر�"<<endl;break;
		case Achieved:
			cout<<Time<<"\t�����ѵ����"<<E.floor<<"�㡣"<<endl;break;
		case GoingUp:
			cout<<Time<<"\t����������..."<<endl;break;
		case GoingDown:
			cout<<Time<<"\t�����½���..."<<endl;break;
		default:break;
	};//switch
}

int main() {
	Elevator E;//����
	Queue	w[Maxfloor+1][2];//ÿ�㶼�������ȴ����У���¥���к���¥����
	//��ʼ��
	InitEle(E);
	srand( (unsigned)time( NULL ));
	for(int i=0;i<=Maxfloor;i++) {
		InitQueue(w[i][Up]);
		InitQueue(w[i][Down]);
	}
#if test
	MaxTime=10000;//12099999
#else
	cout<<"��������ݵ�����ʱ�䣺";
	cin>>MaxTime;
#endif
	while(Time++<MaxTime) {
		//---------------�˿��¼�----------------
		//�³˿ͽ����¼�
		if(InterTime==0) NewClient(E,w);
		else InterTime--;
		//�˿ͷ����¼�����
		for(int i=0;i<=Maxfloor;i++)
			for(int j=0;j<2;j++) {
				CGiveUp(w[i][j],EleFloor(E));
			}
		//�˿ͽ����¼�
		if(InOutCount==0) {
			if(EleStatus(E)==Opened) InOut(E,w);
		}
		else InOutCount--;
		//---------------�����¼�----------------
		if(CountOver(E)||EleStatus(E)==Closed||EleStatus(E)==Idle) {
			Action a;
			a=ElevatorRun(E);

			Print(E,a);
			if(EleStatus(E)==Decelerate) PrintStatus(E,w);

		}

		Sleep(100);//�ӳ�0.1s
	}
	cout<<"��"<<ID<<"�˽���ϵͳ��";
	cout<<"��"<<GiveUpNumber<<"�˷�����"<<endl;
	cout<<"ƽ���ȴ�ʱ��Ϊ��"<<TotalTime/(ID)<<endl;
	DestoryEle(E);
	for(int i=0;i<=Maxfloor;i++) {
		DestroyQueue(w[i][Up]);
		DestroyQueue(w[i][Down]);
	}
	return 0;
}
