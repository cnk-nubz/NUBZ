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
			raise Exception('Brak polaczenia z serwerem: {} Upewnij sie, ze serwer jest wlaczony'.format(ex.args))

	def end_connection(self):
		try:
			self.transport.close()
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			raise Exception('Brak polaczenia z serwerem: {} Upewnij sie, ze serwer jest wlaczony'.format(ex.args))

	def _perform_actions(self, actions):
		self.start_connection()
		try:
			ret = [action() for action in actions]
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			raise Exception(message)

		self.end_connection()
		return ret

	def ping(self, number, text):
		def action():
			msg = HelloMsg(number, text)
			return self.client.ping(msg)
		return self._perform_actions([action])

	def getMapImages(self):
		def action():
			msg = MapImagesRequest()
			return self.client.getMapImages(msg)
		return self._perform_actions([action])

	def setMapImage(self, floor, filename):
		def action():
			msg = SetMapImageRequest(floor, filename)
			self.client.setMapImage(msg)
			return True
		return self._perform_actions([action])

	def getExhibits(self):
		def action():
			msg = NewExhibitsRequest()
			return self.client.getNewExhibits(msg)
		return self._perform_actions([action])

	def getMapImageTiles(self):
		def get_floor0():
			floor0_msg = MapImageTilesRequest(0)
			return self.client.getMapImageTiles(floor0_msg)
		def get_floor1():
			floor1_msg = MapImageTilesRequest(1)
			return self.client.getMapImageTiles(floor1_msg)
		return self._perform_actions([get_floor0, get_floor1])

	def setExhibitFrame(self, frame):
		def action():
			msg = SetExhibitFrameRequest(int(frame['id']), Frame(frame['x'], frame['y'], Size(frame['width'], frame['height'])))
			self.client.setExhibitFrame(msg)
			return True
		return self._perform_actions([action])

	def createNewExhibit(self, request):
		def action():
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
			return self.client.createExhibit(msg)
		return self._perform_actions([action])

	def createSimpleQuestion(self, request):
		def action():
			if request['answerType'] == 1:
				answerType = SimpleQuestionAnswerType.NUMBER
			else:
				answerType = SimpleQuestionAnswerType.TEXT
			msg = CreateSimpleQuestionRequest(request['name'], request['question'], answerType)
			return self.client.createSimpleQuestion(msg)
		return self._perform_actions([action])

	def createMultipleChoiceQuestion(self, request):
		def action():
			msg = CreateMultipleChoiceQuestionRequest(request['name'], request['question'], request['singleAnswer'], request['options'])
			return self.client.createMultipleChoiceQuestion(msg)
		return self._perform_actions([action])

	def createAction(self, request):
		def action():
			msg = CreateActionRequest(request['text'])
			return self.client.createAction(msg)
		return self._perform_actions([action])
