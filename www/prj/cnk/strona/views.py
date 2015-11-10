import sys
from django.shortcuts import render
from django.http import HttpResponse
from django.template import RequestContext, loader
from ThriftCommunicator import ThriftCommunicator


def index(request):
	template = loader.get_template('index.html')
	context = RequestContext(request)
	return HttpResponse(template.render(context))

def thrift(request):
	communicator = ThriftCommunicator()
	communicator.start_connection()
	resulta = communicator.ping(12333, "asdqwerty")
	template = loader.get_template('thrift.html')
	context = RequestContext(request, {"tekst": resulta})
	return HttpResponse(template.render(context))