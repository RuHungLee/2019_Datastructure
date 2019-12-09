#include<stdlib.h>
#include<stdio.h>
#define maxnode 100
#define TRUE 1
#define FALSE 0
int numberNodes;
int numberLinks;
int numberQuery;
int sizeofHashTable;
int prime;
int parent[maxnode];
int distance[maxnode];
int seen[maxnode];
int graph[maxnode][maxnode];
int shortestDis[maxnode][maxnode];
int query[maxnode][2];
#include<stdlib.h>
#include<stdio.h>
#define TRUE 1
#define FALSE 0
//定義hashtable結構，hashtable函式
typedef struct NODE{
    int point1;
    int point2;
    int distance;
    struct NODE *next;
}NODE;
typedef struct NODE *nodePointer;
NODE **hashTable = NULL;

// typedef struct NODE **pointer;
void insertHash(int p1 , int p2 , int dis){
    int index = ((p1+1)*(p2+1))%sizeofHashTable%sizeofHashTable;
    // printf("%d %d -> %d\n",p1,p2,index);
    int first = FALSE;
    nodePointer newnode = NULL;
    newnode = (nodePointer)malloc(sizeof(NODE));
    newnode->point1 = p1;
    newnode->point2 = p2;
    newnode->distance = dis;
    newnode->next = NULL; 
    nodePointer current = hashTable[index];
    nodePointer last = NULL;
    if(current==NULL){hashTable[index] = newnode;first = TRUE;return;}
    if(!first){
        while(current!=NULL){
            if(current->point1==p1 && current->point2==p2){return;}
            last = current;
            current = current->next;
        }
    }
    last->next = newnode;
    return ;
}
int getHashSize(int n){
    int k = n*n*n;
    int x = 0;
    while(x*x<k){x++;}
    return x;
}
void showHash(){
    int i,j;
    for(i=0;i<sizeofHashTable;i++){
        nodePointer ptr = hashTable[i];
        printf("[%d]\n",i);
        while(ptr!=NULL){
            printf("point1:[%c] point2:[%c] dis:[%d]\n",ptr->point1+65,ptr->point2+65,ptr->distance);
            ptr = ptr->next;
        }
    }
}
int searchHash(int p1 , int p2){
    int i , j , dis , index;
    index = ((p1+1)*(p2+1))%sizeofHashTable%sizeofHashTable;
    nodePointer ptr = hashTable[index];
    while(ptr!=NULL){
        if(ptr->point1 == p1 && ptr->point2 == p2){
            return ptr->distance;
        }
        ptr = ptr->next;
    }
    return -1;
}
int hashCnt(){
    int i,cnt = 0;
    for(i=0;i<sizeofHashTable;i++){
        nodePointer ptr = hashTable[i];
        while(ptr!=NULL){
            cnt++;
            // printf("%c -> %c : %d\n",ptr->point1+65,ptr->point2+65,ptr->distance);
            ptr = ptr->next;
        }
    }
    return cnt;
}
//定義優先序列結構，優先序列函式
typedef struct pqueue{
    int point,dis;
    struct pqueue* next;
}pqueue;
typedef struct pqueue *queuePointer;
queuePointer qtemp = NULL;
queuePointer push(queuePointer root,int point,int dis){
    queuePointer newnode = (queuePointer)malloc(sizeof(pqueue));
    newnode->point = point;
    newnode->dis = dis;
    newnode->next = NULL;
    if(root==NULL){root = newnode; return root;}
    if(dis<root->dis){newnode->next = root; root = newnode; return root;}
    queuePointer current = root;
    queuePointer last = NULL;
    for(;current!=NULL;current = current->next){
        if(dis<current->dis){break;}
        last = current;
    }
    if(current){
        newnode->next = current; 
        last->next = newnode;
    }else{last->next = newnode;}
    return root;
}
queuePointer pop(queuePointer root){
    if(root==NULL){printf("empty!");exit(-1);}
    qtemp = root;
    return root->next;
}
int empty(queuePointer root){if(root==NULL){return TRUE;}return FALSE;}
void queueView(queuePointer root){
    queuePointer node = root;
    for(;node!=NULL;node = node->next){
        printf("point:[%d] distance[%d]\n" , node->point , node->dis);
    }
}
//利用BFS找出最短路徑
void BFS(int point){
    queuePointer root = NULL;
    int i , j , k , dis , p;
    root = push(root , point , 0);
    for(i=0;i<numberNodes;i++){seen[i]=FALSE;}
    for(i=0;i<numberNodes;i++){distance[i]=100000;}
    distance[point] = 0;
    parent[point] = -1;
    while(!empty(root)){
        root = pop(root);
        dis = qtemp->dis;
        p = qtemp->point;
        seen[p] = TRUE;
        for(i=0;i<numberNodes;i++){
            if(graph[p][i]!=0&&seen[i]==FALSE){
                if (dis+graph[p][i]<distance[i]){
                    root = push(root,i,dis+graph[p][i]);
                    parent[i] = p;
                    distance[i] = dis+graph[p][i];
                }
            }
        }
    }
}
//開根號找出sample點數量
int getnumberSample(int input){
    int x = 0;
    while(x*x<input){x++;}
    return x;
}
//利用距離找尋sample點
int getSample(int samplePoint[] , int cnt){
    int i , j , dis , ldis = -1 , s = -1;
    for(i=0;i<numberNodes;i++){
        dis  = 100000;
        for(j=0;j<cnt;j++){
            if(shortestDis[i][samplePoint[j]]<dis){
                dis = shortestDis[i][samplePoint[j]];
            }
        }
        if(dis>ldis){ldis = dis; s = i;}
    }
    return s;
}
//查詢sample點
int getS(int c,int samplePoint[],int sampleCnt){
    int i,j,s,mindis=100000;
    for(i=0;i<sampleCnt;i++){
        if(shortestDis[c][samplePoint[i]]<mindis && samplePoint[i]!=c){
            mindis=shortestDis[c][samplePoint[i]];
            s = samplePoint[i];
        }
    }
    return s;
}
int getR(int c,int samplePoint[],int sampleCnt){
    int i,j,mindis=100000,d;
    for(i=0;i<sampleCnt;i++){
        d = shortestDis[c][samplePoint[i]];
        (d<mindis)?(mindis=d):(mindis=mindis);
    }
    return mindis;
}
int main(){
    //標準輸入
    int i,j,k;
    int cnt,start,dst,dis,p,samplePoint[maxnode],sampleCnt = 0;
    scanf("%d %d %d", &numberNodes , &numberLinks , &prime);
    for(i=0;i<numberLinks;i++){
        scanf("%d %d %d %d",&cnt,&start,&dst,&dis);
        graph[start][dst] = dis; graph[dst][start] = dis;  
    }
    scanf("%d", &numberQuery);
    for(i=0;i<numberQuery;i++){
        scanf("%d %d %d",&cnt,&start,&dst);
        query[i][0] = start; query[i][1] = dst;
    }
    int numberSample = getnumberSample(numberNodes);
    //建構最短路徑表
    for(i=0;i<numberNodes;i++){
        BFS(i);
        for(j=0;j<maxnode;j++){
            shortestDis[i][j] = distance[j];
        }
    }
    //建構hashtable
    sizeofHashTable = getHashSize(numberNodes);
    hashTable = (NODE **)malloc(sizeof(NODE *)*sizeofHashTable);
    for(i=0;i<sizeofHashTable;i++){hashTable[i] = NULL;}
    /*
    STEP1. 選一個點，並找出該點到所有點的最短路徑。
    STEP2. 選出最近sample點距離最大點，並找出該點到所有點的最短路徑。
    STEP3. 選出最近sample點距離最大點當作第三個sample點，並找出該點到所有點的最短路徑。
    STEP4. 遍歷所有的點，每一點找出離他最近的sampled點，並以到該sampled點的距離為半徑，找出到圓內所有點距離。
    */
    //STEP1 ~ STEP3
    for(i=0;i<numberSample;i++){
        (i==0)?(samplePoint[i] = 0):(samplePoint[i] = getSample(samplePoint , i));
        p = samplePoint[i];
        for(j=0;j<numberNodes;j++){
            if(shortestDis[j][p]!=0){
                insertHash(j,p,shortestDis[j][p]);
                insertHash(p,j,shortestDis[j][p]);
            }
        }
    }
    int radius = 0;
    for(i=0;i<numberNodes;i++){
        radius = getR(i,samplePoint,numberSample);
        //只要點不是sample點，半徑小於radius就存進hsahtable。
        if(radius!=0){
            for(j=0;j<numberNodes;j++){
                if(shortestDis[i][j]<=radius){
                    if(shortestDis[i][j]!=0){
                        insertHash(i,j,shortestDis[i][j]);
                        insertHash(j,i,shortestDis[i][j]);
                    }
                }
            }
        }
    }
    printf("%d\n",hashCnt()/2);
    int p1 , p2 , d1 , d2 , sample;
    for(i=0;i<numberQuery;i++){
        p1 = query[i][0];
        p2 = query[i][1];
        d1 = searchHash(p1 , p2);
        if(d1==-1){
            //p1->sample->p2
            sample = getS(p1 , samplePoint , numberSample);
            d1 = searchHash(p1,sample)+searchHash(sample,p2);
            //p2->sample->p1
            sample = getS(p2 , samplePoint , numberSample);
            d2 = searchHash(p2,sample)+searchHash(sample,p1);
            (d1<d2)?(d1=d1):(d1=d2);
        }
        printf("%d %d %d %d\n",i,p1,p2,d1);
    }

    return 0;
}

