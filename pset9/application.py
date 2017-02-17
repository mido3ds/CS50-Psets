from cs50 import SQL
from flask import Flask, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from random import randrange
from os import urandom
from datetime import datetime
from subprocess import call

from helpers import usd, login_required, apology, lookup

# configure application
app = Flask(__name__)

# my configurations
app.config.update(
    TEMPLATES_AUTO_RELOAD=True,
    SECRET_KEY=urandom(40),
    HOST='0.0.0.0',
    PORT=randrange(5000, 9001),
    DEBUG=False,
)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    # get cash and make it float
    try: 
        cash = float(
                db.execute(
                'SELECT cash FROM  Users WHERE id = :id',
                id=session['user_id']
            )[0]['cash']
        )
    except IndexError:
        # user is not in DataBase, some error happened

        # clear session and get him to login
        session.clear()
        return redirect(url_for('login'))

    # get stocks for this id 
    stocks = get_user_stocks()

    # add now prices to stocks, calc grand total
    grand_total = cash
    for symbol in stocks:
        stocks[symbol]['price'] = lookup(symbol)['price'] * int(stocks[symbol]['num_shares'])
        grand_total += stocks[symbol]['price']

    return render_template(
        'index.html',
        stocks=stocks,
        cash=cash,
        grand_total=grand_total,
    )

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == 'GET':
        return render_template('buy.html')
    else:
        # check
        if request.form['symbol'] == '' or request.form['num_shares'] == '':
            return apology('some fields are empty')
        
        if int(request.form['num_shares']) < 0:
            return apology('num_shares can\'t be negative')
        elif int(request.form['num_shares']) == 0:
            return apology('so', 'u want to buy nothing')

        # get money
        db_result = db.execute(
            'SELECT cash, username FROM  Users WHERE id = :id',
            id=session['user_id']
        )

        # save time now, e.g:
        # 2017-2-5 3:44:13
        time_purchase = '{:%Y-%m-%d %H:%M:%S}'.format(datetime.now()) 

        lookup_result = lookup(request.form['symbol'])
        if lookup_result is None:
            return apology('error happened', 'check the symbol plz')

        total_buy = lookup_result['price'] * float(request.form['num_shares'])

        # check money is enough
        if total_buy > float(db_result[0]['cash']):
            return apology('you don\'t have enough money', 'ur kiddin me?')
        
        # buy: update cash,
        db.execute(
            'UPDATE  Users SET cash = :cash WHERE id = :id',
            cash=float(db_result[0]['cash'])-total_buy,
            id=session['user_id'],
        )
        # and log it 
        db.execute(
            'INSERT INTO Buying(user_id, symbol, stock_price, num_shares)\
            VALUES(:user_id, :symbol, :price, :num_shares)',
            user_id=session['user_id'],
            symbol=lookup_result['symbol'],
            price=lookup_result['price'],
            num_shares=request.form['num_shares'],
       )

        return render_template(
            'bought.html',
            symbol=lookup_result['symbol'],
            price=usd(lookup_result['price']),
            num_shares=request.form['num_shares'],
            total_cash=usd(total_buy),
            user_name=db_result[0]['username'],
            time=time_purchase,
        )
        

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    # get logs 
    sell_rows = db.execute(
        'SELECT stock_price, num_shares, symbol, time FROM Selling WHERE user_id  = :id',
        id=session['user_id'],
    )
    buy_rows = db.execute(
        'SELECT stock_price, num_shares, symbol, time FROM Buying WHERE user_id  = :id',
        id=session['user_id'],
    )

    # send them
    return render_template(
        'history.html',
        sell_rows=sell_rows,
        buy_rows=buy_rows,
    )

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM Users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", 'POST'])
@login_required
def quote():
    """Get stock quote."""
    if request.method == 'GET':
        return render_template('quote.html')

    if request.method == 'POST':
        if request.form['symbol'] == '':
            return apology('no symbol provided')

        result = lookup(request.form['symbol'])

        if result is None:
            return apology('some error happened')
        
        return render_template(
            'quoted.html', 
            name=result['name'],
            symbol=result['symbol'],
            price=usd(result['price']),
        )
        

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == 'GET':
        return render_template('register.html')

    if request.method == 'POST':
        if request.form['user'] == '':
            return apology('You didn\'t type a user name')

        if user_is_registered(request.form['user']):
            return apology('User name is used, try another name')

        if request.form['password1'] == '' or request.form['password2'] == '':
            return apology('password can\'t be left empty')

        if request.form['password1'] != request.form['password2']:
            return apology('Passwords don\'t match')

        hash = pwd_context.hash(request.form['password1'])

        val = db.execute(
            'INSERT INTO Users (username, hash) VALUES(:user, :passw)', 
            user=request.form['user'], 
            passw=hash,
        )
        print(request.form['user'], request.form['password1'], val)

        return render_template('login.html')

def user_is_registered(user):
    ''' returns True if user is found in db '''
    result = db.execute('SELECT username FROM  Users WHERE username = :user', user=user)
    if len(result) == 1:
        return True
    return False

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # get user stocks
    stocks = get_user_stocks()

    if request.method == 'GET':
        return render_template(
            'sell.html',
            stocks=stocks,
        )

    if request.method == 'POST':
        symbol = request.form['symbol']
        lookup_price = lookup(symbol)['price']

        # sell some
        if request.form['radioOption'] == "Select some shares":
            num_shares = int(request.form['num_shares'])

            # num is psitive
            if num_shares < 0:
                return apology('num of shares can\'t be negative')

            if num_shares == 0:
                return apology('sooo', 'ur selling nothing')

            # num_shares must be <= available shares
            if num_shares > stocks[symbol]['num_shares']:
                return apology('ur selling more than you have')

        # sell all
        else:
            # make num_shares all what use have for this symbol
            num_shares = stocks[symbol]['num_shares']

        sell_price = lookup_price * num_shares

        # update user cash
        db.execute(
            'UPDATE Users \
            SET cash = cash + :sell_price',
            sell_price=sell_price,
        )
        # log it 
        db.execute(
            'INSERT INTO Selling(user_id, symbol, stock_price, num_shares) \
            VALUES(:user_id, :symbol, :price, :num_shares)',
            user_id=session['user_id'],
            symbol=request.form['symbol'],
            price=sell_price,
            num_shares=num_shares,
        )

        return redirect(url_for('index'))

def get_user_stocks():
    '''return dict of stocks that user have.

    dict:
    stocks = {
        '<symbol>':{
            'price':float,
            'num_shares':int
        },
        ..
    }
    '''
    sell = get_user_log('Selling')
    buy = get_user_log('Buying')
    stocks = {}

    for symbol in buy:
        if symbol in sell:
            # sub sell.price from buy.price
            buy[symbol]['num_shares'] -= sell[symbol]['num_shares']
    
        # add it to stocks
        if buy[symbol]['num_shares'] != 0:
            stocks[symbol] = buy[symbol]

    return stocks

def get_user_log(table):
    """return dict of log history in given table name

    dict:
    stocks = {
        '<symbol>':{
            'price':float,
            'num_shares':int
        },
        ..
    }
    """

    rows = db.execute(
        'SELECT symbol, stock_price, num_shares FROM :table WHERE user_id = :id',
        table=table,
        id=session['user_id'],
    )

    # add sold
    stocks = {} 
    for row in rows:
        symbol = row['symbol']

        # if not created, create it
        if symbol not in stocks:
            stocks[symbol] = {
                'price': float(row['stock_price']),
                'num_shares': int(row['num_shares']),
            }
        # dict is created, update num_shares in it
        else:
            stocks[symbol]['num_shares'] += int(row['num_shares'])
        
    return stocks

if __name__=='__main__':
    # open site in browser
    host=app.config['HOST']
    port=app.config['PORT']
    call(['open', 'http://{}:{}'.format(host, port)])

    app.run(host, port)