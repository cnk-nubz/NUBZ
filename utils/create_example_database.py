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
		version INT NOT NULL UNIQUE,
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

######### sample data

# counters
cur.execute('''
	INSERT INTO counters VALUES
		('last_map_version', 0),
		('last_exhibit_version', 5),
		('last_deleted_exhibit_version', -1),
		('last_report_id', 3)
''')

# exhibits
cur.execute('''
	INSERT INTO exhibits (name, version, rgb_hex, map_frame_x, map_frame_y, map_frame_width, map_frame_height, map_floor) VALUES
		('invisible', 3, 6599648, NULL, NULL, NULL, NULL, NULL),
		('exhibit 1 floor 0', 1, 10347354, 100, 100, 200, 200, 0),
		('exhibit 2 floor 0', 2, 16703841, 50, 400, 100, 100, 0),
		('exhibit 3 floor 1', 3, 16695666, 0, 0, 100, 100, 1),
		('exhibit 4 floor 1', 4, 16605278, 150, 150, 150, 150, 1),
		('exhibit 5 floor 1', 5, 10372537, 250, 250, 250, 250, 1)
''')

# experiments
cur.execute('''
	INSERT INTO experiments (name, state, start_date, finish_date, content) VALUES
		('aktywne badanie', 1, '2010-Jan-03', NULL, '
			{
				"actions": [7, 8],
				"breakActions": [12, 9, 11],
				"surveyBefore": {
					"typesOrder": [0, 2, 1, 0, 2],
					"simpleQuestions": [2, 1],
					"multipleChoiceQuestions": [2],
					"sortQuestions": [1, 2]
				},
				"surveyAfter": {
					"typesOrder": [2, 0, 1, 2, 1],
					"simpleQuestions": [1],
					"multipleChoiceQuestions": [1, 2],
					"sortQuestions": [2, 1]
				}
			}
		'),
		('starsze zakonczone badanie', 2, '2002-Jan-03', '2003-Feb-13', '
			{
				"actions": [5, 1, 4, 3, 7, 8],
				"breakActions": [12, 9, 11],
				"surveyBefore": {
					"typesOrder": [0, 2, 1, 0, 2],
					"simpleQuestions": [2, 1],
					"multipleChoiceQuestions": [2],
					"sortQuestions": [1, 2]
				},
				"surveyAfter": {
					"typesOrder": [2, 0, 1, 2, 1],
					"simpleQuestions": [1],
					"multipleChoiceQuestions": [1, 2],
					"sortQuestions": [2, 1]
				}
			}
		'),
		('zakonczone badanie', 2, '2003-Feb-14', '2003-Feb-16', '
			{
				"actions": [5, 1, 4, 3],
				"breakActions": [12, 9, 11],
				"surveyBefore": {
					"typesOrder": [0, 2, 1, 0, 2],
					"simpleQuestions": [2, 1],
					"multipleChoiceQuestions": [2],
					"sortQuestions": [1, 2]
				},
				"surveyAfter": {
					"typesOrder": [2, 0, 1, 2, 1],
					"simpleQuestions": [1],
					"multipleChoiceQuestions": [1, 2],
					"sortQuestions": [2, 1]
				}
			}
		'),
		('gotowe badanie', 0, NULL, NULL, '
			{
				"actions": [3, 7, 8],
				"breakActions": [12, 9, 11],
				"surveyBefore": {
					"typesOrder": [0, 2, 1, 0, 2],
					"simpleQuestions": [2, 1],
					"multipleChoiceQuestions": [2],
					"sortQuestions": [1, 2]
				},
				"surveyAfter": {
					"typesOrder": [2, 0, 1, 2, 1],
					"simpleQuestions": [1],
					"multipleChoiceQuestions": [1, 2],
					"sortQuestions": [2, 1]
				}
			}
		'),
		('inne gotowe badanie', 0, NULL, NULL, '
			{
				"actions": [5, 1, 7, 8],
				"breakActions": [12, 9, 11],
				"surveyBefore": {
					"typesOrder": [0, 2, 1, 0, 2],
					"simpleQuestions": [2, 1],
					"multipleChoiceQuestions": [2],
					"sortQuestions": [1, 2]
				},
				"surveyAfter": {
					"typesOrder": [2, 0, 1, 2, 1],
					"simpleQuestions": [1],
					"multipleChoiceQuestions": [1, 2],
					"sortQuestions": [2, 1]
				}
			}
		')
''')

# reports
cur.execute('''
	INSERT INTO reports VALUES
		(1, '2011-Sep-27', 1, '
			{
				"beginTime": {
					"h": 8,
					"m": 0,
					"s": 1
				},
				"finishTime": {
					"h": 16,
					"m": 55,
					"s": 55
				},
				"history": [
					{
						"exhibitId": 2,
						"beginTime": {
							"h": 10,
							"m": 11,
							"s": 12
						},
						"secs": 15,
						"actions": [1, 4, 7]
					}, {
						"beginTime": {
							"h": 10,
							"m": 11,
							"s": 50
						},
						"exhibitId": 1,
						"secs": 30,
						"actions": [1]
					}, {
						"exhibitID": 2,
						"beginTime": {
							"h": 10,
							"m": 13,
							"s": 12
						},
						"secs": 140,
						"actions": []
					}, {
						"beginTime": {
							"h": 11,
							"m": 11,
							"s": 12
						},
						"secs": 17,
						"actions": [12, 9]
					}, {
						"exhibitId": 1,
						"beginTime": {
							"h": 12,
							"m": 11,
							"s": 12
						},
						"secs": 20,
						"actions": [1, 3]
					}, {
						"beginTime": {
							"h": 13,
							"m": 14,
							"s": 15
						},
						"secs": 14,
						"actions": [11]
					}
				],
				"surveyBefore": {
					"simple": [
						{
							"ans": "12345"
						}, {
							"ans": "brak odpowiedzi"
						}
					],
					"multi": [
						{
							"ans": [5, 6, 9, 12]
						}
					],
					"sort": [
						{
							"ans": [3, 2, 4, 1]
						}, {
							"ans": [7, 6, 5, 12, 8, 9, 10, 11]
						}
					]
				},
				"surveyAfter": {
					"simple": [
						{
							"ans": "smieciowe znaki ; , ."
						}
					],
					"multi": [
						{
							"ans": [2]
						}, {
							"ans": [6, 5, 9, 12]
						}
					],
					"sort": [
						{
							"ans": [5, 6, 8, 12, 7, 9, 10, 11]
						}, {
							"ans": [3, 1, 4, 2]
						}
					]
				}
			}
		'),
	(2, '2011-Sep-28', 1, '
			{
				"history": [
					{
						"exhibitId": 2,
						"beginTime": {
							"h": 10,
							"m": 11,
							"s": 12
						},
						"secs": 15,
						"actions": [1, 4, 7]
					}, {
						"exhibitId": 1,
						"beginTime": {
							"h": 10,
							"m": 11,
							"s": 50
						},
						"secs": 30,
						"actions": [4]
					}
				],
				"beginTime": {
					"h": 9,
					"m": 10,
					"s": 11
				},
				"finishTime": {
					"h": 12,
					"m": 13,
					"s": 14
				},
				"surveyBefore": {
					"simple": [
						{
							"ans": "12345"
						}, {
							"ans": "brak odpowiedzi"
						}
					],
					"multi": [
						{
							"ans": [5, 6, 9, 12]
						}
					],
					"sort": [
						{
							"ans": [3, 2, 4, 1]
						}, {
							"ans": [7, 6, 5, 12, 8, 9, 10, 11]
						}
					]
				},
				"surveyAfter": {
					"simple": [
						{
							"ans": "test answer for simple question 1"
						}
					],
					"multi": [
						{
							"ans": [2]
						}, {
							"ans": [6, 5, 9, 12]
						}
					],
					"sort": [
						{
							"ans": [5, 6, 7, 12, 8, 9, 10, 11]
						}, {
							"ans": [3, 1, 4, 2]
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

# sort questions
cur.execute('''
	INSERT INTO sort_questions (name, question) VALUES
		('sort question 1', 'best language ??'),
		('aa sort question 2', 'sort question ??')
''')

cur.execute('''
	INSERT INTO sort_question_options (question_id, text) VALUES
		(1, 'javascript'),
		(1, 'python'),
		(1, 'bash'),
		(1, 'perl'),
		(2, 'aaa'),
		(2, 'bbb'),
		(2, 'ccc'),
		(2, 'ddd'),
		(2, 'eee'),
		(2, 'fff'),
		(2, 'ggg'),
		(2, 'hhh')
''')

con.commit()

print "Done"
