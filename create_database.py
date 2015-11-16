#!/usr/bin/python

import psycopg2

con = psycopg2.connect(database='nubz_db', user='zpp') 
cur = con.cursor()

# drop
cur.execute('DROP TABLE IF EXISTS map_images')
cur.execute('DROP TABLE IF EXISTS versions')

# create
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

# sample data
cur.execute('''
	INSERT INTO map_images VALUES
		('test0.png', 1, 0),
		('test1.png', 2, 1)
''')
cur.execute('''
	INSERT INTO versions VALUES
		('map_images', 2)
''')

con.commit()

print "Done"