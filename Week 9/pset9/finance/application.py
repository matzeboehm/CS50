import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    user_id = session.get("user_id")
    transactions = db.execute("SELECT symbol, buy_sell, shares FROM transactions WHERE user_id = (?)", user_id)

    # cash
    available_cash = db.execute("SELECT cash FROM users WHERE id = (?)", session.get("user_id"))
    available_cash = float(available_cash[0]["cash"])

    # if no transactions happened yet
    if transactions == []:
        return render_template("index.html", total=round(available_cash, 2), cash=round(available_cash, 2))

    # dict of shares available and how many
    shares = {}
    for rows in transactions:
        if rows["symbol"] not in shares:
            shares[rows["symbol"]] = 0

        if rows["buy_sell"] == "BUY":
            shares[rows["symbol"]] = shares[rows["symbol"]] + rows["shares"]
        elif rows["buy_sell"] == "SELL":
            shares[rows["symbol"]] = shares[rows["symbol"]] - rows["shares"]

    # dict comprehension: delete all where 0 shares are available
    shares = {x: y for x, y in shares.items() if y != 0}

    # total value
    total = float(available_cash)
    export_shares = []
    for symbol in shares:
        # value
        api_values = lookup(symbol)
        stock_price = float(api_values["price"])
        # amount of shares
        amount = float(shares[symbol])
        total = total + stock_price * amount

        temp = []
        temp.append(symbol)
        temp.append(api_values["name"])
        temp.append(int(amount))
        temp.append(stock_price)
        temp.append(round(amount * stock_price, 2))
        export_shares.append(temp)

    if shares != {}:
        return render_template("index.html", export_shares=export_shares, total=round(total, 2), cash=round(available_cash, 2))
    else:
        return render_template("index.html", total=round(available_cash, 2), cash=round(available_cash, 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        api_values = lookup(symbol)
        # Check wether stock symbol exists
        if api_values == None:
            return apology("wrong ticker symbol", 400)

        available_cash = db.execute("SELECT cash FROM users WHERE id = (?)", session.get("user_id"))
        available_cash = float(available_cash[0]["cash"])
        # check if input is numeric
        try:
            shares = float(request.form.get("shares"))
        except:
            return apology("amount of shares must be postitve and integer", 400)
        if shares < 0 or int(shares) != shares:
            return apology("amount of shares must be postitve and integer", 400)
        stock_price = float(api_values["price"])
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        if stock_price * shares > available_cash:
            return apology("not enough cash", 400)
        else:
            db.execute("INSERT INTO transactions (user_id, timestamp, buy_sell, symbol, shares, price) VALUES (?,?,?,?,?,?)",
                       session.get("user_id"), timestamp, "BUY", symbol, shares, stock_price)
            db.execute("UPDATE users SET cash=(?) WHERE id=(?)", available_cash - stock_price * shares, session.get("user_id"))
            return redirect("/")

    else:
        return render_template("buy.html")
        

@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    if request.method == "POST":
        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        new_confirmation = request.form.get("new_confirmation")

        if not old_password:
            return apology("must provide old password", 400)
        elif not new_password:
            return apology("must provide new password", 400)
        elif not new_confirmation:
            return apology("must provide confirmation", 400)
        elif new_password != new_confirmation:
            return apology("new passwords don't match", 400)

        rows = db.execute("SELECT * FROM users WHERE id = ?", session.get("user_id"))

        # Check if old pw matches database
        if check_password_hash(rows[0]["hash"], old_password):
            hashed_password = generate_password_hash(new_password)
            db.execute("UPDATE users SET hash=(?) WHERE id=(?)", hashed_password, session.get("user_id"))

            hashed_password = None
            return render_template("settings.html", success=1)
        else:
            apology("old password is incorrect", 400)

    else:
        return render_template("settings.html")
        

@app.route("/history")
@login_required
def history():
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = (?)", session.get("user_id"))

    export_shares = []
    for rows in transactions:
        temp = []
        temp.append(rows["symbol"])
        temp.append(rows["buy_sell"])
        temp.append(round(rows["price"], 2))
        temp.append(int(rows["shares"]))
        temp.append(rows["timestamp"])
        export_shares.append(temp)

    return render_template("history.html", export_shares=export_shares)
    

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        api_values = lookup(symbol)
        # Check wether stock symbol exists
        if api_values == None:
            return apology("ticker symbol does not exist", 400)
        else:
            return render_template("quoted.html", api_values=api_values)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        # Ensure username and password were submitted and that confirmation matches password
        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)
        elif db.execute("SELECT COUNT(*) FROM users WHERE username=(?)", request.form.get("username"))[0]['COUNT(*)'] == 1:
            return apology("duplicate username", 400)

        # hash password and add to database
        hashed_password = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), hashed_password)

        hashed_password = None
        return render_template("login.html", registered=1)

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        amount = float(request.form.get("shares"))
        api_values = lookup(symbol)
        # Check wether stock symbol exists
        if api_values == None:
            return render_template("buy.html", nonexistant=1)

        # check if user even owns the stock and if he owns enough to sell
        user_id = session.get("user_id")
        transactions = db.execute("SELECT symbol, buy_sell, shares FROM transactions WHERE user_id = (?)", user_id)

        # dict of shares available and how many
        shares = {}
        for rows in transactions:
            if rows["symbol"] not in shares:
                shares[rows["symbol"]] = 0

            if rows["buy_sell"] == "BUY":
                shares[rows["symbol"]] = shares[rows["symbol"]] + rows["shares"]
            elif rows["buy_sell"] == "SELL":
                shares[rows["symbol"]] = shares[rows["symbol"]] - rows["shares"]

        # dict comprehension: delete all where 0 shares are available
        shares = {x: y for x, y in shares.items() if y != 0}

        if shares[symbol] < amount:
            return apology("you don't own enough of chosen stock", 400)
        else:
            stock_price = float(api_values["price"])
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            available_cash = db.execute("SELECT cash FROM users WHERE id = (?)", session.get("user_id"))
            available_cash = float(available_cash[0]["cash"])
            db.execute("INSERT INTO transactions (user_id, timestamp, buy_sell, symbol, shares, price) VALUES (?,?,?,?,?,?)",
                       session.get("user_id"), timestamp, "SELL", symbol, amount, stock_price)
            db.execute("UPDATE users SET cash=(?) WHERE id=(?)", available_cash + stock_price * amount, session.get("user_id"))
            return redirect("/")

    else:
        user_id = session.get("user_id")
        transactions = db.execute("SELECT symbol, buy_sell, shares FROM transactions WHERE user_id = (?)", user_id)

        # dict of shares available and how many
        shares = {}
        for rows in transactions:
            if rows["symbol"] not in shares:
                shares[rows["symbol"]] = 0

            if rows["buy_sell"] == "BUY":
                shares[rows["symbol"]] = shares[rows["symbol"]] + rows["shares"]
            elif rows["buy_sell"] == "SELL":
                shares[rows["symbol"]] = shares[rows["symbol"]] - rows["shares"]

        stocks = shares.keys()

        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
