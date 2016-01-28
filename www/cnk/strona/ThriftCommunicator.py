import os
from django.conf import settings
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'

from thrift import Thrift
from thrift.transport import TSocket, TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

from server import Server
from structs.ttypes import *

class ThriftCommunicator:
	def __init__(self):
		self.host = getattr(settings, 'THRIFT_HOST', 'localhost')
		self.port = int(getattr(settings, 'THRIFT_PORT', 9090))
		self.transport = None
		self.protocol = None
		self.client = None

	def start_connection(self):
		try:
			socket = TSocket.TSocket(self.host, self.port)
			self.transport = TTransport.TFramedTransport(socket)
			self.protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
			self.transport.open()
			self.client = Server.Client(self.protocol)
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None
		return True

	def end_connection(self):
		try:
			self.transport.close()
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None
		return True

	def ping(self, number, text):
		msg = HelloMsg(number, text)
		if not self.start_connection():
			return None
		ret = self.client.ping(msg)
		if not self.end_connection():
			return None
		return ret

	def getMapImages(self):
		msg = MapImagesRequest()
		if not self.start_connection():
			return None

		try:
			ret = self.client.getMapImages(msg)
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None

		if not self.end_connection():
			return None

		return ret

	def setMapImage(self, floor, filename):
		msg = SetMapImageRequest(floor, filename)
		if not self.start_connection():
			return None

		try:
			self.client.setMapImage(msg)
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None

		if not self.end_connection():
			return None
		return True

	def getExhibits(self):
		msg = NewExhibitsRequest()
		if not self.start_connection():
			return None

		try:
			ret = self.client.getNewExhibits(msg)
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None

		if not self.end_connection():
			return None
		return ret

	def getMapImageTiles(self, floor):
		msg = MapImageTilesRequest(floor)
		if not self.start_connection():
			return None

		try:
			ret = self.client.getMapImageTiles(msg)
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None

		if not self.end_connection():
			return None
		return ret

	def setExhibitFrame(self, frame):
		msg = SetExhibitFrameRequest(int(frame['id']), Frame(frame['x'], frame['y'], Size(frame['width'], frame['height'])))
		if not self.start_connection():
			return None

		try:
			self.client.setExhibitFrame(msg)
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None

		if not self.end_connection():
			return None
		return True

	def createNewExhibit(self, request):
		if 'floor' in request.keys() and request['floor'] != None:
			floor = request['floor']
		else:
			floor = None
		if 'visibleMapFrame' in request.keys() and request['visibleMapFrame']:
			t = request['visibleMapFrame']
			frame = MapFrame(
				Frame(t['x'], t['y'], Size(t['width'], t['height'])), t['mapLevel'])
		else:
			frame = None
		msg = CreateExhibitRequest(request['name'], floor, frame)
		if not self.start_connection():
			return None

		try:
			ret = self.client.createExhibit(msg)
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			return None

		if not self.end_connection():
			return None
		return ret
