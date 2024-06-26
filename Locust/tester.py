from locust import HttpUser, TaskSet, task, between
import logging
import sys
import os
import unicodedata  # for text normalization
from dirlisting import dir_list
# pip install locust
# locust -f tester.py --host=http://localhost:8080

# Configure logging
logger = logging.getLogger()
logger.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s | %(levelname)s | %(message)s')

stdout_handler = logging.StreamHandler(sys.stdout)
stdout_handler.setLevel(logging.INFO)
stdout_handler.setFormatter(formatter)

file_handler = logging.FileHandler('logs.log')
file_handler.setLevel(logging.INFO)
file_handler.setFormatter(formatter)


logger.addHandler(file_handler)
logger.addHandler(stdout_handler)




def read_expected_content(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.read().strip()  # Read and strip whitespace
    except FileNotFoundError:
        logger.error(f"File not found: {file_path}")
        return None
    except Exception as e:
        logger.error(f"Error reading file: {e}")
        return None
    
error404 = read_expected_content( "/home/codespace/recovery/turnInPage/404.html" )
index = read_expected_content( "/home/codespace/recovery/turnInPage/index.html" )
files_dir = dir_list("/home/codespace/recovery/turnInPage/files", "files", "./turnInPage/files/")


def check_file_body(expected, actual):
    normalized_expected = unicodedata.normalize('NFKC', expected.strip())
    normalized_actual = unicodedata.normalize('NFKC', actual.strip())
    return normalized_actual == normalized_expected


class UserBehavior(TaskSet):

    """    @task(1)
    def root(self):
        with self.client.get("", catch_response=True) as response:
            response.encoding = 'utf-8'
            if response.status_code == 200 and check_file_body(index, response.text):
                response.success()
            else:
                response.failure(f"Expected 200 but got {response.status_code}")
                logger.info(f"Response content for /: {response.text}")

    @task(2)
    def index(self):
        with self.client.get("/index.html", catch_response=True) as response:
            response.encoding = 'utf-8'
            if response.status_code == 200 and check_file_body(index, response.text):
                response.success()
            else:
                response.failure(f"Expected 200 but got {response.status_code}")
                logger.info(f"Response content for /index.html: {response.text}")

    @task(3)
    def not_found(self):
        with self.client.get("/this_is_not_found", catch_response=True) as response:

            response.encoding = 'utf-8'
            if response.status_code == 404:
                normalized_expected = unicodedata.normalize('NFKC', error404.strip())
                normalized_actual = unicodedata.normalize('NFKC', response.text.strip())
                if normalized_expected != normalized_actual:
                    response.failure(f"Expected {error404} but got {response.text}")
                else:
                    response.success()

            else:
                response.failure(f"Expected 404 but got {response.status_code}")
                logger.info(f"Response content for /this_is_not_found: {response.text}")

    
    @task(4)
    def cgi(self):
        with self.client.get("/cgi/nolen.py", catch_response=True) as response:
            response.encoding = 'utf-8'
            if response.status_code == 200:
                response.success()
            else:
                response.failure(f"Expected 200 but got {response.status_code}")
                logger.info(f"Response content for /cgi/nolen.py: {response.text}")
    @task(5)
    def post_cgi(self):
        payload = {"login": "my_login", "password": "my_password"}

        with self.client.post("/cgi/nolen.py", json=payload, catch_response=True) as response:
            response.encoding = 'utf-8'
            if response.status_code == 200:
                response.success()
            elif response.status_code == 504:
                logger.info(f"Response content for /: {response.text}")
                response.failure()
            else:
                response.failure(f"Expected 200 but got {response.status_code}")
                logger.info(f"Response content for /cgi/nolen.py: {response.text}")"""
    @task(6)
    def files_dir(self):
        with self.client.get("/files/", catch_response=True) as response:
            response.encoding = 'utf-8'
            if response.status_code == 200 and response.text == files_dir:
                response.success()
            elif response.status_code == 504:
                logger.info(f"Response content for /: {response.text}")
                response.failure()
            else:
                response.failure(f"Expected 200 but got {response.status_code}")
                logger.info(f"Response content for /files: {response.text} but expected {files_dir}")
    

class WebsiteUser(HttpUser):
    tasks = [UserBehavior]
    wait_time = between(1, 5)


if __name__ == "__main__":
    os.system("locust -f tester.py --host=http://localhost:8080")