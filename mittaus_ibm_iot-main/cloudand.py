import os
import requests
from dotenv import load_dotenv

"""
IAM autentication values are retrieved from .env file
"""

IAM_LOGIN_URL = 'https://iam.cloud.ibm.com/identity/token?grant_type=urn:ibm:params:oauth:grant-type:apikey&apikey='
DATABASE = "sirq"

def login():
    # Login
    login_url = IAM_LOGIN_URL + os.environ.get('CLOUDANT_APIKEY')
    response = requests.post(login_url, data=None)
    print(response.json())
    return response.json()["access_token"]

# Post value
def post_value(token, value):
    url = f'{os.environ.get("CLOUDANT_URL")}/{DATABASE}'
    response = requests.post(url, json={
        "measure": value
    }, headers={
        'authorization': f'Bearer {token}',
    })
    print(response.json())
    return response.json()

if __name__ == '__main__':
    load_dotenv()
    token = login()
    post_value(token=token, value=20)