import sys
import os
from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse
from django.template import RequestContext, loader
from django.core.urlresolvers import reverse
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

#uploading error:
# 1 - sucessful upload
# 2 - sent file is not a image
# 3 - problem with server
# 4 - not POST method

def _getFloorImages():
	tc = ThriftCommunicator()
	result = tc.getMapImages().levelImageUrls
	urls = [None, None]
	if not result:
		raise Exception()

	try:
		urls[0] = result[0][result[0].find('/'):]
	except KeyError:
		pass

	try:
		urls[1] = result[1][result[1].find('/'):]
	except KeyError:
		pass

	return urls

def index(request):
	err = 0
	floor = 0
	if 'err' in request.GET.keys():
		err = request.GET['err']
	if 'floor' in request.GET.keys():
		floor = request.GET['floor']

	try:
		urls = _getFloorImages()
	except:
		return HttpResponse('<h1>Nie mozna zaladowac map, sprawdz czy serwer jest wlaczony</h1>')

	template = loader.get_template('index.html')
	context = RequestContext(request, {
	"url_floor0" : urls[0],
	"url_floor1" : urls[1],
	"err": err,
	"floor": floor
	})
	if err != 0:
		data = {"a": "witam", "b": "swiat"}
		return JsonResponse(data)
	return HttpResponse(template.render(context))

def uploadImage(request):
	if request.method != 'POST':
		data = {
			"err": 4,
			"floor": 0
		}
		return JsonResponse(data)

	form = MapUploadForm(request.POST, request.FILES)
	if not form.is_valid():
		data = {
			"err": 2,
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
			"err": 3,
			"floor": floor,
			"url_floor0": urls[0],
			"url_floor1": urls[1]
		}
		return JsonResponse(data)

	data = {
		"err": 1,
		"floor": floor,
		"url_floor0": urls[0],
		"url_floor1": urls[1]
	}
	return JsonResponse(data)
