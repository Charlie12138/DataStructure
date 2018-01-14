#ifndef	_HEAD_H 
#define _HEAD_H 
#include<time.h>
#define FALSE 0 
#define TRUE 1
#define ERROR 0
#define Empty 0 
#define OK 1
#define	STACK_INIT_SIZE	10			//�洢�ռ��ʼ������
#define	STACKINCREMENT	5			//�洢�ռ��������
#define Maxfloor 4            		//��߲�
#define Minfloor 0					//��ײ�
#define StackInitSize 10			//�˿�ջ�洢�ռ��ʼ������
#define StackIncrement 5			//�˿�ջ�洢�ռ�������� 
#define CloseTestTime 40			//���ݹ��Ų���ʱ��
#define OverTime 300				//����ͣ�ʱʱ��
#define Accelerate 15				//����ʱ��
#define UpTime 5					//����ʱ��
#define DownTime 15					//�½�ʱ��
#define UpDecelerate 5				//��������
#define DownDecelerate 5			//�½�����
#define DoorTime 20					//���Ź���ʱ��
#define InOutTime 25				//��������ʱ�� 
long MaxTime;						//ϵͳ�����ʱ��
int InOutCount=0;					//���ڽ�����ʱ
int InterTime=0;					//��һ�˿ͽ���ϵͳ��ʱ��
int ID=0;							//�˿ͱ��
int GiveUpNumber=0;					//�˿ͷ��������� 
int TotalTime=0;					//�ܹ��ȴ�ʱ��
typedef int Status;    
float Time=0;						// 
enum ClientStatus{					//�ͻ�״̬ 
	New,							
	GiveUp,
	In,
	Out,
	Finish
};  
enum ElevatorStatus{				//����״̬ 
	Opening,						//���ڿ��� 
	Opened,							//�ѿ��� 
	Closing,						//���ڹ��� 
	Closed,							//�ѹ��� 
	Moving,							//�ƶ� 
	Decelerate,   					//���� 
	Idle 							//ͣ�� 
}; 

enum ElevatorStage{					//���ݽ׶� 
	Up,								//���� 
	Down,							//�½� 
	OpenDoor,						//���� 
	Stop 							//ֹͣ 
};

enum Action{
	DoorOpened,						//�ѿ��� 
	DoorClosed,						//�ѹ��� 
	GoingUp,						//���� 
	GoingDown,						//�½� 
	Achieved,						//ֹͣ 
	None
};

 
typedef struct{				//�˿����� 
	int ClientID;         	//�˿ͱ�� 
	int InTime;				//�˿ͽ���ʱ�� 
	int Infloor;        	//�˿ͽ����¥��
	int Outfloor;       	//�˿�Ҫȥ��¥�� 
	int GiveUpTime;     	//�������̵����Ⱥ�ʱ��  
}Client;

typedef Client *QElemType;
typedef struct QNode{ 		//�Ⱥ���� 
	QElemType data;			//������ 
	struct QNode *next;		//ָ���� 
}QNode,*QueuePtr;
typedef struct{
	QueuePtr front;			//��ͷָ��
	QueuePtr rear;			//��βָ�� 
}Queue;

typedef Client *SElemType;
typedef struct{				//�˿�ջ 
	SElemType *base;		//ջ��ָ��
	SElemType *top;			//ջ��ָ��
	int stacksize;			//��ǰ�ѷ���Ĵ洢�ռ� 
}ClientStack; 

typedef struct{
	int floor;   						//���ݵ�ǰ���ڲ�
	int ClientNumber;					//����������
	ElevatorStatus status;				//���ݵ�ǰ״̬
	ElevatorStage Stage;				//�������н׶�
	int Count;							//���ڵ��ݼ�ʱ
	int CallUp[Maxfloor+1];				//ÿ���Up��ť
	int CallDown[Maxfloor+1];			//ÿ���Down��ť
	int CallCar[Maxfloor+1];			//������Ŀ��㰴ť
	ClientStack S[Maxfloor+1];			//�˿�ջ��Ҫȥ��ͬ¥����˷��ڲ�ͬ��ջ��  
}Elevator;

#endif
