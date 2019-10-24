#宣告 紀錄表 , 計畫表 , 點(0->11))-->finish 
record_path = [[9,11,12,13,14,-1,-1,-1,-1,10,1,2,3,4,-1]]
schedule_path = [5,4,8,7,2,6,3,1,14,-1,-1,-1,-1,-1 , -1]
length = len(schedule_path)
update_time = 0

#合併路線
def merge(updated,updatetime):
    ref = record_path[update_time][:]
    new_path = []
    new_schedule = []
    point = 0
    while(point != -1):
        if not (point in updated):
            new_path.append(point)
        point = ref[point]
    for i in new_path[:-1]:
        point = schedule_path[i]
        while(point in updated):
            point = schedule_path[point]
        #i and point's distance
        for j , p in enumerate(new_path):
            if i == p:
                a = j
            if point == p:
                b = j
        distance = b-a    
        new_schedule.append((i , point , distance))
    new_schedule.append((length-1 , -1 , 0))
    return new_path , new_schedule
#black_point->最後需要更新的點
def black_point():
    black_point = []
    for i in range(length):
        if schedule_path[i]== -1 and i!=length-1:
            black_point.append(i)
    return black_point
if __name__ == '__main__':
    #最後更新的點
    updated = black_point()
    #更新原本沒有接收封包的點
    new = record_path[update_time][:]
    for i in range(length):
        if record_path[0][i] == -1 and i!=length-1:
            new[i]=schedule_path[i]
            updated.append(i)     
    record_path.append(new)
    update_time += 1
    #重複合併並更新的動作
    while(1):
        new_path , new_schedule = merge(updated , 0)
        if len(new_path)==1:
            break
        update_index_list = []
        update_index = -1
        start = 0
        while(1):
            max = -1000
            for i , (p1 , p2 , dis) in enumerate(new_schedule[start:]):
                i = start+i
                if dis>max:
                    update_index = i
                    max = dis
            if max!=0:
                update_index_list.append(update_index)
                start += max
            else:
                break
        #if i$2==0 even round
        new = record_path[update_time][:]
        for index in update_index_list:
            point , _ , distance = new_schedule[index]
            new[point] = schedule_path[point]
            updated.append(point)
        record_path.append(new)
        update_time += 1 

        #Select  other path that can be updated after selecting above path->unfinished.
            # error
            # error
            # error
        #else if i$2!+0 odd round

        new = record_path[update_time][:]
        for index in update_index_list:
            max_distance = 0 
            point , _ , distance = new_schedule[index]
            if distance > max_distance:
                max_distance = distance
            for i in range(1 , distance):
                point , _ , _ = new_schedule[index+i]
                new[point] = schedule_path[point]
                updated.append(point)
            if max_distance > 1:    
                record_path.append(new)
                update_time += 1
        if len(update_index_list)==0:
            break   
    new = record_path[update_time][:]
    last_updated = black_point()
    for point in last_updated:
        new[point] = schedule_path[point]
    record_path.append(new)
    update_time+=1
    print(record_path)
