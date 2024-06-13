from locust import HttpUser, TaskSet, task, between

#pip install locust
#locust -f tester.py --host=localhost:8080

class UserBehavior(TaskSet):
    @task
    def get_root(self):
        self.client.get("")

class WebsiteUser(HttpUser):
    tasks = [UserBehavior]
    wait_time = between(1, 5) 