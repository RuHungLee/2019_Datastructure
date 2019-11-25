#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define maxnodes 100
#define maxPairs 1000
#define TRUE 1
#define FALSE 0 
int numberNodes;
int numberLinks;
int numberTest;
int numPairs;
int ***allPath = NULL;
int graphCap[maxnodes][maxnodes];
float graphWeight[maxnodes][maxnodes];
float loadingList[maxnodes][maxnodes];
int allPairs[maxPairs][maxnodes];
int testRoute[maxnodes][3];
int seen[maxnodes];
int queue[maxnodes];
//利用src temp dest合併segment routing並回傳長度
int merge(int path[],int src,int temp,int dest){
    int i , j , cnt=0;
    for(i=1;i<allPath[src][temp][0];i++){path[cnt++]=allPath[src][temp][i];}
    for(i=1;i<=allPath[temp][dest][0];i++){path[cnt++]=allPath[temp][dest][i];}
    return cnt;
}
//印出所有的最短路徑
void readPath(){
    int i , j , k , cnt;
    for(i=0;i<numberNodes;i++){
        printf("=========[%c]==========\n" , i+65);
        for(j=0;j<numberNodes;j++){
            cnt = allPath[i][j][0];
            printf("%d ",cnt);
            for(k=1;k<=cnt;k++){
                printf("%c ",allPath[i][j][k]+65);
            }
            printf("\n");
        }
    }
}
//路徑優先交換
void swap(float weight[],int hops[],int path[][maxnodes],int i,int j){
    float temp;
    int m;
    int temPath[maxnodes];
    for(m=0;m<hops[i];m++){temPath[m]= path[i][m];}
    for(m=0;m<hops[j];m++){path[i][m]=path[j][m];}
    for(m=0;m<hops[i];m++){path[j][m]=temPath[m];}
    temp = weight[i]; weight[i] = weight[j]; weight[j] = temp;
    temp = hops[i]; hops[i] = hops[j]; hops[j] = temp;
}
// 如果兩條路徑weight和hops一樣，比較兩條路徑的ID。
int compare(int path1[] , int path2[] , int len1 , int len2){
    int i , j;
    int changed = FALSE;
    if(len1!=len2)exit(-1);
    for(i=0;i<len1;i++){
        if(path2[i]<path1[i]){
            changed = TRUE;
            break;
        }else if(path2[i]>path1[i]){break;}
    }
    return changed;
} 
//路徑嘗試優先順序     
void bubblesort(float weight[],int hops[],int path[][maxnodes]){
    int i , j , k , changed = FALSE;
    for(i=0;i<numberNodes-1;i++){
        for(j=0;j<numberNodes-1-i;j++){
            if(weight[j]>weight[j+1]){
                swap(weight,hops,path,j,j+1);
            }else if(weight[j]==weight[j+1]){
                if(hops[j]>hops[j+1]){
                    swap(weight,hops,path,j,j+1);
                }else if(hops[j]==hops[j+1]){
                    changed = compare(path[j],path[j+1],hops[j],hops[j+1]);
                    if(changed){
                        swap(weight,hops,path,j,j+1);
                    }
                }
            }
        }
    }
}
//利用BFS產生樹
void BFS(int parents[],int root){
    int i , j , p , label , seenCnt;
    int parentCnt = 1;  
    int rear=0;
    int front=0;
    for(i=0;i<numberNodes;i++){seen[i]=0;}
    queue[rear++] = root;
    parents[root] = -1;
    seen[root]=1;
    while(rear != front){
        p = queue[front++];
        for(i=0;i<numberNodes;i++){
            label = graphCap[p][i];
            if(label!=0 && seen[i]==0){parents[i]=p; queue[rear++]=i; seen[i]=1;}
        }
    }
}
//計算該條路徑得總權重值
float getWeight(int path[] , int len){
    int i , j , point , nextpoint;
    float weight = 0.0;
    point = path[0];
    for(i=1;i<len;i++){
        nextpoint = path[i];
        weight+=graphWeight[point][nextpoint];
        point = nextpoint;
    }
    return weight;
}
//選出來的路徑更新權重
int updateWeight(int path[] ,int hops , float loading){
    int i , j , point , nextpoint , len;
    float totalLoad;
    point = path[0];
    for(i=1;i<hops;i++){
        nextpoint = path[i];
        if(graphCap[point][nextpoint]<loading+loadingList[point][nextpoint]){return FALSE;}
        point = nextpoint;
    }
    point = path[0];
    for(i=1;i<hops;i++){
        nextpoint = path[i];
        loadingList[point][nextpoint] = loadingList[point][nextpoint]+loading;
        loadingList[nextpoint][point] = loadingList[nextpoint][point]+loading;
        totalLoad = loadingList[point][nextpoint];
        if(graphCap[point][nextpoint]==totalLoad){
            graphWeight[point][nextpoint] = 100000;
            graphWeight[nextpoint][point] = 100000;
        }else{
            graphWeight[point][nextpoint] = totalLoad/(graphCap[point][nextpoint]-totalLoad);
            graphWeight[nextpoint][point] = totalLoad/(graphCap[nextpoint][point]-totalLoad);
        }
        point = nextpoint;
    }
    return TRUE;
}
//配置記憶體存取所有的最短路徑
void mallocPath(int numberNodes){
    int i , j , k;
    allPath = (int ***)malloc(numberNodes*sizeof(int **));
    for(i=0;i<numberNodes;i++){
        allPath[i] = (int **)malloc(numberNodes*sizeof(int *));
        for(j=0;j<numberNodes;j++){
            allPath[i][j] = (int *)calloc(numberNodes,sizeof(int));
        }
    }
}

//利用樹產生最短路徑
void getPath(int parents[],int path[],int n, int m){
    int i ,j=1;
    int cnt = 1;
    path[cnt++] = n;
    int p = parents[n];
    while(p!=-1){path[cnt++]=p; p=parents[p];}
    for(i=cnt-1;i>0;i--){allPath[m][n][j++]=path[i];}
    allPath[m][n][0] = cnt-1;
};

//主程式
int main(){
    //標準輸入
    scanf("%d %d", &numberNodes , &numberLinks);
    int i,j,k;
    int cnt,start,dst,loading;
    for(i=0;i<numberLinks;i++){
        scanf("%d %d %d %d",&cnt,&start,&dst,&loading);
        graphCap[start][dst] = loading; graphCap[dst][start] = loading;
    }
    scanf("%d", &numberTest);
    for(i=0;i<numberTest;i++){
        scanf("%d %d %d %d",&cnt,&start,&dst,&loading);
        testRoute[i][0] = start; testRoute[i][1] = dst; testRoute[i][2] = loading;
    }
    //產生並存取最短路徑    
    int parents[maxnodes];
    int paths[maxnodes];
    mallocPath(numberNodes);
    for(i=0;i<numberNodes;i++){
        BFS(parents,i);
        for(j=0;j<numberNodes;j++){
            getPath(parents,paths,j,i);
        }
    }
    // readPath();
    //利用weight，hops，ID找出正確路徑，並更新權重。
    int src,dest,acTemp,accepted,time=0;
    int ansPath[maxnodes][maxnodes];
    int acceptedCnt = 0;
    int totalLoading = 0;
    int path[maxnodes][maxnodes];
    int hops[maxnodes];
    float weight[maxnodes];
    //測資依序插入
    for(time=0;time<numberTest;time++){
        src = testRoute[time][0];
        dest = testRoute[time][1];
        loading = testRoute[time][2];
        accepted = FALSE;
        //遍歷所有可能的最短路徑組合(two segment routing)，利用路徑權重和hops數選出正確路徑
        for(j=0;j<numberNodes;j++){
            hops[j] = merge(path[j],src,j,dest);
            weight[j] = getWeight(path[j],hops[j]); 
        }
        //test==================================
        // printf("[%d->%d loading:%d] testing\n",src,dest,loading);
        // printf("Before Sorting\n");
        // //before sorting
        // for(i=0;i<numberNodes;i++){
        //     printf("hops:%d weight:%f " ,hops[i],weight[i]);
        //     for(j=0;j<hops[i];j++){
        //         printf("[%d]",path[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("After Sorting\n");
        // //after sorting
        bubblesort(weight,hops,path);
        // for(i=0;i<numberNodes;i++){
        //     printf("hops:%d weight:%f ",hops[i],weight[i]);
        //     for(j=0;j<hops[i];j++){
        //         printf("[%d]",path[i][j]);
        //     }
        //     printf("\n");
        // }
        //test==================================
        for(i=0;i<numberNodes;i++){
            accepted = updateWeight(path[i],hops[i],loading);
            if(accepted){
                ansPath[acceptedCnt][0] = hops[i];
                for(j=1;j<=hops[i];j++){ansPath[acceptedCnt][j] = path[i][j-1];}
                acceptedCnt++;
                totalLoading += loading;
                break;
            }
        }   
    }
    //打印答案。
    int index,temp;
    printf("%d %d\n",acceptedCnt,totalLoading);
    for(time=0;time<acceptedCnt;time++){
        printf("%d ",time);
        for(j=1;j<=ansPath[time][0];j++){
            if(j==ansPath[time][0]){
                printf("%d\n",ansPath[time][j]);
            }else{
                printf("%d ",ansPath[time][j]);
            }
        }
    }
    return 0;
}