from __future__ import unicode_literals
from django.shortcuts import render
import requests
import csv
from .train import train_model
from django.http import HttpResponse
import json
from .send_email import send_email
from datetime import datetime
from apscheduler.schedulers.blocking import BlockingScheduler

guess = ""
date = ""
res_data = []

def send_data(request):
    resp ={}
    if request.method == 'GET':
        resp['status'] = 'Success'
        resp['status_code'] = '200'
        resp['data'] = res_data
    else :
        resp['status'] = 'Fail'
        resp['status_code'] = '400'
        resp['message'] = "Error method"
    return HttpResponse(json.dumps(resp), content_type='application/json')


def guess_weather():
    response = requests.get("https://api.thingspeak.com/channels/2126333/feeds.json?results=2")
    for data in (response.json["feeds"]):
        temp = data['field1']
        pressure = data['field2']
        wind_speed = data['field3']
        rain = data['field4']
        direction = data['field5']
        humid = data['field6']
        res_data.append(temp)
        res_data.append(pressure)
        res_data.append(wind_speed)
        res_data.append(rain)
        res_data.append(direction)
        res_data.append(humid)
    date = datetime.strftime(date, "%d-%m-%Y %H:%M:%S" )
    #train chỉ truyền vào temp, pressure, wind_speed, direction, humid
    guess = train_model()
    res_data.append(date)
    res_data.append(guess)
    with open('data.csv' 'a') as f:
        writer = csv.writer(f)
        writer.writerow([date, temp, pressure, wind_speed, rain, direction, humid, guess])

def send_weather_email():
    send_email(guess, date)


