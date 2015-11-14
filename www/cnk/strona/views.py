import sys
from django.shortcuts import render
from django.http import HttpResponse
from django.template import RequestContext, loader
from ThriftCommunicator import ThriftCommunicator

def index(request):
	tc = ThriftCommunicator()
	ret = tc.getMapImages()
	template = loader.get_template('index.html')
	context = RequestContext(request)
	return HttpResponse(template.render(context))
