# NUBZ - a tool facilitating surveying of visitors

This project was developed to help the Copernicus Science Centre in Warsaw, Poland conduct visitor surveys and collect their results.

## Simplified workflow
1. Set up the survey using the web interface, entering:
  - the map of the museum
  - locations and descriptions of exhibits
  - which visitor's interactions with exhibits should be tracked
  - questions to be included in the pre- and post-visit questionnaires
2. Conduct the survey using the Android app:
  - follow the visitor and record their interactions with exhibits
  - administer the relevant questionnaires
3. Download the results from the web interface:
  - generate a report for one or more surveys
  - export the results

## Technical architecture
The project, which we used as a great opportunity to learn new skills, techniques and technologies, consists of three parts:
 - a backend server
 - a web interface
 - an Android application

which transparently synchronise the data (using [Apache Thrift](https://thrift.apache.org) as the underlying RPC library), obviating the need for manual user interventions.

### Backend server
All the business logic is handled by the backend server, which collects, manages and serves the data. It:
 - receives the survey results
 - prepares the museum map by pre-rendering its tiles at a couple of zoom levels
 - stores the data in a [PostgreSQL](https://www.postgresql.org/) database

Because no existing C++ PostgreSQL library suited all of our needs, we implemented a small wrapper around [pqxx](http://pqxx.org) supporting JSON values and checking queries for type errors and column/table mismatches at compile-time.

### Web interface
On the server-side, the web interface is a thin [Django](https://www.djangoproject.com) web app which acts as a proxy between the backend server and a rich web client.
The client uses [jQuery](https://jquery.com) and [Bootstrap](http://getbootstrap.com) extensively, and the museum map is displayed using [Leaflet](http://leafletjs.com). 

### Android app
The Android app regularly synchronises the data with the backend server and stores it locally in a [Realm](https://realm.io) database. Hence, it remains useable and guarantees data persistence even if network connectivity is lost.

Because the museum map is large, we implemented a solution similar to Google Maps': the Android app loads only the currently visible part of the map.
