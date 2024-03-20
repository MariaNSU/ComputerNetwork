import requests
from bs4 import BeautifulSoup
import sys

storage_path = 'D:\\downloadedImages'
url = 'https://www.wikipedia.org/'
image_urls = []
data = ''
try:
    response = requests.get(url)
    if response.status_code != 200:
        print('Error in data getting:', response.status_code)
        sys.exit(0)
    data = response.text
except requests.exceptions.RequestException as e:
    print('Request error with data:', e)

soup = BeautifulSoup(data, 'html.parser')
images = soup.find_all('img')

for img in images:
    try:
        image_response = requests.get(f'{url}{img.get('src')}')
        if image_response.status_code != 200:
            print('Error in image getting:', image_response.status_code)
            sys.exit(0)
        img_name = f'{storage_path}/{img.get('src').split('/')[-1]}'
        with open(img_name, 'wb') as file:
            file.write(image_response.content)
        print('Successful download: ', img_name)

    except requests.exceptions.RequestException as e:
        print('Request error with img:', e)




