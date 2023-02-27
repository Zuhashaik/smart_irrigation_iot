import pandas as pd
import requests
import json
import csv


#samples:
# hubli-dharwad(iiitdwd)= "15.393028015866818, 75.02535657811156" 
# Karumba QLD 4891, Australia= "-17.51114921748322, 140.86299454209237 "
gps_coordinates=input("Enter coordinates of your place: ")

url = "https://api.weatherapi.com/v1/forecast.json?key=c685be0d28b8490a811100656232102&q="+gps_coordinates+"&days=9&aqi=no&alerts=no"

# Make API request
response = requests.get(url)

# Convert response to JSON
data = json.loads(response.content)

# Extract data for each hour and save to CSV
file='data.csv'
with open(file, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Date', 'Time', 'Temperature (C)', 'Precipitation (mm)', 'Precipitation(%)', 'Will_it_rain?'])
    for forecast in data['forecast']['forecastday']:
        date = forecast['date']
        for hour in forecast['hour']:
            writer.writerow([date, hour['time'], hour['temp_c'], hour['precip_mm'], hour['chance_of_rain'], hour['will_it_rain']])


df=pd.read_csv("data.csv")
# Get current time from user input
time_now = int(input("What's the current time (in hours, 0-23): "))

# Check if it will rain in the next 3 days
for i, will_rain in enumerate(df['Will_it_rain?']):
    if i >= time_now and will_rain == 1:
        print(f"Yes at {df.loc[i, 'Time']} {df.loc[i, 'Precipitation (mm)']} mm")
        break
else:
    print("There will be no rainfall for the next 3 days.")
