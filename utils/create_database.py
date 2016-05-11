#!/usr/bin/python

import psycopg2

con = psycopg2.connect(database='nubz_db', user='zpp') 
cur = con.cursor()

######### create

cur.execute('''
	CREATE TABLE map_images (
		floor INT PRIMARY KEY,
		filename VARCHAR NOT NULL,
		width INT NOT NULL,
		height INT NOT NULL,
		zoom_levels JSONB NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE counters (
		name VARCHAR NOT NULL UNIQUE,
		counter INT NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE exhibits (
		id SERIAL PRIMARY KEY,
		name VARCHAR NOT NULL,
		version INT NOT NULL,
		rgb_hex INT NOT NULL,

		map_frame_x INT NULL,
		map_frame_y INT NULL,
		map_frame_width INT NULL,
		map_frame_height INT NULL,
		map_floor INT NULL,

		is_deleted BOOLEAN DEFAULT FALSE NOT NULL,
		ref_count INT DEFAULT 0 NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE actions (
		id SERIAL PRIMARY KEY,
		text VARCHAR NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE simple_questions (
		id SERIAL PRIMARY KEY,
		name VARCHAR NOT NULL,
		question VARCHAR NOT NULL,
		number_answer BOOLEAN NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE multiple_choice_questions (
		id SERIAL PRIMARY KEY,
		name VARCHAR NOT NULL,
		question VARCHAR NOT NULL,
		single_answer BOOLEAN NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE multiple_choice_question_options (
		id SERIAL PRIMARY KEY,
		question_id INT NOT NULL REFERENCES multiple_choice_questions(id) ON DELETE CASCADE,
		text VARCHAR NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE sort_questions (
		id SERIAL PRIMARY KEY,
		name VARCHAR NOT NULL,
		question VARCHAR NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE sort_question_options (
		id SERIAL PRIMARY KEY,
		question_id INT NOT NULL REFERENCES sort_questions(id) ON DELETE CASCADE,
		text VARCHAR NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE experiments (
		id SERIAL PRIMARY KEY,
		name VARCHAR NOT NULL,
		state INTEGER NOT NULL,
		start_date VARCHAR NULL,
		finish_date VARCHAR NULL,
		content JSONB NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE reports (
		id INT NOT NULL,
		receive_date VARCHAR NULL,
		experiment_id INT NOT NULL REFERENCES experiments(id),
		content JSONB NOT NULL
	)
''')

######### initial data

# counters
cur.execute('''
	INSERT INTO counters VALUES
		('last_exhibit_version', 0),
		('last_deleted_exhibit_version', 0),
		('last_report_id', 0)
''')

con.commit()
