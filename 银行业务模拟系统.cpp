#include<iostream>  
#include<stdio.h>    
#include<conio.h>  
#include<time.h>   
#include<windows.h>  
#include<stdlib.h>
#include"Bank.h"  
using namespace std;  

//===========队列的基本操作========== 
void InitQueue(Queue &Q){  
//初始化操作，建立一个空队列
    Q.front=Q.rear=(CustNode*)malloc(sizeof(CustNode));  
    if(!(Q.front))  
         exit(1);  
    Q.front->next=0;  
 }  
 
void EnQueue(Queue &Q,int e){  
//入队列
//插入元素e为队列Q的新的队尾元素
    CustNode* p=new CustNode;  
    p->Amount=e;  
    p->next=NULL; 
    if(Q.front==NULL){		
    //队列为空，初始化    
		Q.front=p;  
        Q.rear=p;  
    }  
    else{ 					  
    //队列不为空，插入元素e    
		Q.rear->next=p;  
        Q.rear=Q.rear->next;  
    }  
}  
  
  
void DeQueue(Queue &Q){
//出队列  
    CustNode *p;  
    p=Q.front;  
    if(Q.front->next==NULL)		//队列只有一个元素  
        Q.front=Q.rear=NULL;  
    else  						//调整队列头指针 
        Q.front=Q.front->next;  
    delete p;  
}  
  

CustNode *Queuefront(Queue &Q){ 
//返回队首元素 
    return Q.front;  
}  
  
  
CustNode *Queuerear(Queue &Q){  
//返回队尾元素
    return Q.rear;  
}  

//==========事件类型的基本操作========= 
void ClientArrive(){
//客户到达事件处理
//随机产生顾客，进入队列一产生到达事件 进入事件队列  
    EnQueue(Q1,(rand()%(2*MaxAmount)-MaxAmount));  			//随机产生顾客加入第一队列  
    Queuerear(Q1)->BeginTime=CurrentTime;					//当前时间为客户的到达时间  
    Queuerear(Q1)->num=number;								//客户号为客户序列号  
    EnQueue(Event,(Queuerear(Q1)->Amount)); 				//将产生事件加入事件队列  
    Queuerear(Event)->BeginTime=CurrentTime;  
    Queuerear(Event)->Type="到达";  
    Queuerear(Event)->num=number;  
    number++;  
}  
  
  
void InAmount(){
//存款事件处理 
    BankAmount+=Queuefront(Q1)->Amount; 									//更新资金总额  
    EnQueue(Event,Queuefront(Q1)->Amount); 									//加入事件队列   
    Queuerear(Event)->Type="离开";  
    Queuerear(Event)->num=Queuefront(Q1)->num;  
    //离开时间为到达时间加上随机产生的介于最大处理时间和最小处理时间的处理时间
    Queuerear(Event)->EndTime=(Queuefront(Q1)->BeginTime+rand()%(DealMaxTime-DealMinTime +1)+DealMinTime);  
    counter++; 																//更新客户总数  
    BankAmountTime+=(Queuerear(Event)->EndTime-Queuefront(Q1)->BeginTime); 	//更新逗留时间  
    DeQueue(Q1); 															//删除第一队列第一个业务  
    DealTime=Queuerear(Event)->EndTime;										//交易时间为客户的离开时间  
    state=0;																//窗口没有交易需要处理  
}  
  
  
void OutAmount(){  
//取款事件处理 
    if((-Q1.front->Amount)>BankAmount){		   
    //资金短缺 加入第二队列  
        EnQueue(Q2,Queuefront(Q1)->Amount);  
        Queuerear(Q2)->BeginTime=Queuefront(Q1)->BeginTime;  
        Queuerear(Q2)->num=Queuefront(Q1)->num;  
        DeQueue(Q1);  
    }  
    else{  
        BankAmount+=Queuerear(Q1)->Amount;				//更新资金总额  
        EnQueue(Event,Queuefront(Q1)->Amount); 			//加入事件队列   
        Queuerear(Event)->Type="离开";  
        Queuerear(Event)->num=Queuefront(Q1)->num;  
        //客户的离开时间为客户的到达时间加上随机产生的介于最大处理时间和最小处理时间的处理时间
        Queuerear(Event)->EndTime=(Queuefront(Q1)->BeginTime +rand()%(DealMaxTime-DealMinTime +1)+DealMinTime);  
        Queuerear(Event)->BeginTime=0;  
        DealTime=Queuerear(Event)->EndTime;										//交易时间为客户的离开时间  
        counter++; 																//更新客户总数   
        BankAmountTime+=(Queuerear(Event)->EndTime-Queuerear(Q1)->BeginTime); 	//更新逗留时间  
        DeQueue(Q1); 															//删除第一队列第一个业务  
        state=0;																//窗口没有交易需要处理  
    }  
}  
  
  
CustNode *SearchQ2(Queue &Q,int m){  
//在队列Q2中进行搜索可以处理的元素 
    CustNode *sign=Q.front; 				//标记头节点  
    CustNode *temp;  
    while(Q.front!=NULL){  
        if((-(Q.front->Amount))<m){			//队首元素可以处理  
            if(Q.front==Q.rear){  
                temp=Q.front;  
                Q.front=Q.rear=NULL;  
                return temp;  
            }  
        	else{							//队首元素出列  
            	temp=Q.front;   
            	Q.front=Q.front->next; 		//首节点后移一位,返回原首节点  
            	return temp;  
        	}  
        }  
        else{								//队首元首不能被处理  
            if(Q.front==Q.rear){
			}  
            else{							//首节点移到队列尾部  
              
            	Q.rear->next=Q.front;   
                Q.rear=Q.rear->next;  
                Q.front=Q.front->next;  
                Q.rear->next=NULL;  
            }  
        }  
        if(Q.front==sign)					//队列循环一周时停止  
            return NULL;  
    }  
    return NULL;  
}  
  
  
  
void DealQ2(){ 
//在队列Q2中对于可以处理的元素进行处理 
   	CustNode* temped;  
	int randomTemp;
    while((temped=SearchQ2(Q2,BankAmount))&&temped!=NULL){ 					//查找可处理取款  
      
        BankAmount+=temped->Amount; 										//更新资金总额  
        EnQueue(Event,temped->Amount); 										//加入事件队列   
        Queuerear(Event)->Type="离开";  
        Queuerear(Event)->num=temped->num;  
        randomTemp=rand()%(DealMaxTime-DealMinTime +1)+DealMinTime;			//处理时间为随机产生的介于最大处理时间和最小处理时间之间的处理时间  
        Queuerear(Event)->EndTime=CurrentTime+randomTemp ;					//客户离开时间为当前时间加上处理时间  
        DealTime+=randomTemp;												//更新交易时间  
        counter++; 															//更新客户总数  
        BankAmountTime+=(Queuerear(Event)->EndTime-temped->BeginTime); 		//更新逗留时间   
        delete temped; 														//删除节点  
        temped = NULL;  
    }  
    state = 0;  
}  
  
 
void BankOutLook(){  
//银行模拟系统初始界面     
    printf("   =========================================================================		\n");
	printf("   =========================================================================		\n");
    printf("                       Simulation of The Bank business								\n");
    printf("   -------------------------------------------------------------------------	  \n\n");
	printf("                       ---------------------------------                            \n");
	printf("                    	     Number: 3116004979                                     \n");
	printf("                    	     CLASS ：16网络二班                                 	\n");
    printf("                    	     NAME  ：  詹泽霖                                       \n");
    printf("                        ---------------------------------                           \n");
    printf("   =========================================================================        \n");
    printf("   *************************************************************************		\n");  
    printf("   *************************************************************************		\n");  
    printf("   *******************                               ***********************		\n");
	printf("   *******************    0.退出   1.进入模拟系统    ***********************		\n");   
    printf("   *******************                               ***********************		\n"); 
    printf("   *************************************************************************		\n");  
    printf("   *************************************************************************		\n");  
    printf("   ************************      请选择服务      ***************************		\n");
    printf("   =========================================================================        \n");
    printf("   请输入选择的操作对应编号："); 
}    
   
int main(){ 
	system("color 70");
    BankOutLook();  
    int n,t1=0,t2=0,t3=0,t4=0,m=0;
    scanf("%d",&n);  
    while(n==1){  
        srand(time(NULL)); 							//初始化随机函数  
        printf("   请输入银行的初始存款：");  
        scanf("%d",&BankAmount);   
        if(BankAmount<0){                          
            printf("   输入错误！初始存款不能小于0！请再次输入！\n");   
            printf("   请输入银行的初始存款：");  
            scanf("%d",&BankAmount);   
            if(BankAmount<0){       
                printf("   输入错误！初始存款不能小于0！请最后一次输入！\n");  
                printf("   请输入银行的初始存款：");  
                scanf("%d",&BankAmount);   
                if(BankAmount<0){ 
                    printf("   三次输入都错误！请按任意键退出！\n");  
                    getch();   
                    goto end;   
                }  
            }  
        }  
        
        printf("   请输入银行的营业时间：");  
        scanf("%d",&CloseTime);  
        if(CloseTime>=1440){                 
            printf("   输入错误！一天的营业时间不能超过1440分钟（24个小时）！请再次输入！\n");  
            printf("   请输入银行的营业时间：");  
            scanf("%d",&CloseTime);  
            if(CloseTime>=1440){   
                printf("   输入错误！一天的营业时间不能超过1440分钟（24个小时）！请最后一次输入！\n");  
                printf("   请输入银行的营业时间：");  
                scanf("%d",&CloseTime);  
                if(CloseTime>=1440){   
                    printf("   三次输入都错误！请按任意键退出！\n");  
                    getch();    
                    goto end;  
                }  
            }  
        } 
        
        printf("   请输入最大到达时间间隔：");   
        scanf("%d",&ClientArriveMaxTime);  
        if(ClientArriveMaxTime>CloseTime){         
            printf("   输入错误！最大到达时间间隔必须小于营业时间！请再次输入！\n");  
            printf("   请输入最大到达时间间隔：");   
            scanf("%d",&ClientArriveMaxTime);  
            if(ClientArriveMaxTime>CloseTime){   
                printf("   输入错误！最大到达时间间隔必须小于营业时间！请最后一次输入！\n");  
                printf("   请输入最大到达时间间隔：");   
                scanf("%d",&ClientArriveMaxTime);  
                if(ClientArriveMaxTime>CloseTime){  
                    printf("   三次输入都错误！请按任意键退出！\n");  
                    getch();   
                    goto end;  
                }   
            }  
        }  
        
        printf("   请输入最小到达时间间隔：");   
        scanf("%d",&ClientArriveMinTime);  
        if(ClientArriveMinTime<=0||ClientArriveMinTime>=ClientArriveMaxTime){       
            printf("   输入错误！最小到达时间间隔必须介于零和最大到达时间之间！请再次输入！\n");   
            printf("   请输入最小到达时间间隔：");   
            scanf("%d",&ClientArriveMinTime);  
            if(ClientArriveMinTime<=0||ClientArriveMinTime>=ClientArriveMaxTime){  
                printf("   输入错误！最小到达时间间隔必须介于零和最大到达时间之间！请最后一次输入！\n");   
                printf("   请输入最小到达时间间隔：");   
                scanf("%d",&ClientArriveMinTime);  
                if(ClientArriveMinTime<=0||ClientArriveMinTime>=ClientArriveMaxTime){ 
                    printf("   三次输入都错误！请按任意键退出！\n");  
                    getch();  
                    printf("   请按任意键退出!\n");  
                    goto end;  
                }   
            }  
        }   
        
        printf("   请输入最大的处理时间：");  
        scanf("%d",&DealMaxTime);  
        if(DealMaxTime>CloseTime){           
            printf("   输入错误！最大处理时间必须小于营业时间！请再次输入！\n");   
            printf("   请输入最大的处理时间：");  
            scanf("%d",&DealMaxTime);  
            if(DealMaxTime>CloseTime){  
               	printf("   输入错误！最大处理时间必须小于营业时间！请最后一次输入！\n");   
                printf("   请输入最大的处理时间：");  
                scanf("%d",&DealMaxTime);  
                if(DealMaxTime>CloseTime){  
                    printf("   三次输入都错误！请按任意键退出！\n");  
                    getch();     
                    goto end;  
                }  
            }  
        }  
        
        printf("   请输入最小的处理时间：");  
        scanf("%d",&DealMinTime);     
        if(DealMinTime<=0||DealMinTime>=DealMaxTime){
            printf("   输入错误！最小处理时间必须介于零和最大处理时间之间！请再次输入！\n");  
            printf("   请输入最小的处理时间：");  
            scanf("%d",&DealMinTime);     
            if(DealMinTime<=0||DealMinTime>=DealMaxTime){      
                printf("   输入错误！最小处理时间必须介于零和最大处理时间之间！请最后一次输入！\n");  
                printf("   请输入最小的处理时间：");  
                scanf("%d",&DealMinTime);     
                if(DealMinTime<=0 || DealMinTime>=DealMaxTime){  
                    printf("   三次输入都错误！请按任意键退出！\n");  
                    getch();   
                    goto end;  
                }  
            }  
        } 
		 
        printf("   请输入交易额的最大上限：");  
        scanf("%d",&MaxAmount);  
        if(MaxAmount>=BankAmount||MaxAmount>50000){  
            printf("   输入错误！超出本银行的服务范围！最大交易额应低于银行开始营业时的资金总额且小于50000！请再次输入！\n");  
            printf("   请输入交易额的最大上限：");  
            scanf("%d",&MaxAmount);  
            if(MaxAmount>=BankAmount||MaxAmount>50000){  
                printf("   输入错误！超出本银行的服务范围！最大交易额应低于银行开始营业时的资金总额且小于50000！请最后一次输入！\n");  
                printf("   请输入交易额的最大上限：");  
                scanf("%d",&MaxAmount);  
                if(MaxAmount>=BankAmount||MaxAmount>50000){  
                    printf("   三次输入都错误！请按任意键退出！\n");  
                    getch();  
                    goto end;  
                }  
            } 
        }  
        
        MaxTime+=rand()%(ClientArriveMaxTime-ClientArriveMinTime+1)+ClientArriveMinTime; 
		//随机生成介于最大到达时间间隔和最小到达时间间隔之间的首次到达时间  
          
        while(CurrentTime<CloseTime){								//当前时间小于营业时间  
            CurrentTime++;  
            if(DealTime<CurrentTime) 
				DealTime=CurrentTime ;  
            if(DealTime==CurrentTime) 								//有窗口在处理交易
				state=1;  
            if(CurrentTime==MaxTime){ 								//到达事件  
                ClientArrive();  
                //随机生成介于最大到达时间间隔和最小到达时间间隔之间的到达时间
                MaxTime+=rand()%(ClientArriveMaxTime-ClientArriveMinTime+1)+ClientArriveMinTime;  
            }  
            if(state==1&&Q1.front!=NULL){  
                if(Q1.front->Amount>= 0){  
                    InAmount();									//调用存款函数  
                    DealQ2();									//调用搜索处理函数  
                    NeedIn++;  
                }  
                else{   
                    InAmount();									//调用取款函数  
                    NeedOut++;  
                }  
            }  
        }  
        
        printf("     客户序列\t      事件类型         时间          处理金额\n");  
        while(Event.front!=NULL){ 								//清除事件队列  
            if(Event.front->Type=="离开"){  
                printf("\t%d\t\t离开\t\t%d\t\t%d\n",Event.front->num, Event.front->EndTime,Event.front->Amount);  
                if(Event.front->Amount>=0)     					//成功存款人数
					t1++;			  
                else                     						//成功取款人数
					t3++;				  
            }  
            else{  
                printf("\t%d\t\t到达\t\t%d\t\t%d\n",Event.front->num, Event.front->BeginTime,Event.front->Amount);  
                if(Event.front->Amount>=0)     					//需要存款人数
					t2++;			  
                else                     						//需要取款人数 
					t4++;				 
            }  
            SuccessIn=NeedIn-(t2-t1);  
            SuccessOut=NeedOut-(t4-t3);  
            DeQueue(Event);  
        }  
        while(Q1.front!=NULL){									//更新结束时第一队列中未处理的客户  
            BankAmountTime+=(CloseTime-Q1.front->BeginTime);   
            counter++;  
            DeQueue(Q1);  
        }  
        printf("\n");   
        printf("	需要存款的客户人数：%d\n",NeedIn);  
        printf("	需要取款的客户人数：%d\n",NeedOut);  
        printf("	成功办理存款的客户人数：%d\n",SuccessIn);  
        printf("	成功办理取款的客户人数：%d\n",SuccessOut);  
        printf("	存款成功办理率：%f\n",float(SuccessIn*100)/NeedIn);  
        printf("	取款成功办理率：%f\n",float(SuccessOut*100)/NeedOut);  
        printf("	客户进入系统平均时为:%f\n",float(BankAmountTime)/counter);  
        printf("	银行当前余额：%d\n",BankAmount);  
        printf("	请按任意键退出！\n");  
        break;  
	}  
	if(n==0)   
		printf("请按任意键退出！\n");  
	end:getch();  
    return 0;  
}  
