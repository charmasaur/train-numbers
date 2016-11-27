from flask import Markup, Flask, request, render_template

import train_py

app = Flask(__name__)

@app.route('/', methods=['GET'])
def train_numbers():
    val = request.args.get('number')
    exp = request.args.get('exp')
    response = ''
    if val:
        if train_py.go(val, True if exp else False):
            response = val + " works"
            if exp:
                response = response + ' (with exponentials... try again <a href="?number=' + val + '">without</a>)'
        else:
            response = val + " doesn't work"
            if exp:
                response = response + " (even with exponentials)"
            else:
                response = response + ' (try again <a href="?number=' + val + '&exp=on">with exponentials</a>)'
    return render_template(
            'form.html',
            response=Markup(response))
