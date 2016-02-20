import sys
import os
import json
from enum import Enum
from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse
from django.template import RequestContext, loader
from django.template.loader import render_to_string
from django.core.urlresolvers import reverse
from django.conf import settings
from django.views.decorators.csrf import ensure_csrf_cookie
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

def get_const(name):
    return getattr(settings, name, None)

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
		return None

	floorTilesInfo = {}

	for i in xrange(0, 2):
		if not floorTiles[i]:
			continue

		if not floorTiles[i].zoomLevels:
			floorTilesInfo[i] = {
			0: {
				'tileWidth': defaultImage['tileWidth'],
				'tileHeight': defaultImage['tileHeight'],
				'scaledWidth': defaultImage['scaledWidth'],
				'scaledHeight': defaultImage['scaledHeight']
			}}
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
	if not result:
		print >>sys.stderr, "kurde xxxxxxxxxxxxxxxxx2"
		return None

	exhibitDict = {}
	for k in result.exhibits:
		e = result.exhibits[k]
		frame = None
		if e.mapFrame != None:
			frame = {
				'x': e.mapFrame.frame.x,
				'y': e.mapFrame.frame.y,
				'width': e.mapFrame.frame.size.width,
				'height': e.mapFrame.frame.size.height,
				'mapLevel': e.mapFrame.floor
			}
		exhibitDict[k] = {
			'name': e.name,
			'id': k,
			'frame': frame
		}
	return exhibitDict

def getMapPage(request, file, activeLink):
	if not _pingServer():
		return HttpResponse('<h1>Nie mozna nawiazac polaczenia z serwerem, upewnij sie, ze jest wlaczony</h1>')

	floorTilesInfo = _getMapImageInfo()
	exhibits = _getExhibits()
	if floorTilesInfo is None or exhibits is None:
		return HttpResponse('<h1>Nie mozna pobrac informacji o eksponatach, sprawdz czy baza danych jest wlaczona</h1>')
	template = loader.get_template(file)

	if len(floorTilesInfo[0]) == 1: #just default image
		urlFloor0 = r"/static/floorplan0.jpg"
	else: #get from config file
		urlFloor0 = getattr(settings, 'FLOOR0_TILES_DIRECTORY', r"/static/floorplan0.jpg")

	if len(floorTilesInfo[1]) == 1:
		urlFloor1 = r"/static/floorplan1.jpg"
	else:
		urlFloor1 = getattr(settings, 'FLOOR1_TILES_DIRECTORY', r"/static/floorplan1.jpg")

	context = RequestContext(request, {
		'activeFloor': 0,
		'exhibits': exhibits,
		'floorTilesInfo': floorTilesInfo,
		'urlFloor0': urlFloor0,
		'urlFloor1': urlFloor1,
        'activeLink': activeLink
	})
	return HttpResponse(template.render(context))

def index(request):
	return getMapPage(request, 'map/justMap.html', "0")

@ensure_csrf_cookie
def editMapPage(request):
    return getMapPage(request, 'map/editMap.html', "1")

class uploadError(Enum):
	SUCCESS = 1
	NOT_AN_IMAGE = 2
	SERVER_PROBLEM = 3
	WRONG_REQUEST_METHOD = 4
	INVALID_DATA = 5

def uploadImage(request):
	if request.method != 'POST':
		data = {
			"err": uploadError.WRONG_REQUEST_METHOD.value,
		}
		return JsonResponse(data)

	form = MapUploadForm(request.POST, request.FILES)
	if not form.is_valid():
		data = {
			"err": uploadError.NOT_AN_IMAGE.value,
		}
		return JsonResponse(data)

	m = MapUploader(image = form.cleaned_data['image'])
	m.save()
	floor = form.cleaned_data['floor']
	#send information about update
	tc = ThriftCommunicator()
	filename = m.image.name
	#extract filename
	setResult = tc.setMapImage(floor, os.path.basename(filename))
	floorTilesInfo = _getMapImageInfo()
	if not setResult or not floorTilesInfo:
		data = {
			"err": uploadError.SERVER_PROBLEM.value,
		}
		return JsonResponse(data)

	if len(floorTilesInfo[floor]) == 1: #just default image
		floorUrl = r"/static/floorplan0.jpg"
	else: #get from config file
		if floor == 0:
			floorUrl = getattr(settings, 'FLOOR0_TILES_DIRECTORY', r"/static/floorplan0.jpg")
		else:
			floorUrl = getattr(settings, 'FLOOR1_TILES_DIRECTORY', r"/static/floorplan1.jpg")
	data = {
		"err": uploadError.SUCCESS.value,
		"floor": floor,
		"floorTilesInfo": floorTilesInfo,
		"floorUrl": floorUrl
	}
	return JsonResponse(data)

def updateExhibitPosition(request):
	data = {
		"success": False
	}
	if request.method != 'POST':
		return JsonResponse(data)
	jsonData = request.POST.get("jsonData")
	frame = json.loads(jsonData)
	tc = ThriftCommunicator()
	setPosition = tc.setExhibitFrame(frame)
	if not setPosition:
		return JsonResponse(data)
	data = {
		"success": True,
		"id": int(frame['id']),
		"x": frame['x'],
		"y": frame['y'],
        "width": frame['width'],
        "height": frame['height']
	}
	return JsonResponse(data)

def createNewExhibit(request):
	data = {
		"success": False
	}
	if request.method != 'POST':
		return JsonResponse(data)
	jsonData = request.POST.get("jsonData")
	exhibitRequest = json.loads(jsonData)
	tc = ThriftCommunicator()
	newExhibit = tc.createNewExhibit(exhibitRequest)
	if not newExhibit:
		return JsonResponse(data)

	if newExhibit.mapFrame:
		frame = newExhibit.mapFrame
		exhibitFrame = {
			"x": frame.frame.x,
			"y": frame.frame.y,
			"width": frame.frame.size.width,
			"height": frame.frame.size.height,
			"mapLevel": frame.floor
		}
	else:
		exhibitFrame = None
	data = {
		"success": True,
		"id": int(newExhibit.exhibitId),
		"name": newExhibit.name,
		"frame": exhibitFrame
	}
	return JsonResponse(data)

def surveys(request):
	template = loader.get_template('surveys.html')
	return HttpResponse(template.render(RequestContext(request, {'activeLink' : "2"})))

def getDialog(request, dialog):
	contextDict = {
		'data': dialog['data']
	}
	html = render_to_string('dialog/dialog.html', contextDict)
	retDict = {
		'data': dialog,
		'html': html.replace("\n", "")
	}
	return JsonResponse(retDict)

def getSimpleQuestionDialog(request):
	return getDialog(request, get_const("SIMPLE_QUESTION_DIALOG"))

def getMultipleChoiceQuestionDialog(request):
    return getDialog(request, get_const("MULTIPLE_CHOICE_QUESTION_DIALOG"))

def getExhibitPanel(request):
    html = render_to_string('exhibitPanel/exhibitPanel.html')
    return HttpResponse(html)

def getExhibitListElement(request):
    html = render_to_string('exhibitPanel/exhibitListElement.html')
    return HttpResponse(html)

def getSortQuestionDialog(request):
    return getDialog(request, get_const("SORT_QUESTION_DIALOG"))

def getNewActionDialog(request):
    return getDialog(request, get_const("NEW_ACTION_DIALOG"))

def getChangeMapDialog(request):
    floor = request.POST.get("floor")
    html = render_to_string('dialog/changeMap.html', {'floor': floor})
    return JsonResponse({'html': html.replace("\n", "")})
