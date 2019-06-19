import sqlite3

conn = sqlite3.connect('trec2017.db')

with conn:
    cur = conn.cursor()
    cur.execute("drop table inexclude")
    cur.execute(
        "CREATE TABLE inexclude(id integer, inclusion text, exclusion text, hasclearinexclude integer, FOREIGN KEY(id) REFERENCES clinicaltrials(id))")
