#include<stdlib.h>
#include<stdio.h>
#define maxlength 1024
#define True 1
#define False 0 
int number_of_point = 12;
int path[maxlength];
int schedule[maxlength];
int record_path[maxlength][100];
int schedule_path[12];

int black_point[maxlength];
int number_of_black_point = 0;
int updated_point[maxlength];
int number_of_updated_point = 0;
int update_index_list[maxlength];
int update_time = 0;
int temp[maxlength];
int linearsearch(int src[] ,int len , int key){
    int i;
    for(i=0;i<len;i++){if(src[i]==key)return True;}
    return False;
}
void findupdated(){
    int i , j , k;
    for(i=0;i<number_of_point-1;i++){
        if(path[i]==schedule[i]){
            updated_point[number_of_updated_point++] = i;
        }
    }
}

int merge(int updated[] , int merge_schedule[][3]){
    //合併新的路徑
    int i , j , k;
    int point = 0;
    int merge_length = 0;
    while(point!=-1){
        if(!linearsearch(updated,number_of_updated_point,point)){
            merge_schedule[merge_length++][0] = point;
        }
        point = record_path[update_time][point];
    }
    //重新規劃新的更新表
    for(i=0;i<merge_length;i++){
        int before = i;
        int after = -1;
        int distance = -1;
        int next_point = -1;
        point = merge_schedule[i][0];
        next_point = schedule_path[point];

        //每一個點都一定會有要更新到的點->不用條件停止
        while(linearsearch(updated,number_of_updated_point,next_point)){
            next_point = schedule_path[next_point];
        }
        for(j=0;j<merge_length;j++){
            if(merge_schedule[j][0] == next_point){after = j;break;}
        }
        distance = after-before;
        merge_schedule[i][1] = next_point;
        merge_schedule[i][2] = distance;
        merge_schedule[merge_length-1][1] = -1;
        merge_schedule[merge_length-1][2] = 0;
    }
    return merge_length;
}

int main(void){

    int i ,j , k;
    int cnt = 0;
    FILE *fin = fopen("input.txt" , "r");
    FILE *fout = fopen("output.txt" , "w");
    fscanf(fin , "%d" , &number_of_point);
    for(i=0;i<number_of_point;i++){fscanf(fin , "%d" , &path[i]);}
    for(i=0;i<number_of_point;i++){fscanf(fin , "%d" , &schedule[i]);}
    for(i=0;i<number_of_point;i++){
        record_path[update_time][i] = path[i];
        schedule_path[i] = schedule[i];
    }
    findupdated();
    //尋找黑點->加入更新點-->finish
    for(i=0;i<number_of_point;i++){
        if(schedule_path[i]==-1 && i!=number_of_point-1){
            black_point[number_of_black_point++] = i;
            updated_point[number_of_updated_point++] = i;
        }
    }
    //更新原本沒有收到封包的點-->finish
    for(i=0;i<number_of_point;i++){record_path[update_time+1][i] = record_path[update_time][i];}
    update_time++;
    for(i=0;i<number_of_point;i++){
        if(record_path[update_time][i]==-1 && i!=number_of_point-1){
            record_path[update_time][i] = schedule[i];
            updated_point[number_of_updated_point++] = i;
        }
    }
    //重複合併路徑和更新路徑的動作
    while(1){
        int merge_schedule[maxlength][3];
        int merge_length = 0;
        merge_length = merge(updated_point , merge_schedule);
        
        //退出條件
        if(merge_length==1)break;
        //odd round->挑點錯誤
        int update_index = 0;
        int start = 0;
        int cnt = 0;
        while(1){
            int max = -1000;
            for(i=start;i<merge_length;i++){
                int dis = merge_schedule[i][2];
                if (dis>max){
                    update_index = i;
                    max=dis;   
                }
            }
            if(max!=0 && max!=-1000){
                update_index_list[cnt++] = update_index;
                start = update_index+max;
            }else{break;}
        }

        for(i=0;i<merge_length-1;i++){
            int c = True;
            int left = i;
            int right = i+merge_schedule[i][2];
            if(merge_schedule[i][2]<1)continue;
            if(!linearsearch(update_index_list,cnt,i)){
                for(j=0;j<cnt;j++){
                    int check_left = update_index_list[j];
                    int check_right = check_left+merge_schedule[check_left][2];
                    if((!(right<=check_left)&&!(left>=check_right))){c=False;}
                }
                if(c==True){update_index_list[cnt++] = i;}
            }
        }

        if(cnt>0){    
            for(i=0;i<number_of_point;i++){record_path[update_time+1][i] = record_path[update_time][i];}
            update_time++;
            for(i=0;i<cnt;i++){
                int index = update_index_list[i];
                int point = merge_schedule[index][0];
                record_path[update_time][point] = schedule_path[point];
                updated_point[number_of_updated_point++] = point;
            }
        }
        //even round
        int templen = 0;
        int check = 0;//可更新
        for(i=0;i<cnt;i++){
            int index = update_index_list[i];
            int dis = merge_schedule[index][2];
            if (dis>1){check=1;}
            for(j=1;j<dis;j++){
                temp[templen++] = merge_schedule[index+j][0];
                updated_point[number_of_updated_point++] = merge_schedule[index+j][0];
            }
        }
        if(check){
            for(i=0;i<number_of_point;i++){record_path[update_time+1][i] = record_path[update_time][i];}
            update_time++;    
            for(i=0;i<templen;i++){
                int update_point = temp[i];
                record_path[update_time][update_point] = schedule_path[update_point];
            }
        }
    }
    //不一定都有黑點-->還沒更新
    if (number_of_black_point>0){
        for(i=0;i<number_of_point;i++){record_path[update_time+1][i] = record_path[update_time][i];}
        update_time++;
        //更新黑點
        for(i=0;i<number_of_black_point;i++){
            int update_point = black_point[i];
            record_path[update_time][update_point] = -1;
        }
    }
    //打印結果
    fprintf(fout , "%d\n" , update_time+1);
    for(i=0;i<update_time+1;i++){
        for(j=0;j<number_of_point;j++){
            fprintf(fout , "%d " , record_path[i][j]);
        }
        fprintf(fout , "\n");
    }
    return 0;
}
