import sys
import os
import json
from enum import Enum
from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse
from django.template import RequestContext, loader
from django.core.urlresolvers import reverse
from django.conf import settings
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

def _pingServer():
	tc = ThriftCommunicator()
	result = tc.ping(1, 'x')
	return result == 1

defaultImage = {
    'tileWidth': 2200,
    'tileHeight': 1700,
    'scaledWidth': 2200,
    'scaledHeight': 1700
}

def _getMapImageInfo():
	tc = ThriftCommunicator()
	floorTiles = [
		tc.getMapImageTiles(0),
		tc.getMapImageTiles(1)
	]

	if not floorTiles:
		return {}

	floorTilesInfo = {}

	for i in xrange(0, 2):
		if not floorTiles[i]:
			continue

		if not floorTiles[i].zoomLevels:
			floorTilesInfo[i] = {
				'tileWidth': defaultImage['tileWidth'],
				'tileHeight': defaultImage['tileHeight'],
				'scaledWidth': defaultImage['scaledWidth'],
				'scaledHeight': defaultImage['scaledHeight']
			}
		else:
			floorTilesInfo[i] = {
			idx: {
				'tileWidth': zoom.tileSize.width,
				'tileHeight': zoom.tileSize.height,
				'scaledWidth': zoom.scaledSize.width,
				'scaledHeight': zoom.scaledSize.height
			} for idx, zoom in enumerate(floorTiles[i].zoomLevels)}

	return floorTilesInfo

def _getExhibits():
	tc = ThriftCommunicator()
	result = tc.getExhibits()
	if not result or not result.exhibits:
		return {}

	exhibitDict = {}
	for k in result.exhibits:
		e = result.exhibits[k]
		if e.frame == None:
			exhibitDict[k] = {
				'name': e.name,
				'frame': None
			}
			continue
		exhibitDict[k] = {
			'name': e.name,
			'frame': {
				'x': e.frame.x,
				'y': e.frame.y,
				'width': e.frame.width,
				'height': e.frame.height,
				'mapLevel': e.frame.mapLevel
			},
		}
	return exhibitDict

def index(request):
	if not _pingServer():
		return HttpResponse('<h1>Nie mozna nawiazac polaczenia z serwerem, upewnij sie, ze jest wlaczony</h1>')

	floorTilesInfo = _getMapImageInfo()
	exhibits = _getExhibits()
	template = loader.get_template('index.html')
	context = RequestContext(request, {
		'activeFloor': 0,
		'exhibits': exhibits,
		'floorTilesInfo': floorTilesInfo,
		'urlFloor0': getattr(settings, 'FLOOR0_TILES_DIRECTORY', r"static/floorplan0.jpg"),
		'urlFloor1': getattr(settings, 'FLOOR1_TILES_DIRECTORY', r"static/floorplan0.jpg")
	})
	return HttpResponse(template.render(context))

class uploadError(Enum):
	SUCCESS = 1
	NOT_AN_IMAGE = 2
	SERVER_PROBLEM = 3
	NOT_POST_METHOD = 4

def uploadImage(request):
	if request.method != 'POST':
		data = {
			"err": uploadError.NOT_POST_METHOD.value,
			"floor": 0
		}
		return JsonResponse(data)

	form = MapUploadForm(request.POST, request.FILES)
	if not form.is_valid():
		data = {
			"err": uploadError.NOT_AN_IMAGE.value,
			"floor": 0
		}
		return JsonResponse(data)

	m = MapUploader(image = form.cleaned_data['image'])
	m.save()

	#send information about update
	tc = ThriftCommunicator()
	floor = form.cleaned_data['floor']
	filename = m.image.name
	#extract filename
	set_result = tc.setMapImage(floor, os.path.basename(filename))

	floorTilesInfo = _getMapImageTilesSize()

	if not set_result:
		data = {
			"err": uploadError.SERVER_PROBLEM.value,
			"floor": floor,
		}
		return JsonResponse(data)

	data = {
		"err": uploadError.SUCCESS.value,
		"floor": floor,
        "tileSize": {"width": floorTilesInfo[floor]['tileWidth'], "height": floorTilesInfo[floor]['tileHeight']},
        "scaledSize": {"width": floorTilesInfo[floor]['scaledWidth'], "height": floorTilesInfo[floor]['scaledHeight']}
	}
	return JsonResponse(data)
