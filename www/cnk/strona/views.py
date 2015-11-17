import sys
import json
from django.shortcuts import render
from django.http import HttpResponse
from django.template import RequestContext, loader
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
