# static server hosting local files
from flask import Flask, send_from_directory, request
import os
import dotenv

dotenv.load_dotenv()
HOST = os.getenv('HOST')
PORT = os.getenv('PORT')

app = Flask(__name__)

@app.route('/')
def index():
    return send_from_directory('static', 'index.html')

@app.route("/heartbeat")
def heartbeat():  
    return "OK"

if __name__ == "__main__":
    app.run(host=HOST, port=PORT)