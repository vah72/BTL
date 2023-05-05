import requests
from train import train_model
from send_email import send_email
import pytz
import threading
import pandas as pd
from datetime import datetime
from apscheduler.schedulers.blocking import BlockingScheduler

def guess_weather():
    response = requests.get("https://api.thingspeak.com/channels/2126333/feeds.json?results=2")
    # print(response.json()["feeds"])
    for data in (response.json()["feeds"]):
        temp = data['field1']
        pressure = data['field2']
        wind_speed = data['field3']
        rain = data['field4']
        direction = data['field5']
        humid = data['field6']
    
    #lấy mùi giờ Việt Nam
    utc_time = datetime.strptime(data['created_at'], '%Y-%m-%dT%H:%M:%SZ')
    vn_time = utc_time.replace(tzinfo=pytz.utc).astimezone(pytz.timezone('Asia/Ho_Chi_Minh'))
    date = datetime.strftime(vn_time, "%d-%m-%Y %H:%M:%S" )
    #train chỉ truyền vào temp, pressure, wind_speed, direction, humid
    guess = train_model(temp, pressure, wind_speed, direction, humid)
    print(date, temp, pressure, wind_speed, rain, direction, humid, guess)
    with open('data.csv', 'a') as f:
        f.writelines(f'{date},{temp},{pressure},{wind_speed},{rain},{direction},{humid},{guess}\n')
def send_weather_email():
    df = pd.read_csv('data.csv')
    date =df['date'].iloc[-1]
    print(date)
    guess =df['guess'].iloc[-1]
    send_email(guess, date)

def run_guess_scheduler():
    guess_scheduler = BlockingScheduler()
    guess_scheduler.add_job(guess_weather,'interval', seconds=10 )
    guess_scheduler.start()

def run_email_scheduler():
    email_scheduler = BlockingScheduler()
    email_scheduler.add_job(send_weather_email, 'cron', hour=15, minute=4, second=0, timezone='Asia/Ho_Chi_Minh')
    email_scheduler.start()


if __name__ == '__main__':
    guess = ""
    date = ""

    guess_thread = threading.Thread(target=run_guess_scheduler)
    email_thread = threading.Thread(target=run_email_scheduler)

    email_thread.start()
    guess_thread.start()
   

    guess_thread.join()
    email_thread.join()

