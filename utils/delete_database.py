#!/usr/bin/python

import psycopg2

con = psycopg2.connect(database='nubz_db', user='zpp') 
cur = con.cursor()

cur.execute('DROP TABLE IF EXISTS map_images CASCADE')
cur.execute('DROP TABLE IF EXISTS counters CASCADE')
cur.execute('DROP TABLE IF EXISTS exhibits CASCADE')
cur.execute('DROP TABLE IF EXISTS reports CASCADE')
cur.execute('DROP TABLE IF EXISTS actions CASCADE')
cur.execute('DROP TABLE IF EXISTS experiments CASCADE')
cur.execute('DROP TABLE IF EXISTS simple_questions CASCADE')
cur.execute('DROP TABLE IF EXISTS multiple_choice_question_options CASCADE')
cur.execute('DROP TABLE IF EXISTS multiple_choice_questions CASCADE')
cur.execute('DROP TABLE IF EXISTS sort_question_options CASCADE')
cur.execute('DROP TABLE IF EXISTS sort_questions CASCADE')

con.commit()
