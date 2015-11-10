import sys
from django.shortcuts import render
from django.http import HttpResponse
from django.template import RequestContext, loader
from ThriftCommunicator import ThriftCommunicator

def pingpong(request):
	communicator = ThriftCommunicator()
	communicator.start_connection()
	resulta = communicator.ping(12333, "asdqwerty")
	template = loader.get_template('pingpong.html')
	context = RequestContext(request, {"tekst": resulta})
	return HttpResponse(template.render(context))