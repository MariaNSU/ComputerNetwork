import requests
from bs4 import BeautifulSoup
import sys

storage_path = 'D:\\downloadedImages'
url = 'https://www.picturesofengland.com/England/Cumbria/article/1583'
homeUrl = 'https://www.picturesofengland.com'

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
    img_path = img.get('src')
    if not(homeUrl in img_path):
        img_path = homeUrl+img_path
    try:
        image_response = requests.get(f'{img_path}')
        if image_response.status_code != 200:
            print('Error in image getting:', f'{url}{img.get('src')}')
            sys.exit(0)
        img_name = f'{storage_path}/{img_path.split('/')[-1]}'
        with open(img_name, 'wb') as file:
            file.write(image_response.content)
        print('Successful download: ', img_name)
    except requests.exceptions.RequestException as e:
        print('Request error with img:', e)




