import numpy as np
from tensorflow.keras.models import load_model

def train_model(temp, pressure, wind_speed, direction, humid):
    label_names = {
        0: 'Partly Cloudy',
        1: 'Mostly Cloudy ',
        2: 'Overcast',
        3: 'Clear',
        4: 'Foggy',
        5: 'Breezy and Overcast',
        6: 'Breezy and Mostly Cloudy',
        7: 'Breezy and Partly Cloudy',
        8: 'Dry and Partly Cloudy',
        9: 'Windy and Partly Cloudy',
        10: 'Light Rain'
    }
    model = load_model('weather_model.h5')
    predict = model.predict([[float(temp), float(pressure), float(wind_speed), float(direction), float(humid)]])
    predicted_class = np.argmax(predict)
    predicted_class_name = label_names[predicted_class]
    print('Lớp đích được dự đoán là:', predicted_class_name)
    return predicted_class_name