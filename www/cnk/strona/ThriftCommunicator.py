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

	def _start_connection(self):
		socket = TSocket.TSocket(self.host, self.port)
		self.transport = TTransport.TFramedTransport(socket)
		self.protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
		self.transport.open()
		self.client = Server.Client(self.protocol)
		return self.client

	def _end_connection(self):
		self.transport.close()

	def _perform_in_single_connection(self, actions):
		try:
			client = self._start_connection()
			ret = [action(client) for action in actions]
			self._end_connection()
		except Exception as ex:
			template = "An exception of type {0} occured. Arguments:\n{1!r}"
			message = template.format(type(ex).__name__, ex.args)
			print message
			raise Exception(message)
		return ret

	def ping(self, number, text):
		def action(client):
			msg = HelloMsg(number, text)
			return client.ping(msg)
		return self._perform_in_single_connection([action])[0]

	def setMapImage(self, floor, filename):
		def action(client):
			msg = SetMapImageRequest(floor, filename)
			client.setMapImage(msg)
		return self._perform_in_single_connection([action])[0]

	def getExhibits(self):
		def action(client):
			msg = NewExhibitsRequest()
			return client.getNewExhibits(msg)
		return self._perform_in_single_connection([action])[0]

	def getMapImageTiles(self):
		def action(client):
			floor_msg = NewMapImagesRequest()
			return client.getNewMapImages(floor_msg).floors
		return self._perform_in_single_connection([action])[0]

	def setExhibitFrame(self, frame):
		def action(client):
			msg = SetExhibitFrameRequest(int(frame['id']), Frame(frame['x'], frame['y'], Size(frame['width'], frame['height'])))
			client.setExhibitFrame(msg)
		return self._perform_in_single_connection([action])[0]

	def createNewExhibit(self, request):
		def action(client):
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
			msg = CreateExhibitRequest(request['name'], request['rgbHex'], floor, frame)
			return client.createExhibit(msg)
		return self._perform_in_single_connection([action])[0]

	def updateExhibit(self, request):
		def action(client):
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
			msg = UpdateExhibitRequest(request['id'], request['rgbHex'], floor, frame)
			return client.updateExhibit(msg)
		return self._perform_in_single_connection([action])[0]

	def createSimpleQuestion(self, request):
		def action(client):
			if request['answerAsNumber'] == 1:
				answerType = SimpleQuestionAnswerType.NUMBER
			else:
				answerType = SimpleQuestionAnswerType.TEXT
			msg = CreateSimpleQuestionRequest(request['name'], request['question'], answerType)
			return client.createSimpleQuestion(msg)
		return self._perform_in_single_connection([action])[0]

	def createMultipleChoiceQuestion(self, request):
		def action(client):
			msg = CreateMultipleChoiceQuestionRequest(request['name'], request['question'], request['singleAnswer'], request['options'])
			return client.createMultipleChoiceQuestion(msg)
		return self._perform_in_single_connection([action])[0]

	def createAction(self, request):
		def action(client):
			msg = CreateActionRequest(request['text'])
			return client.createAction(msg)
		return self._perform_in_single_connection([action])[0]

	def createSortQuestion(self, request):
		def action(client):
			msg = CreateSortQuestionRequest(request['name'], request['question'], request['options'])
			return client.createSortQuestion(msg)
		return self._perform_in_single_connection([action])[0]

	def getAllQuestions(self):
		def action(client):
			return client.getAllQuestions()
		return self._perform_in_single_connection([action])[0]

	def getAllActions(self):
		def action(client):
			return client.getAllActions()
		return self._perform_in_single_connection([action])[0]

	def createExperiment(self, request):
		def action(client):
			sb = request['surveyBefore']
			sa = request['surveyAfter']
			surveyBefore = QuestionsIdsList(sb['questionsOrder'], sb['simpleQuestions'], sb['multipleChoiceQuestions'], sb['sortQuestions'])
			surveyAfter = QuestionsIdsList(sa['questionsOrder'], sa['simpleQuestions'], sa['multipleChoiceQuestions'], sa['sortQuestions'])
			msg = CreateExperimentRequest(request['name'], surveyBefore, request['exhibitActions'], request['breakActions'], surveyAfter)
			client.createExperiment(msg)
		return self._perform_in_single_connection([action])[0]
