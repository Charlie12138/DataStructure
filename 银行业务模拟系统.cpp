#include<iostream>  
#include<stdio.h>    
#include<conio.h>  
#include<time.h>   
#include<windows.h>  
#include<stdlib.h>
#include"Bank.h"  
using namespace std;  

//===========���еĻ�������========== 
void InitQueue(Queue &Q){  
//��ʼ������������һ���ն���
    Q.front=Q.rear=(CustNode*)malloc(sizeof(CustNode));  
    if(!(Q.front))  
         exit(1);  
    Q.front->next=0;  
 }  
 
void EnQueue(Queue &Q,int e){  
//�����
//����Ԫ��eΪ����Q���µĶ�βԪ��
    CustNode* p=new CustNode;  
    p->Amount=e;  
    p->next=NULL; 
    if(Q.front==NULL){		
    //����Ϊ�գ���ʼ��    
		Q.front=p;  
        Q.rear=p;  
    }  
    else{ 					  
    //���в�Ϊ�գ�����Ԫ��e    
		Q.rear->next=p;  
        Q.rear=Q.rear->next;  
    }  
}  
  
  
void DeQueue(Queue &Q){
//������  
    CustNode *p;  
    p=Q.front;  
    if(Q.front->next==NULL)		//����ֻ��һ��Ԫ��  
        Q.front=Q.rear=NULL;  
    else  						//��������ͷָ�� 
        Q.front=Q.front->next;  
    delete p;  
}  
  

CustNode *Queuefront(Queue &Q){ 
//���ض���Ԫ�� 
    return Q.front;  
}  
  
  
CustNode *Queuerear(Queue &Q){  
//���ض�βԪ��
    return Q.rear;  
}  

//==========�¼����͵Ļ�������========= 
void ClientArrive(){
//�ͻ������¼�����
//��������˿ͣ��������һ���������¼� �����¼�����  
    EnQueue(Q1,(rand()%(2*MaxAmount)-MaxAmount));  			//��������˿ͼ����һ����  
    Queuerear(Q1)->BeginTime=CurrentTime;					//��ǰʱ��Ϊ�ͻ��ĵ���ʱ��  
    Queuerear(Q1)->num=number;								//�ͻ���Ϊ�ͻ����к�  
    EnQueue(Event,(Queuerear(Q1)->Amount)); 				//�������¼������¼�����  
    Queuerear(Event)->BeginTime=CurrentTime;  
    Queuerear(Event)->Type="����";  
    Queuerear(Event)->num=number;  
    number++;  
}  
  
  
void InAmount(){
//����¼����� 
    BankAmount+=Queuefront(Q1)->Amount; 									//�����ʽ��ܶ�  
    EnQueue(Event,Queuefront(Q1)->Amount); 									//�����¼�����   
    Queuerear(Event)->Type="�뿪";  
    Queuerear(Event)->num=Queuefront(Q1)->num;  
    //�뿪ʱ��Ϊ����ʱ�������������Ľ��������ʱ�����С����ʱ��Ĵ���ʱ��
    Queuerear(Event)->EndTime=(Queuefront(Q1)->BeginTime+rand()%(DealMaxTime-DealMinTime +1)+DealMinTime);  
    counter++; 																//���¿ͻ�����  
    BankAmountTime+=(Queuerear(Event)->EndTime-Queuefront(Q1)->BeginTime); 	//���¶���ʱ��  
    DeQueue(Q1); 															//ɾ����һ���е�һ��ҵ��  
    DealTime=Queuerear(Event)->EndTime;										//����ʱ��Ϊ�ͻ����뿪ʱ��  
    state=0;																//����û�н�����Ҫ����  
}  
  
  
void OutAmount(){  
//ȡ���¼����� 
    if((-Q1.front->Amount)>BankAmount){		   
    //�ʽ��ȱ ����ڶ�����  
        EnQueue(Q2,Queuefront(Q1)->Amount);  
        Queuerear(Q2)->BeginTime=Queuefront(Q1)->BeginTime;  
        Queuerear(Q2)->num=Queuefront(Q1)->num;  
        DeQueue(Q1);  
    }  
    else{  
        BankAmount+=Queuerear(Q1)->Amount;				//�����ʽ��ܶ�  
        EnQueue(Event,Queuefront(Q1)->Amount); 			//�����¼�����   
        Queuerear(Event)->Type="�뿪";  
        Queuerear(Event)->num=Queuefront(Q1)->num;  
        //�ͻ����뿪ʱ��Ϊ�ͻ��ĵ���ʱ�������������Ľ��������ʱ�����С����ʱ��Ĵ���ʱ��
        Queuerear(Event)->EndTime=(Queuefront(Q1)->BeginTime +rand()%(DealMaxTime-DealMinTime +1)+DealMinTime);  
        Queuerear(Event)->BeginTime=0;  
        DealTime=Queuerear(Event)->EndTime;										//����ʱ��Ϊ�ͻ����뿪ʱ��  
        counter++; 																//���¿ͻ�����   
        BankAmountTime+=(Queuerear(Event)->EndTime-Queuerear(Q1)->BeginTime); 	//���¶���ʱ��  
        DeQueue(Q1); 															//ɾ����һ���е�һ��ҵ��  
        state=0;																//����û�н�����Ҫ����  
    }  
}  
  
  
CustNode *SearchQ2(Queue &Q,int m){  
//�ڶ���Q2�н����������Դ����Ԫ�� 
    CustNode *sign=Q.front; 				//���ͷ�ڵ�  
    CustNode *temp;  
    while(Q.front!=NULL){  
        if((-(Q.front->Amount))<m){			//����Ԫ�ؿ��Դ���  
            if(Q.front==Q.rear){  
                temp=Q.front;  
                Q.front=Q.rear=NULL;  
                return temp;  
            }  
        	else{							//����Ԫ�س���  
            	temp=Q.front;   
            	Q.front=Q.front->next; 		//�׽ڵ����һλ,����ԭ�׽ڵ�  
            	return temp;  
        	}  
        }  
        else{								//����Ԫ�ײ��ܱ�����  
            if(Q.front==Q.rear){
			}  
            else{							//�׽ڵ��Ƶ�����β��  
              
            	Q.rear->next=Q.front;   
                Q.rear=Q.rear->next;  
                Q.front=Q.front->next;  
                Q.rear->next=NULL;  
            }  
        }  
        if(Q.front==sign)					//����ѭ��һ��ʱֹͣ  
            return NULL;  
    }  
    return NULL;  
}  
  
  
  
void DealQ2(){ 
//�ڶ���Q2�ж��ڿ��Դ����Ԫ�ؽ��д��� 
   	CustNode* temped;  
	int randomTemp;
    while((temped=SearchQ2(Q2,BankAmount))&&temped!=NULL){ 					//���ҿɴ���ȡ��  
      
        BankAmount+=temped->Amount; 										//�����ʽ��ܶ�  
        EnQueue(Event,temped->Amount); 										//�����¼�����   
        Queuerear(Event)->Type="�뿪";  
        Queuerear(Event)->num=temped->num;  
        randomTemp=rand()%(DealMaxTime-DealMinTime +1)+DealMinTime;			//����ʱ��Ϊ��������Ľ��������ʱ�����С����ʱ��֮��Ĵ���ʱ��  
        Queuerear(Event)->EndTime=CurrentTime+randomTemp ;					//�ͻ��뿪ʱ��Ϊ��ǰʱ����ϴ���ʱ��  
        DealTime+=randomTemp;												//���½���ʱ��  
        counter++; 															//���¿ͻ�����  
        BankAmountTime+=(Queuerear(Event)->EndTime-temped->BeginTime); 		//���¶���ʱ��   
        delete temped; 														//ɾ���ڵ�  
        temped = NULL;  
    }  
    state = 0;  
}  
  
 
void BankOutLook(){  
//����ģ��ϵͳ��ʼ����     
    printf("   =========================================================================		\n");
	printf("   =========================================================================		\n");
    printf("                       Simulation of The Bank business								\n");
    printf("   -------------------------------------------------------------------------	  \n\n");
	printf("                       ---------------------------------                            \n");
	printf("                    	     Number: 3116004979                                     \n");
	printf("                    	     CLASS ��16�������                                 	\n");
    printf("                    	     NAME  ��  ղ����                                       \n");
    printf("                        ---------------------------------                           \n");
    printf("   =========================================================================        \n");
    printf("   *************************************************************************		\n");  
    printf("   *************************************************************************		\n");  
    printf("   *******************                               ***********************		\n");
	printf("   *******************    0.�˳�   1.����ģ��ϵͳ    ***********************		\n");   
    printf("   *******************                               ***********************		\n"); 
    printf("   *************************************************************************		\n");  
    printf("   *************************************************************************		\n");  
    printf("   ************************      ��ѡ�����      ***************************		\n");
    printf("   =========================================================================        \n");
    printf("   ������ѡ��Ĳ�����Ӧ��ţ�"); 
}    
   
int main(){ 
	system("color 70");
    BankOutLook();  
    int n,t1=0,t2=0,t3=0,t4=0,m=0;
    scanf("%d",&n);  
    while(n==1){  
        srand(time(NULL)); 							//��ʼ���������  
        printf("   ���������еĳ�ʼ��");  
        scanf("%d",&BankAmount);   
        if(BankAmount<0){                          
            printf("   ������󣡳�ʼ����С��0�����ٴ����룡\n");   
            printf("   ���������еĳ�ʼ��");  
            scanf("%d",&BankAmount);   
            if(BankAmount<0){       
                printf("   ������󣡳�ʼ����С��0�������һ�����룡\n");  
                printf("   ���������еĳ�ʼ��");  
                scanf("%d",&BankAmount);   
                if(BankAmount<0){ 
                    printf("   �������붼�����밴������˳���\n");  
                    getch();   
                    goto end;   
                }  
            }  
        }  
        
        printf("   ���������е�Ӫҵʱ�䣺");  
        scanf("%d",&CloseTime);  
        if(CloseTime>=1440){                 
            printf("   �������һ���Ӫҵʱ�䲻�ܳ���1440���ӣ�24��Сʱ�������ٴ����룡\n");  
            printf("   ���������е�Ӫҵʱ�䣺");  
            scanf("%d",&CloseTime);  
            if(CloseTime>=1440){   
                printf("   �������һ���Ӫҵʱ�䲻�ܳ���1440���ӣ�24��Сʱ���������һ�����룡\n");  
                printf("   ���������е�Ӫҵʱ�䣺");  
                scanf("%d",&CloseTime);  
                if(CloseTime>=1440){   
                    printf("   �������붼�����밴������˳���\n");  
                    getch();    
                    goto end;  
                }  
            }  
        } 
        
        printf("   ��������󵽴�ʱ������");   
        scanf("%d",&ClientArriveMaxTime);  
        if(ClientArriveMaxTime>CloseTime){         
            printf("   ���������󵽴�ʱ��������С��Ӫҵʱ�䣡���ٴ����룡\n");  
            printf("   ��������󵽴�ʱ������");   
            scanf("%d",&ClientArriveMaxTime);  
            if(ClientArriveMaxTime>CloseTime){   
                printf("   ���������󵽴�ʱ��������С��Ӫҵʱ�䣡�����һ�����룡\n");  
                printf("   ��������󵽴�ʱ������");   
                scanf("%d",&ClientArriveMaxTime);  
                if(ClientArriveMaxTime>CloseTime){  
                    printf("   �������붼�����밴������˳���\n");  
                    getch();   
                    goto end;  
                }   
            }  
        }  
        
        printf("   ��������С����ʱ������");   
        scanf("%d",&ClientArriveMinTime);  
        if(ClientArriveMinTime<=0||ClientArriveMinTime>=ClientArriveMaxTime){       
            printf("   ���������С����ʱ����������������󵽴�ʱ��֮�䣡���ٴ����룡\n");   
            printf("   ��������С����ʱ������");   
            scanf("%d",&ClientArriveMinTime);  
            if(ClientArriveMinTime<=0||ClientArriveMinTime>=ClientArriveMaxTime){  
                printf("   ���������С����ʱ����������������󵽴�ʱ��֮�䣡�����һ�����룡\n");   
                printf("   ��������С����ʱ������");   
                scanf("%d",&ClientArriveMinTime);  
                if(ClientArriveMinTime<=0||ClientArriveMinTime>=ClientArriveMaxTime){ 
                    printf("   �������붼�����밴������˳���\n");  
                    getch();  
                    printf("   �밴������˳�!\n");  
                    goto end;  
                }   
            }  
        }   
        
        printf("   ���������Ĵ���ʱ�䣺");  
        scanf("%d",&DealMaxTime);  
        if(DealMaxTime>CloseTime){           
            printf("   ������������ʱ�����С��Ӫҵʱ�䣡���ٴ����룡\n");   
            printf("   ���������Ĵ���ʱ�䣺");  
            scanf("%d",&DealMaxTime);  
            if(DealMaxTime>CloseTime){  
               	printf("   ������������ʱ�����С��Ӫҵʱ�䣡�����һ�����룡\n");   
                printf("   ���������Ĵ���ʱ�䣺");  
                scanf("%d",&DealMaxTime);  
                if(DealMaxTime>CloseTime){  
                    printf("   �������붼�����밴������˳���\n");  
                    getch();     
                    goto end;  
                }  
            }  
        }  
        
        printf("   ��������С�Ĵ���ʱ�䣺");  
        scanf("%d",&DealMinTime);     
        if(DealMinTime<=0||DealMinTime>=DealMaxTime){
            printf("   ���������С����ʱ����������������ʱ��֮�䣡���ٴ����룡\n");  
            printf("   ��������С�Ĵ���ʱ�䣺");  
            scanf("%d",&DealMinTime);     
            if(DealMinTime<=0||DealMinTime>=DealMaxTime){      
                printf("   ���������С����ʱ����������������ʱ��֮�䣡�����һ�����룡\n");  
                printf("   ��������С�Ĵ���ʱ�䣺");  
                scanf("%d",&DealMinTime);     
                if(DealMinTime<=0 || DealMinTime>=DealMaxTime){  
                    printf("   �������붼�����밴������˳���\n");  
                    getch();   
                    goto end;  
                }  
            }  
        } 
		 
        printf("   �����뽻�׶��������ޣ�");  
        scanf("%d",&MaxAmount);  
        if(MaxAmount>=BankAmount||MaxAmount>50000){  
            printf("   ������󣡳��������еķ���Χ������׶�Ӧ�������п�ʼӪҵʱ���ʽ��ܶ���С��50000�����ٴ����룡\n");  
            printf("   �����뽻�׶��������ޣ�");  
            scanf("%d",&MaxAmount);  
            if(MaxAmount>=BankAmount||MaxAmount>50000){  
                printf("   ������󣡳��������еķ���Χ������׶�Ӧ�������п�ʼӪҵʱ���ʽ��ܶ���С��50000�������һ�����룡\n");  
                printf("   �����뽻�׶��������ޣ�");  
                scanf("%d",&MaxAmount);  
                if(MaxAmount>=BankAmount||MaxAmount>50000){  
                    printf("   �������붼�����밴������˳���\n");  
                    getch();  
                    goto end;  
                }  
            } 
        }  
        
        MaxTime+=rand()%(ClientArriveMaxTime-ClientArriveMinTime+1)+ClientArriveMinTime; 
		//������ɽ�����󵽴�ʱ��������С����ʱ����֮����״ε���ʱ��  
          
        while(CurrentTime<CloseTime){								//��ǰʱ��С��Ӫҵʱ��  
            CurrentTime++;  
            if(DealTime<CurrentTime) 
				DealTime=CurrentTime ;  
            if(DealTime==CurrentTime) 								//�д����ڴ�����
				state=1;  
            if(CurrentTime==MaxTime){ 								//�����¼�  
                ClientArrive();  
                //������ɽ�����󵽴�ʱ��������С����ʱ����֮��ĵ���ʱ��
                MaxTime+=rand()%(ClientArriveMaxTime-ClientArriveMinTime+1)+ClientArriveMinTime;  
            }  
            if(state==1&&Q1.front!=NULL){  
                if(Q1.front->Amount>= 0){  
                    InAmount();									//���ô���  
                    DealQ2();									//��������������  
                    NeedIn++;  
                }  
                else{   
                    InAmount();									//����ȡ���  
                    NeedOut++;  
                }  
            }  
        }  
        
        printf("     �ͻ�����\t      �¼�����         ʱ��          ������\n");  
        while(Event.front!=NULL){ 								//����¼�����  
            if(Event.front->Type=="�뿪"){  
                printf("\t%d\t\t�뿪\t\t%d\t\t%d\n",Event.front->num, Event.front->EndTime,Event.front->Amount);  
                if(Event.front->Amount>=0)     					//�ɹ��������
					t1++;			  
                else                     						//�ɹ�ȡ������
					t3++;				  
            }  
            else{  
                printf("\t%d\t\t����\t\t%d\t\t%d\n",Event.front->num, Event.front->BeginTime,Event.front->Amount);  
                if(Event.front->Amount>=0)     					//��Ҫ�������
					t2++;			  
                else                     						//��Ҫȡ������ 
					t4++;				 
            }  
            SuccessIn=NeedIn-(t2-t1);  
            SuccessOut=NeedOut-(t4-t3);  
            DeQueue(Event);  
        }  
        while(Q1.front!=NULL){									//���½���ʱ��һ������δ����Ŀͻ�  
            BankAmountTime+=(CloseTime-Q1.front->BeginTime);   
            counter++;  
            DeQueue(Q1);  
        }  
        printf("\n");   
        printf("	��Ҫ���Ŀͻ�������%d\n",NeedIn);  
        printf("	��Ҫȡ��Ŀͻ�������%d\n",NeedOut);  
        printf("	�ɹ�������Ŀͻ�������%d\n",SuccessIn);  
        printf("	�ɹ�����ȡ��Ŀͻ�������%d\n",SuccessOut);  
        printf("	���ɹ������ʣ�%f\n",float(SuccessIn*100)/NeedIn);  
        printf("	ȡ��ɹ������ʣ�%f\n",float(SuccessOut*100)/NeedOut);  
        printf("	�ͻ�����ϵͳƽ��ʱΪ:%f\n",float(BankAmountTime)/counter);  
        printf("	���е�ǰ��%d\n",BankAmount);  
        printf("	�밴������˳���\n");  
        break;  
	}  
	if(n==0)   
		printf("�밴������˳���\n");  
	end:getch();  
    return 0;  
}  
