from flask import Flask, render_template, request, redirect, url_for, Response, session
from werkzeug import secure_filename
from functools import wraps
import os
import time
app = Flask(__name__)
games = {}
num = 0
setup_pipe = 'setup'
setup = ''

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

@app.route('/tictactoe')
def tic_tac_toe():
    return render_template('tic_tac_toe.html')

@app.route('/tictactoe/setup')
def tic_tac_toe_setup():
    """
        Get request called by processin.js to start a new game
    """
    global num
    num = num + 1

    global setup
    setup = open(setup_pipe, 'w')
    setup.write(str(num) + "\n")
    setup.flush()

    #os.sleep(1)
    os.mkfifo('cpipe_'+str(num), 0666);
    os.mkfifo('spipe_'+str(num), 0666);

    time.sleep(2)
    print "HERE"
    read_pipe = open('spipe_'+str(num), os.O_RDONLY | os.O_NONBLOCK)
    print "HERE2"
    write_pipe = open('cpipe_'+str(num), 'w')
    print "HERE1"

    global games
    games[num] = [read_pipe, write_pipe]
    return str(num)

@app.route('/tictactoe/<game_id>/<move>')
def tictactoe_move(game_id, move):
    #Write move to server then read move back
    g_num = int(game_id)
    games[g_num][1].write(move + '\n')
    games[g_num][1].flush()
    return_move = games[g_num][0].read()

    return return_move.strip()


if __name__ == '__main__':
    app.debug = True
    app.run(host='127.0.0.1', port=8888)
