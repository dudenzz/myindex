import sqlite3

conn = sqlite3.connect('trec2017.db')

with conn:
    cur = conn.cursor()
    #cur.execute("drop table clinicaltrials")
    cur.execute(
        "CREATE TABLE clinicaltrials(id integer primary key, nct_id text, brief_title text, official_title text, brief_summary text, detailed_description text,"
        "overall_status text, start_date text, primary_completion_date text, completion_date text, phase text, study_type text, study_design_info text, primary_outcome text, secondary_outcome text,"
        "condition text, arm_group text, intervention text, criteria text, gender text, minimum_age integer, maximum_age integer, healthy_volunteers text, condition_browse text, intervention_browse text,"
        "keyword text)")
