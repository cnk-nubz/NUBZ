#!/usr/bin/python

import psycopg2

con = psycopg2.connect(database='nubz_db', user='zpp') 
cur = con.cursor()

######### drop
cur.execute('DROP TABLE IF EXISTS map_images')
cur.execute('DROP TABLE IF EXISTS versions')
cur.execute('DROP TABLE IF EXISTS exhibits')

######### create
cur.execute('''
	CREATE TABLE map_images (
		filename VARCHAR NOT NULL,
		version INT NOT NULL UNIQUE,
		level INT NOT NULL UNIQUE
	)
''')
cur.execute('''
	CREATE TABLE versions (
		element VARCHAR NOT NULL UNIQUE,
		version INT NOT NULL
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

######### sample data
# map_images
cur.execute('''
	INSERT INTO map_images VALUES
		('floorplan0.jpg', 1, 0),
		('floorplan0.jpg', 2, 1)
''')

# versions
cur.execute('''
	INSERT INTO versions VALUES
		('map_images', 2),
		('exhibits', 5)
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

con.commit()

print "Done"