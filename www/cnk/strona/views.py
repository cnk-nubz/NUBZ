import sys
import os
import json
from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from django.template import RequestContext, loader
from django.core.urlresolvers import reverse
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

#uploading error:
# 0 - no error
# 1 - sucessful upload
# 2 - sent file is not a image
# 3 - problem with server
# 4 - not POST method
def index(request):
	err = 0
	floor = 0
	if 'err' in request.GET.keys():
		err = request.GET['err']
	if 'floor' in request.GET.keys():
		floor = request.GET['floor']

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
	"err": err,
	"floor": floor
	})
	return HttpResponse(template.render(context))

def uploadImage(request):
	if request.method != 'POST':
		url = "{}?err=4&floor=0".format(reverse('index'))
		return HttpResponseRedirect(url)

	form = MapUploadForm(request.POST, request.FILES)
	if not form.is_valid():
		url = "{}?err=2&floor=0".format(reverse('index'))
		return HttpResponseRedirect(url)

	m = MapUploader(image = form.cleaned_data['image'])
	m.save()

	#send information about update
	tc = ThriftCommunicator()
	floor = form.cleaned_data['floor']
	filename = m.image.name
	#extract filename
	ret = tc.setMapImage(floor, os.path.basename(filename))
	if ret == False:
		url = "{}?err=3&floor={}".format(reverse('index'), floor)
		return HttpResponseRedirect(url)

	url = "{}?err=1&floor={}".format(reverse('index'), floor)
	return HttpResponseRedirect(url)
