from __future__ import unicode_literals
from django.http import HttpResponse
import json
import pandas as pd


def send_data(request):
    resp ={}
    res_data = ""
    df = pd.read_csv('data.csv', names=['date', 'temp', 'pressure', 'wind_speed', 'rain', 'direction', 'humid', 'guess'])
    res_data =df.tail(1).to_dict('records')
    print(res_data)
    if request.method == 'GET':
        resp['status'] = 'Success'
        resp['status_code'] = '200'
        resp['data'] = res_data
    else :
        resp['status'] = 'Fail'
        resp['status_code'] = '400'
        resp['message'] = "Error method"
    return HttpResponse(json.dumps(resp), content_type='application/json')





