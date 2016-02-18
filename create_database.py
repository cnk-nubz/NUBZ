#!/usr/bin/python

import psycopg2

con = psycopg2.connect(database='nubz_db', user='zpp') 
cur = con.cursor()

######### drop
cur.execute('DROP TABLE IF EXISTS map_images')
cur.execute('DROP TABLE IF EXISTS map_tiles')
cur.execute('DROP TABLE IF EXISTS map_tiles_info')
cur.execute('DROP TABLE IF EXISTS counters')
cur.execute('DROP TABLE IF EXISTS exhibits')
cur.execute('DROP TABLE IF EXISTS reports')
cur.execute('DROP TABLE IF EXISTS actions')
cur.execute('DROP TABLE IF EXISTS active_experiment')
cur.execute('DROP TABLE IF EXISTS experiments')
cur.execute('DROP TABLE IF EXISTS simple_questions')
cur.execute('DROP TABLE IF EXISTS multiple_choice_question_options')
cur.execute('DROP TABLE IF EXISTS multiple_choice_questions')

######### create
cur.execute('''
	CREATE TABLE map_images (
		filename VARCHAR NOT NULL,
		width INT NOT NULL,
		height INT NOT NULL,
		floor INT NOT NULL UNIQUE,
		version INT NOT NULL UNIQUE
	)
''')

cur.execute('''
	CREATE TABLE map_tiles (
		floor INT NOT NULL,
		zoom_level INT NOT NULL,
		row INT NOT NULL,
		col INT NOT NULL,
		filename VARCHAR NOT NULL,
		UNIQUE(floor, zoom_level, row, col)
	)
''')

cur.execute('''
	CREATE TABLE map_tiles_info (
		floor INT NOT NULL,
		zoom_level INT NOT NULL,
		rows_count INT NOT NULL,
		columns_count INT NOT NULL,
		img_width INT NOT NULL,
		img_height INT NOT NULL,
		tile_size INT NOT NULL,
		UNIQUE(floor, zoom_level)
	)
''')

cur.execute('''
	CREATE TABLE counters (
		element VARCHAR NOT NULL UNIQUE,
		counter INT NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE exhibits (
		id SERIAL PRIMARY KEY,
		name VARCHAR NOT NULL,
		version INT NOT NULL,

		map_frame_x INT NULL,
		map_frame_y INT NULL,
		map_frame_width INT NULL,
		map_frame_height INT NULL,
		map_floor INT NULL
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
	CREATE TABLE experiments (
		id SERIAL PRIMARY KEY,
		name VARCHAR NOT NULL,
		doc JSONB NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE reports (
		id INT NOT NULL,
		experiment_id INT NOT NULL REFERENCES experiments(id),
		doc JSONB NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE active_experiment (
		id INT NULL REFERENCES experiments(id)
	);

	INSERT INTO active_experiment VALUES
		(NULL);

	CREATE RULE no_insert AS ON INSERT TO active_experiment DO INSTEAD NOTHING; 
	CREATE RULE no_delete AS ON DELETE TO active_experiment DO INSTEAD NOTHING;
''')

######### sample data
# map_images
cur.execute('''
	INSERT INTO map_images VALUES
		('floorplan0.jpg', 2200, 1700, 0, 1),
		('floorplan1.jpg', 2200, 1700, 1, 2)
''')

# counters
cur.execute('''
	INSERT INTO counters VALUES
		('map_images_version', 2),
		('exhibits_version', 5),
		('reports_last_id', 3)
''')

# exhibits
cur.execute('''
	INSERT INTO exhibits (name, version, map_frame_x, map_frame_y, map_frame_width, map_frame_height, map_floor) VALUES
		('invisible', 3, NULL, NULL, NULL, NULL, NULL),
		('exhibit 1 floor 0', 1, 100, 100, 200, 200, 0),
		('exhibit 2 floor 0', 2, 50, 400, 100, 100, 0),
		('exhibit 3 floor 1', 3, 0, 0, 100, 100, 1),
		('exhibit 4 floor 1', 4, 150, 150, 150, 150, 1),
		('exhibit 5 floor 1', 5, 250, 250, 250, 250, 1)
''')

# experiments
cur.execute('''
	INSERT INTO experiments (name, doc) VALUES
		('badanie testowe', '
			{
				"actions": [5, 1, 4, 3, 7, 8],
				"breakActions": [12, 9, 11],
				"surveyBefore": {
					"typesOrder": [0, 1, 0],
					"simpleQuestions": [2, 1],
					"multipleChoiceQuestions": [2]
				},
				"surveyAfter": {
					"typesOrder": [0, 1, 1],
					"simpleQuestions": [1],
					"multipleChoiceQuestions": [1, 2]
				}
			}
		')
''')

# reports
cur.execute('''
	INSERT INTO reports VALUES
		(1, 1, '
			{
				"history": [
					{
						"exhibitId": 2,
						"durationInSecs": 15,
						"actions": [1, 4, 7]
					}, {
						"exhibitId": 1,
						"durationInSecs": 30,
						"actions": [1]
					}, {
						"exhibitID": 2,
						"durationInSecs": 140,
						"actions": []
					}, {
						"durationInSecs": 17,
						"actions": [12, 9]
					}, {
						"exhibitId": 1,
						"durationInSecs": 20,
						"actions": [1, 3]
					}, {
						"durationInSecs": 14,
						"actions": [11]
					}
				],
				"surveyBefore": {
					"simpleQuestions": [
						{
							"answer": "12345"
						}, {
						}
					],
					"multipleChoiceQuestions": [
						{
							"answer": [5, 6, 9, 12]
						}
					]
				},
				"surveyAfter": {
					"simpleQuestions": [
						{
							"answer": "test answer for simple question 1"
						}
					],
					"multipleChoiceQuestions": [
						{
							"answer": [2]
						}, {
						}
					]
				}
			}
		')
''')

# actions
cur.execute('''
	INSERT INTO actions (text) VALUES
		('biega'),
		('spiewa'),
		('recytuje'),
		('stepuje'),
		('oglada'),
		('bawi sie'),
		('robi jakas bardzo skomplikowana czynnosc opisana wieloma slowami'),
		('cos poza badaniem'),
		('biega'),
		('spiewa2'),
		('dlugi dlugi dlugi dlugi tekst'),
		('odpoczywa')
''')

# simple questions
cur.execute('''
	INSERT INTO simple_questions (name, question, number_answer) VALUES
		('simple question 1 - str', 'test test test string answer', FALSE),
		('simple question 2 - num', 'test test test num answer', TRUE)
''')

# multiple choice questions
cur.execute('''
	INSERT INTO multiple_choice_questions (name, question, single_answer) VALUES
		('multiple choice question 1 - single', 'multiple choice question 1 - single ??', TRUE),
		('multiple choice question 2 - multi', 'multiple choice question 2 - multi ??', FALSE)
''')

cur.execute('''
	INSERT INTO multiple_choice_question_options (question_id, text) VALUES
		(1, 'a'),
		(1, 'b'),
		(1, 'c'),
		(1, 'd'),
		(2, 'aa'),
		(2, 'bb'),
		(2, 'cc'),
		(2, 'dd'),
		(2, 'ee'),
		(2, 'ff'),
		(2, 'gg'),
		(2, 'hh')
''')

# current experiment
cur.execute('''
	UPDATE active_experiment
	SET id=1
''')

con.commit()

print "Done"