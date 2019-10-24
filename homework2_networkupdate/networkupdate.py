#宣告 紀錄表 , 計畫表 , 點(0->11))-->finish 
current_path = [[7,2,9,10,-1,-1,-1,8,1,3,11,-1]]
schedule_path = [4,6,1,2,5,3,11,-1,-1,-1,-1,-1]
length = len(schedule_path)
update_time = 0

#combine
def merge(updated_point , last_update_point):
    ref_path = current_path[update_time][:]
    print(f'ref_path {ref_path}')
    print(f'update_point {updated_point}')
    print(f'last_update_point {last_update_point}')
    path = []
    path.append(0)
    next_point = ref_path[0]
    #實線
    while(next_point!=-1):
        if (not next_point in updated_point[:-1]) and (not next_point in last_update_point):
            path.append(next_point)
        next_point = ref_path[next_point]
    return path

#(point1->point2->distance)  
def update_distance(path, updated_point):
    p_p_d = []
    for i in path[:-1]:
        dis = 1
        last_point = i
        next_point = schedule_path[i]
        print(last_point , next_point)
        while(next_point in updated_point):
            dis = dis+1
            next_point = schedule_path[next_point]
        p_p_d.append((last_point , next_point , dis))
    return p_p_d

#查看哪些點不需要再被更新了-->finish
def updated():
    update_list = []
    for i in range(length):
        if current_path[update_time][i] == schedule_path[i]:
            update_list.append(i)
    return update_list 

#找出最後一輪做更新的點(這裡稱black_point)-->finish
def black_point():
    black_point = []
    for i in range(length):
        if schedule_path[i]== -1 and i!=length-1:
            black_point.append(i)
    return black_point

if __name__ == '__main__':
    #最後更新的點-->finish
    last_update_point = black_point()

    #更新原本沒有做任何連線的點-->finish
    new = []
    for i in range(length):
        if current_path[0][i] == -1:
            new.append(schedule_path[i])
        else:
            new.append(current_path[0][i])        
    current_path.append(new)
    update_time += 1
    
    #合併後產生新的需要更新的陣列(更新過和最後需要更新的點不管)
    updated_point = updated()
    virtual_path = merge(updated_point = updated_point, last_update_point = last_update_point)
    point_distance = update_distance(path = virtual_path , updated_point = updated_point)    
    print(f'virtual_path {virtual_path}')
    print(f'point_distance {point_distance}')
    #奇數更新(shortcut phase)和偶數更新(prune phase)-->unfinish
    
    '''
    #最後一輪更新-->finish
    new = current_path[update_time][:]
    print(new)
    for i in last_update_point:
        new[i] = schedule_path[i]
    current_path.append(new)
    #打印最後的schedules-->finish
    print(current_path)
    '''