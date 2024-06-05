#!/usr/bin/env python3

import subprocess
import time

subprocess.run(["rabbitmq-plugins", "enable", "rabbitmq_mqtt"])
time.sleep(10)
subprocess.run(["rabbitmqadmin", "declare", "queue", "name=CounterQueue"])
subprocess.run(["rabbitmqadmin", "declare", "binding", "source=amq.topic", "destination=CounterQueue", "routing_key=Counter"])