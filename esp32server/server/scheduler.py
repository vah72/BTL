from apscheduler.schedulers.blocking import BlockingScheduler
from django.core.management import call_command

email_scheduler = BlockingScheduler()
guess_scheduler = BlockingScheduler()

@email_scheduler.scheduled_job('cron', hour=7)
def send_weather_email():
    call_command('send_weather_email')

@guess_scheduler.scheduled_job('interval', minutes=2)
def backup_database():
    call_command('guess_weather')

