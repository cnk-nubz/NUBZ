import sys
import os
import json
from enum import Enum
from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse
from django.template import RequestContext, loader
from django.core.urlresolvers import reverse
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

def _getFloorImages():
	tc = ThriftCommunicator()
	result = tc.getMapImages()

	if not result:
		raise Exception()

	result = result.levelImageUrls
	urls = [None, None]

	try:
		urls[0] = result[0][result[0].find('/'):]
	except KeyError:
		pass

	try:
		urls[1] = result[1][result[1].find('/'):]
	except KeyError:
		pass

	return urls

def _getExhibits():
	tc = ThriftCommunicator()
	result = tc.getExhibits()

	if not result:
		raise Exception()

	return result.exhibits

def index(request):
	try:
		urls = _getFloorImages()
	except:
		return HttpResponse('<h1>Nie mozna zaladowac map, sprawdz czy serwer jest wlaczony</h1>')

	try:
		exhibits = _getExhibits()
	except:
		pass

	exhibitList = list()
	for i in exhibits.keys():
		if exhibits[i].frame:
			e = exhibits[i].frame
			exhibitList.append({
				"x": e.x,
				"y": e.y,
				"height": e.height,
				"width": e.width,
				"mapLevel": e.mapLevel,
			})

	template = loader.get_template('index.html')
	context = RequestContext(request, {
		"url_floor0" : urls[0],
		"url_floor1" : urls[1],
		"floor": 0,
		"exhibits": exhibitList
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
	try:
		urls = _getFloorImages()
	except:
		urls = [None, None]
	if not set_result:
		data = {
			"err": uploadError.SERVER_PROBLEM.value,
			"floor": floor,
			"url_floor0": urls[0],
			"url_floor1": urls[1]
		}
		return JsonResponse(data)

	data = {
		"err": uploadError.SUCCESS.value,
		"floor": floor,
		"url_floor0": urls[0],
		"url_floor1": urls[1]
	}
	return JsonResponse(data)
