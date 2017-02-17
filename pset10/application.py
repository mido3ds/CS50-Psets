import os, subprocess, re
from flask import Flask, jsonify, render_template, request, url_for, g
from flask_jsglue import JSGlue
from random import randrange
from urllib.request import urlopen
from bs4 import BeautifulSoup

from cs50 import SQL

# configure application
app = Flask(__name__)
JSGlue(app)
app.config.update(
    # HOST='192.168.1.4',
    HOST='0.0.0.0',
    PORT=randrange(5000, 9000),
    DEBUG=False,
    SECRET_KEY=os.urandom(40),
    API_KEY='AIzaSyCYAYgBPRn2rDKuiJ9ylTmzX1LiML_mSIk',
)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles")
def articles():
    """Look up articles for geo."""

    # user didn't send geo
    if 'geo' not in request.args:
        return jsonify([])

    # send request to google
    google_news = u'https://news.google.com/news/feeds?geo={}&output=rss'.format(request.args['geo'])
    document = urlopen(google_news).read().decode('utf-8')
    if document is None:
        return jsonify([])
    document = BeautifulSoup(document, 'html.parser')

    # parse rss
    items = document.rss.channel.find_all('item')
    response = []
    for item in items:
        response.append({
            'link': item.link.text,
            'title': item.title.text,
        })

    return jsonify(response)

@app.route("/search")
def search():
    """Search for places that match query."""
    if 'q' not in request.args or request.args['q'] == '':
        return jsonify({'error':'bad request'})

    queries = request.args['q'].split(', ')
    results = _search(queries)

    return jsonify(results)

def _search(queries):
    '''return list of dicts of requested query, 
    see http://pset8.cs50.net/search?q=02138 .
    
    q is one query element as a str 
    '''
    result = []

    for q in queries:
        q += '%'

        if q[:-1].isalpha():
            # city/state
            result.extend(
                db.execute(
                    'SELECT * FROM places WHERE \
                    country_code LIKE :q OR place_name LIKE :q OR admin_name2 LIKE :q',
                    q=q,
                )
            )

        elif q[:-1].isdigit():
            # post 
            result.extend(
                db.execute(
                    'SELECT * FROM places WHERE postal_code LIKE :q',
                    q=q,
                )
            ) 

    return result



@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)

if __name__=='__main__':
    subprocess.call([
        'open',
        'http://{}:{}'.format(app.config['HOST'], app.config['PORT'])
    ])
    app.run(
        port=app.config['PORT'],
        host=app.config['HOST'],
        debug=True,
    )