#ifndef _Bank_H_
#define _Bank_H_
#include <string>
using namespace std;

/*�ͻ��������*/
struct CustNode{  
    int num; 						//�ͻ���  
    string Type; 					//������뿪  
    int BeginTime;					//����ʱ��  
    int EndTime;					//�뿪ʱ��  
    int Amount; 					//����Ϊ������Ϊȡ��  
    CustNode *next;					//ָ����  
};  

/*�ȴ���������*/
struct Queue{ 						//����  
    CustNode *front;				//����ͷָ��  
    CustNode *rear;					//����βָ��  
};

/*���ñ����Ķ���*/  
int BankAmount; 					//��ʼʱ�����ִ��ʽ��ܶ�  
int CloseTime; 						//Ӫҵ����ʱ��  
int ClientArriveMaxTime; 			//���������¼�֮��ļ������  
int ClientArriveMinTime; 			//���������¼�֮��ļ������  
int DealMaxTime; 					//�ͻ�֮�佻�׵�ʱ������  
int DealMinTime; 					//�ͻ�֮�佻�׵�ʱ������  
int MaxAmount; 						//���׶�����  
int NeedIn=0;						//��Ҫ��������  
int NeedOut=0;						//��Ҫȡ�������  
int SuccessIn=0;					//�ɹ���������  
int SuccessOut=0;					//�ɹ�ȡ�������  
int CurrentTime=0; 					//��ǰʱ��  
int BankAmountTime=0; 				//�ͻ�������ʱ��  
int counter=0; 						//�ͻ�����  
int number=1; 						//��ʼ�ͻ����к�  
bool state=1; 						//�����ж��Ƿ��д����ڴ���  
int DealTime=0;						//����ʱ��  
int MaxTime=0;						//��󵽴�ʱ��  
Queue Event; 						//�¼�����  
Queue Q1; 							//����һ  
Queue Q2; 							//���ж� 

#endif
