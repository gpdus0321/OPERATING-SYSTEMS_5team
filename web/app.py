import heapq
import copy
from flask import Flask, render_template, request
import random


app = Flask(__name__)
scheduler = []
randomc = []


def calc(num, TQ, __p):
    class process:
        name = 0
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
                return self.arrive < other.arrive
            return self.burst < other.burst

    class processing:
        start = 0
        end = 0
        burst = 0

        def __init__(self, start, end, burst):
            self.start = start
            self.end = end
            self.burst = burst

    global fin_time
    fin_time = 0

    processing_time = [[]
                       for i in range(num)]
    burst_time = []
    complete_time = [0 for i in range(num)]
    wait_time = [0 for i in range(num)]
    turnaround_time = [0 for i in range(num)]
    response_time = [0 for i in range(num)]
    Process = []

    for i in range(num):
        p = process(i, __p[i].get('arrive'), __p[i].get(
            'burst'), __p[i].get('priority'))
        Process.append(p)
        burst_time.append(__p[i].get('burst'))

    def sort_arrive():
        for i in range(num):
            for j in range(num - 1 - i):
                if (Process[j].arrive > Process[j + 1].arrive):
                    temp = copy.deepcopy(Process[j + 1])
                    Process[j + 1] = copy.deepcopy(Process[j])
                    Process[j] = copy.deepcopy(temp)

    def FCFS():
        sort_arrive()

        time = 0

        avg_wait = 0.0
        avg_turnaround = 0.0
        avg_response = 0.0

        for i in range(num):
            processing_time[i] = []

        for i in range(num):
            burst_t = Process[i].burst
            arrive_t = Process[i].arrive
            tmp = processing(time, burst_t + time, burst_t)

            name = Process[i].name
            processing_time[name].append(tmp)

            if time < arrive_t:
                time = arrive_t

            wait_time[name] = time - arrive_t
            avg_wait += time - arrive_t

            turnaround_time[name] = time + burst_t - arrive_t
            avg_turnaround += time + burst_t - arrive_t

            burst_time[name] = burst_t

            complete_time[name] = time + burst_t

            response_time[name] = time - arrive_t
            avg_response += time - arrive_t

            time += burst_t
            global fin_time
            fin_time = time

        avg_wait /= num
        avg_turnaround /= num
        avg_response /= num
        print_("FCFS", avg_wait, avg_turnaround, avg_response)

    def done_find(done, done_len, num):
        for i in range(done_len):
            if (done[i] == num):
                return True
        return False

    def SJF():
        readyQ = []

        for i in range(num):
            processing_time[i] = []

        sort_arrive()
        for i in range(num):
            for j in range(num-i-1):
                if (Process[j].arrive == Process[j + 1].arrive and Process[j].burst > Process[j + 1].burst):
                    temp = Process[j + 1]
                    Process[j + 1] = Process[j]
                    Process[j] = temp

        time = 0
        avg_wait = 0
        avg_turnaround = 0
        avg_response = 0

        done = [0 for i in range(num)]
        done_num = 0

        process_num = 0
        present = Process[process_num]
        name = present.name

        global fin_time
        while (time < fin_time):
            name = present.name
            done[done_num] = name
            done_num += 1

            burst_t = present.burst
            arrive_t = present.arrive

            if (arrive_t > time):
                time = arrive_t

            tmp = processing(time, burst_t + time, burst_t)
            processing_time[name].append(tmp)

            wait_time[name] = time - arrive_t
            avg_wait += time - arrive_t

            turnaround_time[name] = burst_t + time - arrive_t
            avg_turnaround += burst_t + time - arrive_t

            response_time[name] = time - arrive_t
            avg_response += time - arrive_t

            time += burst_t

            for i in range(num):
                readyQ2 = copy.deepcopy(readyQ)
                isin = False
                for j in range(len(readyQ)):
                    tmp = heapq.heappop(readyQ2)
                    if (tmp.name == Process[i].name):
                        isin = True
                if (Process[i].arrive <= time) and (done_find(done, done_num, Process[i].name) == False) and (isin == False):
                    heapq.heappush(readyQ, Process[i])

            if (len(readyQ) > 0):
                next_process = readyQ[0].name
                heapq.heappop(readyQ)
                for i in range(num):
                    if (Process[i].name == next_process):
                        process_num = i

            present = Process[process_num]

        avg_wait /= num
        avg_turnaround /= num
        avg_response /= num

        print_("SJF", avg_wait, avg_turnaround, avg_response)

    def SRTF():
        readyQ = []

        burst = [0 for i in range(num)]
        for i in range(num):
            burst[i] = Process[i].burst

        for i in range(num):
            processing_time[i] = []

        sort_arrive()

        for i in range(i):
            for j in range(num-i-1):
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
        done = [0 for i in range(num)]
        done_num = 0

        Slist = []

        for j in range(num):
            Slist.append(Process[j])
            total += Process[j].burst
            response_time[j] =- 1

        present = Slist[0]
        Slist.pop(0)
        pt = processing(0, 0, 0)

        global fin_time
        while (time <= fin_time):
            while (True):
                if (len(Slist) != 0 and Slist[0].arrive == time):
                    tmp = Slist[0]
                    Slist.pop(0)
                    if (tmp.burst < present.burst):
                        pt.end = time
                        pt.burst = time - pt.start
                        if (pt.burst != 0):
                            processing_time[present.name].append(
                                copy.deepcopy(pt))
                        pt.start = time

                        heapq.heappush(readyQ, present)
                        present = tmp
                    else:
                        heapq.heappush(readyQ, tmp)
                else:
                    break
            if (present.burst == 0):
                _name = present.name
                _burst = present.burst
                _arrive = present.arrive
                complete = time

                if (done_find(done, done_num, _name) == False):
                    done[done_num] = _name
                    done_num += 1

                wait = complete - burst_time[_name] - _arrive
                turnaround = complete - _arrive
                turnaround_time[_name] = turnaround
                wait_time[_name] = wait

                avg_wait += wait
                avg_turnaround += turnaround

                pt.end = time
                pt.burst = time - pt.start
                if (pt.burst != 0):
                    processing_time[_name].append(copy.deepcopy(pt))
                pt.start = time
                if (len(readyQ) != 0):
                    present = readyQ[0]
                    heapq.heappop(readyQ)
                elif (time != fin_time and len(readyQ) == 0):
                    print("asdf")
                else:
                    break
            if(response_time[present.name] == -1):
                response_time[present.name] = time - present.arrive
                avg_response += response_time[present.name]
                
            present.burst -= 1
            time = time + 1
        avg_turnaround /= num
        avg_wait /= num
        avg_response /= num

        for i in range(num):
            Process[i].burst = burst[i]
        print_("SRTF", avg_wait, avg_turnaround, avg_response)

    def RR():
        sort_arrive()

        sum = 0
        complete = 0
        turnaround = 0
        wait = 0

        avr_wait = 0.0
        avr_turnaround = 0.0
        avr_response = 0.0
        Plist = []
        readyQ = []

        pt = processing(0, 0, 0)

        for i in range(num):
            processing_time[i] = []

        for i in range(num):
            Plist.append(copy.deepcopy(Process[i]))
            sum += Process[i].burst

        i = 0
        while (i < sum):
            while True:
                if (len(Plist) != 0 and Plist[0].arrive <= i + TQ):
                    readyQ.append(Plist.pop(0))
                else:
                    break

            ing = readyQ.pop(0)

            name_ = ing.name
            burst_ = ing.burst
            arrive_ = ing.arrive
            pt.start = i

            if (burst_ > TQ):
                while (len(Plist) != 0 and Plist[0].arrive <= i + TQ):
                    readyQ.append(Plist.pop(0))

                pt.end = pt.start + TQ
                pt.burst = TQ
                processing_time[name_].append(copy.deepcopy(pt))
                pt.start = pt.end

                ing.burst -= TQ
                readyQ.append(ing)
                i = i + TQ
            else:
                while (True):
                    if (len(Plist) != 0 and Plist[0].arrive <= i + burst_):
                        readyQ.append(Plist[0])
                        Plist.pop(0)
                    else:
                        break

                pt.end = pt.start + burst_
                pt.burst = burst_
                if (pt.burst != 0):
                    processing_time[name_].append(copy.deepcopy(pt))
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
        
        for i in range(num):
            avr_response += response_time[i]
            
            
        avr_wait /= num
        avr_turnaround /= num
        avr_response /= num

        print_("RR", avr_wait, avr_turnaround, avr_response)

    def Priority():
        sort_arrive()

        pe = 0
        avr_wait = 0.0
        avr_turnaround = 0.0
        avr_response = 0.0

        for i in range(num):
            processing_time[i] = []

        for i in range(num):
            j = i
            while (j < num and Process[j].arrive <= pe):
                j += 1
            sort_priority(i, j)

            name_ = Process[i].name
            burst_ = Process[i].burst
            arrive_ = Process[i].arrive

            pt = processing(pe, burst_ + pe, burst_)
            pe = pt.end
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
        print_("비선점 Priority", avr_wait, avr_turnaround, avr_response)

    def sort_priority(i, temp):
        index = 0
        for x in range(i, temp-1):
            min = process(0, 0, 0, 100)
            for j in range(x, temp):
                if (min.priority > Process[j].priority):
                    min = Process[j]
                    index = j
            tmp = Process[x]
            Process[x] = Process[index]
            Process[index] = tmp

    def print_(title, avr_wait, avr_turnaround, avr_response):
        for i in range(num):
            print("P", i)
            for j in range(len(processing_time[i])):
                print(processing_time[i][j].start, processing_time[i]
                      [j].end, "->", processing_time[i][j].burst)

        print("대기시간")
        for i in range(num):
            print(i, wait_time[i])

        print("프로세스별 반환시간 : ")
        for i in range(num):
            print(i, turnaround_time[i])

        print("프로세스별 응답시간")
        for i in range(num):
            print(response_time[i])

        print("평균 대기 ", avr_wait, "/ 평균 반환 ",
              avr_turnaround, "/ 평균 응답", avr_response)

        _p = []
        _pp = []
        mx = 0
        for i in range(num):
            for j in range(len(processing_time[i])):
                if (mx < processing_time[i][j].end):
                    mx = processing_time[i][j].end
                _p.append({'pid': __p[i].get('name'), 'startTime': processing_time[i][j].start,
                           'endTime': processing_time[i][j].end, 'background': random_color(i)})
            _pp.append({'pid': __p[i].get('name'), 'waitingTime': wait_time[i], 'turnaroundTime': turnaround_time[i],
                        'responseTime': response_time[i]})
        global scheduler
        scheduler.append(
            {'schedulerName': title, 'processes': _p, 'ps': _pp,
                'mx': mx, 'averageWaitingTime': avr_wait, 'averageTurnaroundTime': avr_turnaround, 'averageResponseTime': avr_response}
        )

    global scheduler
    scheduler = []
    FCFS()
    SJF()
    SRTF()
    RR()
    Priority()


def random_color(i):
    if (len(randomc) > i):
        return randomc[i]
    randomc.append('{0:x}'.format(200 + random.randint(0, 55))+'{0:x}'.format(
        175 + random.randint(0, 30))+'{0:x}'.format(175 + random.randint(0, 30)))
    return randomc[i]


@ app.route('/')
def hello_world():
    return render_template('index.html', scheduler=scheduler)


@ app.route('/', methods=['post'])
def form_submit():
    if (request.form.get('quota') == ''):
        return ('', 204)
    num = int(request.form.get('cnt'))
    __p = [{} for i in range(num)]
    for i in range(num):
        __p[i] = {'pid': request.form.get('pid{}'.format(i)), 'name': i, 'arrive': int(request.form.get('arrive_time{}'.format(
            i))), 'burst': int(request.form.get('service_time{}'.format(i))), 'priority': int(request.form.get('priority{}'.format(i)))}

    calc(num, int(request.form.get('quota')), __p)
    return render_template('index.html', scheduler=scheduler)


if __name__ == '__main__':
    app.run()
