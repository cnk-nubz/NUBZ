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
		map_level INT NULL
	)
''')

cur.execute('''
	CREATE TABLE reports (
		id INT NOT NULL,
		doc JSONB NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE actions (
		id SERIAL PRIMARY KEY,
		text VARCHAR NOT NULL
	)
''')

cur.execute('''
	CREATE TABLE experiments (
		id INT NOT NULL PRIMARY KEY,
		name VARCHAR NOT NULL,
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
	INSERT INTO exhibits (name, version, map_frame_x, map_frame_y, map_frame_width, map_frame_height, map_level) VALUES
		('invisible', 3, NULL, NULL, NULL, NULL, NULL),
		('exhibit 1 floor 0', 1, 100, 100, 200, 200, 0),
		('exhibit 2 floor 0', 2, 50, 400, 100, 100, 0),
		('exhibit 3 floor 1', 3, 0, 0, 100, 100, 1),
		('exhibit 4 floor 1', 4, 150, 150, 150, 150, 1),
		('exhibit 5 floor 1', 5, 250, 250, 250, 250, 1)
''')

# reports
cur.execute('''
	INSERT INTO reports VALUES
		(1, '
			{
				"history": [{
						"exhibitId": 2,
						"durationInSecs": 15,
						"actions": [1, 2, 4]
					}, {
						"exhibitId": 0,
						"durationInSecs": 30,
						"actions": [1]
					}, {
						"durationInSecs": 140,
						"actions": [5]
					}
				]
			}
		'),
		(3, '
			{
				"history": [{
						"durationInSecs": 17,
						"actions": []
					}, {
						"exhibitId": 1,
						"durationInSecs": 20,
						"actions": [1, 2]
					}, {
						"durationInSecs": 14,
						"actions": [5]
					}
				]
			}
		')
''')

# actions
cur.execute('''
	INSERT INTO actions VALUES
		(1, 'biega'),
		(2, 'spiewa'),
		(3, 'recytuje'),
		(4, 'stepuje'),
		(5, 'oglada'),
		(6, 'bawi sie'),
		(7, 'robi jakas bardzo skomplikowana czynnosc opisana wieloma slowami'),
		(8, 'cos poza badaniem'),
		(9, 'biega'),
		(10, 'spiewa'),
		(11, 'dlugi dlugi dlugi dlugi tekst'),
		(12, 'odpoczywa')
''')

# experiments
cur.execute('''
	INSERT INTO experiments VALUES
		(1, 'badanie testowe', '
			{
				"actions": [5, 1, 4, 3, 7, 8],
				"breakActions": [12, 9, 11]
			}
		')
''')

# current experiment
cur.execute('''
	UPDATE active_experiment
	SET id=1
''')

con.commit()

print "Done"