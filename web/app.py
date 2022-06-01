from flask import Flask, render_template, redirect, request, url_for
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import random

import ctypes


app = Flask(__name__)


def random_color():
    return '{0:x}'.format(random.randint(200, 255))+'{0:x}'.format(random.randint(175, 205))+'{0:x}'.format(random.randint(175, 205))


scheduler = [{'schedulerName': "FCFS", 'processes': [{'pid': 'p1', 'startTime': 0, 'endTime': 10, 'waitingTime': 0, 'background': random_color()}, {'pid': 'p2', 'startTime': 10, 'endTime': 11, 'waitingTime': 10, 'background': random_color()}, {'pid': 'p3', 'startTime': 11, 'endTime': 13, 'waitingTime': 11, 'background': random_color()}], 'mx': 13, 'averageWaitingTime': 10}, {
    'schedulerName': "FCFS", 'processes': [{'pid': 'p1', 'startTime': 0, 'endTime': 10, 'waitingTime': 0, 'background': random_color()}, {'pid': 'p2', 'startTime': 10, 'endTime': 11, 'waitingTime': 10, 'background': random_color()}, {'pid': 'p3', 'startTime': 11, 'endTime': 13, 'waitingTime': 11, 'background': random_color()}], 'mx': 13, 'averageWaitingTime': 10}]


@app.route('/')
def hello_world():
    return render_template('index.html', scheduler=scheduler)


@ app.route('/', methods=['post'])
def form_submit():
    data = request.form.getlist('pid')
    print(data)
    return ('', 204)


if __name__ == '__main__':
    app.run()

processes = [{
    "pid": "p",
    "startTime": 0,
    "endTime": 1
}, {
    "pid": "p",
    "startTime": 1,
    "endTime": 10
}, {
    "pid": "p",
    "startTime": 10,
    "endTime": 15
}, {
    "pid": "p",
    "startTime": 15,
    "endTime": 19
}, {
    "pid": "p",
    "startTime": 19,
    "endTime": 23
}]

colors = ['red', 'orange', 'blue', 'green']


fig, gnt = plt.subplots()


gnt.set_xlabel('Time')
gnt.set_ylabel('Processor')

# Setting ticks on y-axis
# gnt.set_yticks([15, 25, 35])
# Labelling tickes of y-axis
# gnt.set_yticklabels(['1', '2', '3'])
gnt.set_xticklabels([])
gnt.set_yticklabels([])

# Setting graph attribute
gnt.grid(False)

# Declaring a bar in schedule
for i, p in enumerate(processes):
    gnt.broken_barh([(p['startTime'], p['endTime']-p['startTime'])],
                    (0, 1), facecolors=('tab:{}'.format(colors[i % 4])))

plt.savefig("gantt1.png")
