import sys
import json
from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from django.template import RequestContext, loader
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

def index(request):
	tc = ThriftCommunicator()
	ret = tc.getMapImages()
	ret_version = ret.version
	ret_urls = ret.levelImageUrls
	url_floor0 = None
	url_floor1 = None

	if 0 in ret_urls.keys():
		url_floor0 = ret_urls[0]
	if 1 in ret_urls.keys():
		url_floor1 = ret_urls[1]

	url_floor0 = url_floor0[url_floor0.find('/'):]
	url_floor1 = url_floor1[url_floor1.find('/'):]
	template = loader.get_template('index.html')
	context = RequestContext(request, {
	"url_floor0" : url_floor0,
	"url_floor1" : url_floor1,
	})
	return HttpResponse(template.render(context))

def uploadImage(request):
	if request.method != 'POST':
		return HttpResponse('prosze nie hakowac strony')
	form = MapUploadForm(request.POST, request.FILES)
	if not form.is_valid():
		return HttpResponse('to nie obrazek')
	m = MapUploader(image = form.cleaned_data['image'])
	m.save()

	#send information about update
	tc = ThriftCommunicator()
	floor = 0
	filename = m.image.name
	ret = tc.setMapImage(floor, filename[2:]) #remove ./ at beggining of filename
	if ret == False:
		return HttpResponse('Blad aktualizacji mapy!') #TODO
	return HttpResponseRedirect('/')
