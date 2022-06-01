from pickle import FALSE, TRUE
import heapq


class process:
    name = ''
    arrive = 0
    burst = 0
    priority = 0

    def __init__(self, name, arrive, burst, priority):
        self.name = name
        self.arrive = arrive
        self.burst = burst
        self.priority = priority

    def __lt__(self, other):
        if (self.burst == other.burst):
            return self.arrive > other.arrive
        return self.burst > other.burst


class processing:
    start = 0
    end = 0
    burst = 0

    def __init__(self, start, end, burst):
        self.start = start
        self.end = end
        self.burst = burst


num = 0
TQ = 0
fin_time = 0

num = int(input())
TQ = int(input())

processing_time = [[process(0, 0, 0, 0) for i in range(num)]
                   for j in range(num)]
burst_time = [0 for i in range(num)]
complete_time = [0 for i in range(num)]
wait_time = [0 for i in range(num)]
turnaround_time = [0 for i in range(num)]
response_time = [0 for i in range(num)]
Process = []

for i in range(num):
    arrive = int(input())
    burst = int(input())
    priority = int(input())
    p = process(i, arrive, burst, priority)
    Process.append(p)
    burst_time.append(burst)


def sort_arrive(Process):
    for i in range(num-1):
        for j in range(num - 1 - i):
            if (Process[j].arrive > Process[j + 1].arrive):
                temp = Process[j + 1]
                Process[j + 1] = Process[j]
                Process[j] = temp


def FCFS(Process):
    sort_arrive(Process)

    time = 0

    avg_wait = 0.0
    avg_turnaround = 0.0
    avg_response = 0.0

    for i in range(num):
        processing_time[i] = []

    for i in range(num):
        burst_t = Process[i].burst
        arrive_t = Process[i].arrive
        tmp = processing(burst_t, time, burst_t + time)

        name = Process[i].name
        processing_time[name].append(tmp)

        while time < arrive_t:
            time += 1

        wait_time[name] = time - arrive_t
        avg_wait += time - arrive_t

        turnaround_time[name] = time + burst_t - arrive_t
        avg_turnaround += time + burst_t - arrive_t

        burst_time[name] = burst_t

        complete_time[name] = time + burst_t

        response_time[name] = time - arrive_t
        avg_response += time - arrive_t

        time += burst_t
        fin_time = time

    avg_wait /= num
    avg_turnaround /= num
    avg_response /= num
    print_(avg_wait, avg_turnaround, avg_response)


def done_find(done, done_len, num):
    for i in range(done_len):
        if (done[i] == num):
            return True
    return False


def SJF(Process):
    readyQ = []

    for i in range(num):
        processing_time[i] = []

    sort_arrive(Process)
    for i in range(num-1):
        for j in range(num-i):
            if (Process[j].arrive == Process[j + 1].arrive and Process[j].burst > Process[j + 1].burst):
                temp = Process[j + 1]
                Process[j + 1] = Process[j]
                Process[j] = temp

    time = 0
    avg_wait = 0
    avg_turnaround = 0
    avg_response = 0

    done = []
    done_num = 0

    process_num = 0
    present = Process[process_num]
    name = present.name

    while time < fin_time:
        name = present.name
        done[done_num] = name
        done_num += 1

        burst_t = present.burst
        arrive_t = present.arrive

        if arrive_t > time:
            time = arrive_t
        tmp = processing(time, burst_t + time, burst_t)
        processing_time[name].append(tmp)

        wait_time[name] = time - arrive_t
        avg_wait += (time - arrive_t)

        turnaround_time[name] = burst_t + time - arrive_t
        avg_turnaround += burst_t + time - arrive_t

        response_time[name] = time - arrive_t
        avg_response += time - arrive_t

        time += burst_t

        for i in range(num):
            readyQ2 = readyQ
            isin = False
            for j in range(readyQ):
                tmp = readyQ2[0]
                heapq.heappop(readyQ2)
                if (tmp.name == Process[i].name):
                    isin = True

            if (Process[i].arrive <= time) and (done_find(done, done_num, Process[i].name) == False) and (isin == False):
                heapq.heappush(readyQ, Process[i])

        next_process = readyQ[0].name

        heapq.heappop(readyQ)

        for i in range(num):
            if (Process[i].name == next_process):
                process_num = i

        present = Process[process_num]

    avg_wait /= num
    avg_turnaround /= num
    avg_response /= num

    print_(avg_wait, avg_turnaround, avg_response)


def SRTF(Process):
    readyQ = []

    burst = [0 for i in range(num)]
    for i in range(num):
        burst[i] = Process[i].burst

    for i in range(num):
        processing_time[i] = []

    sort_arrive(Process)

    for i in range(i-1):
        for j in range(num-i):
            if (Process[j].arrive == Process[j + 1].arrive and Process[j].burst > Process[j + 1].burst):
                temp = Process[j + 1]
                Process[j + 1] = Process[j]
                Process[j] = temp

    total = 0
    complete = 0
    turnaround = 0
    wait = 0
    time = 0
    avg_wait = 0
    avg_turnaround = 0
    avg_response = 0
    process_num = 0
    done = []
    done_num = 0

    Slist = []

    for j in range(num):
        Slist.append(Process[j])
        total += Process[j].burst

    present = Slist[0]
    Slist.pop(0)
    pt = processing(0, 0, 0)

    while (time <= fin_time):
        while (True):
            if (len(Slist) != 0 and Slist[0].arrive == time):
                tmp = Slist[0]
                Slist.pop()
                if (tmp.burst < present.burst):
                    pt.end = time
                    pt.burst = time - pt.start
                    if (pt.burst != 0):
                        processing_time[present.name].append(pt)
                    pt.start = time

                    heapq.heappush(readyQ, present)
                    present = tmp
                else:
                    heapq.heappush(readyQ, tmp)
            else:
                break
        if (present.burst == 0):
            name = present.name
            burst = present.burst
            arrive = present.arrive
            complete = time

            if (done_find(done, done_num, name) == False):
                done[done_num] = name
                done_num += 1
                avg_response += complete - burst_time[name] - arrive
                response_time[name] = complete - burst_time[name] - arrive

            wait = complete - burst_time[name] - arrive
            turnaround = complete - arrive
            turnaround_time[name] = turnaround
            wait_time[name] = wait

            avg_wait += wait
            avg_turnaround += turnaround

            pt.end = time
            pt.burst = time - pt.start
            if (pt.burst != 0):
                processing_time[name].append(pt)
            pt.start = time
            if (len(readyQ) != 0):
                present = readyQ[0]
                heapq.heappop(readyQ)
            elif (time != fin_time and len(readyQ) == 0):
                print("asdf")
            else:
                break
        present.burst -= 1
        time = time + 1
    avg_turnaround /= num
    avg_wait /= num
    avg_response /= num

    for i in range(num):
        Process[i].burst = burst[i]
    print_(avg_wait, avg_turnaround, avg_response)


def RR(Process):
    sort_arrive(Process)

    sum = 0
    complete = 0
    turnaround = 0
    wait = 0

    avr_wait = 0.0
    avr_turnaround = 0.0
    avr_response = 0.0
    Plist = []
    readyQ = []

    for i in range(num):
        processing_time[i] = []

    for i in range(num):
        Plist.append(Process[i])
        sum += Process[i].burst

    i = 0
    while (i < sum):
        while (True):
            if (len(Plist) != 0 and Plist[0].arrive <= i):
                heapq.heappush(readyQ, Plist[0])
                Plist.pop()
            else:
                break

        ing = readyQ[0]
        readyQ.pop()

        name_ = ing.name
        burst_ = ing.burst
        arrive_ = ing.arrive
        pt = processing(i, 0, 0)

        if (burst_ > TQ):
            while (True):
                if (len(Plist) != 0 and Plist.front().arrive <= i + TQ):
                    heapq.heappush(readyQ, Plist[0])
                    Plist.pop()
                else:
                    break

            pt.end = pt.start + TQ
            pt.burst = TQ
            processing_time[name_].append(pt)
            pt.start = pt.end

            ing.burst -= TQ
            readyQ.push(ing)
            i = i + TQ
        else:
            while (True):
                if (len(Plist) != 0 and Plist.front().arrive <= i + burst_):
                    heapq.heappush(readyQ, Plist[0])
                    Plist.pop()
                else:
                    break

            pt.end = pt.start + burst_
            pt.burst = burst_
            if (pt.burst != 0):
                processing_time[name_].append(pt)
            pt.start = pt.end

            complete = i + burst_
            turnaround = complete - arrive_
            wait = complete - arrive_ - burst_time[name_]
            complete_time[name_] = complete
            turnaround_time[name_] = turnaround
            wait_time[name_] = wait

            avr_wait += wait
            avr_turnaround += turnaround

            i = i + burst_

        response_time[name_] = processing_time[name_][0].start - arrive_
        avr_response += response_time[name_]

    avr_wait /= num
    avr_turnaround /= num
    avr_response /= num

    print_(avr_wait, avr_turnaround, avr_response)


def Priority(Process):
    sort_arrive(Process)

    pt = processing(0, 0, 0)
    avr_wait = 0.0
    avr_turnaround = 0.0
    avr_response = 0.0

    for i in range(num):
        processing_time[i] = []

    for i in range(num):
        j = i
        while (Process[j].arrive <= pt.end):
            if (j >= num):
                break
            j += 1
        sort_priority(Process, i, j)

        name_ = Process[i].name
        burst_ = Process[i].burst
        arrive_ = Process[i].arrive

        pt.start = pt.end
        pt.end = burst_ + pt.end
        pt.burst = burst_
        processing_time[name_].append(pt)

        wait_time[name_] = pt.start - arrive_
        avr_wait += wait_time[name_]

        turnaround_time[name_] = pt.end - arrive_
        avr_turnaround += turnaround_time[name_]

        response_time[name_] = pt.start - arrive_
        avr_response += response_time[name_]

    avr_wait /= num
    avr_turnaround /= num
    avr_response /= num
    print_(avr_wait, avr_turnaround, avr_response)


def sort_priority(Process, i, temp):
    tmp = process()
    min = process()
    index = 0
    for x in range(i, temp-1):
        min.priority = 100
        for j in range(x, temp):
            if (min.priority > Process[j].priority):
                min = Process[j]
                index = j
        tmp = Process[x]
        Process[x] = Process[index]
        Process[index] = tmp


def print_(avr_wait, avr_turnaround, avr_response):
    """ 간트차트는 start 오름차순 정렬해서 죽 뽑으면 될듯. """

    for i in range(num):
        print("P", i)
        for j in range(len(processing_time[i])):
            print(processing_time[i][j].start)
            print(processing_time[i][j].end)
            print(processing_time[i][j].burst)

    print("대기시간")
    for i in range(num):
        print(i, wait_time[i])

    print("프로세스별 반환시간 : ")
    for i in range(num):
        print(i, turnaround_time[i])

    print("프로세스별 응답시간")
    for i in range(num):
        print(response_time[i])

    print(avr_wait)
    print(avr_turnaround)
    print(avr_response)


FCFS(Process)
SJF(Process)
SRTF(Process)
RR(Process)
Priority(Process)
