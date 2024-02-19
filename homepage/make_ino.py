import uuid
import time
import json
import dotenv
import os

dotenv.load_dotenv()

network_heartbeat_url = os.getenv('NETWORK_HEARTBEAT_URL')

node = {
    "NODE_ID": str(uuid.uuid4()),
    "NODE_NAME": "homepage_demo",
    "NODE_TIMESTAMP": str(int(time.time()*1000)),
    "NODE_ORIGIN": "blocks_v2 homepage demo",
}

config = {
    "WIFI_SSID": "testbench",
    "WIFI_PASS": "testbench",
    "NETWORK_HEARTBEAT_URL": network_heartbeat_url,
}

with open("./static/embedded.html", "r") as f:
    embedded_template = f.read()

with open("./node/node_template.ino", "r") as f:
    node_template = f.read()

for key, value in config.items():
    node_template = node_template.replace(key, value)

for key, value in node.items():
    node_template = node_template.replace(key, value)
    embedded_template = embedded_template.replace(key, "'" + value + "'")

node_template = node_template.replace("NODE_HOME", embedded_template.replace('"', '\\"').replace("\n", "\\n"))
node_template = node_template.replace("NODE_JSON", json.dumps(node, indent=4).replace('"', '\\"').replace("\n", "\\n"))

with open(f"./node/embedded_node/embedded_node.ino", "w") as f:
    f.write(node_template)