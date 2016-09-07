from flask import Flask, render_template, request, redirect, url_for, Response, session
from werkzeug import secure_filename
import socket
from functools import wraps
import os
import time
app = Flask(__name__)
app.secret_key = '\xffd\xd4\xef\x1dN\x83)\x91\x99\xd15V \x1a\xc6\x04\xa6y\x92\x06\r\x8a\xba'
#games = 0
#for i in range(0, 8):
#    games.append(False)
GAME_PORT = 8001
num = 0
sockets = []
for i in range(0, 7):
    sockets.append(False)

def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if not 'user' in session:
            return redirect(url_for('login'))
        return f(*args, **kwargs)
    return decorated_function

@app.route('/')
def index():
    """
        Home page
    """
    return render_template('index.html')

@app.route('/projects')
def projects():
    """
        Home page
    """
    return render_template('projects.html')

@app.route('/tictactoe')
def tic_tac_toe():
    return render_template('tic_tac_toe.html')

@app.route('/tictactoe/setup')
def tic_tac_toe_setup():
    """
        Get request called by processin.js to start a new game
    """

    i = 0
    while i < 8:
        if sockets[i]  == False:
            break
        else:
            sockets[i].send("HEARTBEAT");
            val = sockets[i].recv(6).strip()
            if not val == "ALIVE":
                sockets[i] = False
                break
        i += 1



    s = socket.socket(
            socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect(("localhost", GAME_PORT))
    except:
        return "-2"
    
    print "Using number %d" % i
    sockets[i] = s
    return str(i)

@app.route('/tictactoe/<game_id>/<move>')
def tictactoe_move(game_id, move):
    #Write move to server then read move back
    i = int(game_id)
    socket = sockets[i]
    socket.send(move);
    chunk = socket.recv(1024).strip()
    return chunk
#
#@app.route('/tictactoe/disconnect/<game_id>')
#def tictactoe_disconnect('/tictactoe/disconnect/<game_id>'):
#    g_num = int(game_id)
#    games[game_id].close()
#    games[g_num] = False
#

if __name__ == '__main__':
    app.debug = True
    app.run(host='127.0.0.1', port=8888)
