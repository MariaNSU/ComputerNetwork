import requests
import sys
import matplotlib.pyplot as plt
from datetime import datetime

url = 'https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=temperature_2m,wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m'
data = ''
try:
    response = requests.get(url)
    if response.status_code != 200:
        print('Error in data getting:', response.status_code)
        sys.exit(0)
    data = response.json()
except requests.exceptions.RequestException as e:
    print('Request error with data:', e)

dates_times = data["hourly"]["time"]
temp = data['hourly']['temperature_2m']
modified_dates = []

for date_time in dates_times:
    date_obj = datetime.strptime(date_time, "%Y-%m-%dT%H:%M")
    modified_dates.append(date_obj.strftime("%d.%m"))

modified_dates = sorted(list(set(modified_dates)))


fig = plt.figure(figsize=(10, 8))
plt.plot(dates_times, temp)

plt.xticks(dates_times[::24], modified_dates)
plt.xlabel('Date', fontsize=12)
plt.ylabel('Temperature, C', fontsize=12)
plt.title('Temperature graph', fontsize=12)

plt.show()
