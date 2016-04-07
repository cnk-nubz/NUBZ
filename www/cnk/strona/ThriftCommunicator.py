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
        errorTemplate = "An exception of type {0} occured. Arguments:\n{1!r}"

        try:
            client = self._start_connection()
            ret = [action(client) for action in actions]
            self._end_connection()
        except InternalError as ex:
            message = errorTemplate.format(type(ex).__name__, ex.args)
            print message
            raise InternalError(message)
        except InvalidData as ex:
            message = errorTemplate.format(type(ex).__name__, ex.args)
            print message
            raise InvalidData(message)
        except DuplicateName as ex:
            message = errorTemplate.format(type(ex).__name__, ex.args)
            print message
            raise DuplicateName(message)
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

    def getAllExhibits(self):
        def action(client):
            return client.getAllExhibits()
        return self._perform_in_single_connection([action])[0]

    def getMapImageTiles(self):
        def action(client):
            floor_msg = NewMapImagesRequest()
            return client.getNewMapImages(floor_msg).floors
        return self._perform_in_single_connection([action])[0]

    def setExhibitFrame(self, frame):
        def action(client):
            msg = SetExhibitFrameRequest(int(frame['id']), Frame(
                frame['x'], frame['y'], Size(frame['width'], frame['height'])))
            client.setExhibitFrame(msg)
        return self._perform_in_single_connection([action])[0]

    def createNewExhibit(self, request):
        def action(client):
            if 'floor' in request.keys() and request['floor'] is not None:
                floor = request['floor']
            else:
                floor = None
            if 'visibleMapFrame' in request.keys() and request[
                    'visibleMapFrame']:
                t = request['visibleMapFrame']
                frame = MapFrame(
                    Frame(
                        t['x'],
                        t['y'],
                        Size(
                            t['width'],
                            t['height'])),
                    t['mapLevel'])
            else:
                frame = None
            msg = CreateExhibitRequest(request['name'].encode(
                'utf-8'), request['rgbHex'], floor, frame)
            return client.createExhibit(msg)
        return self._perform_in_single_connection([action])[0]

    def updateExhibit(self, request):
        def action(client):
            if 'floor' in request.keys() and request['floor'] is not None:
                floor = request['floor']
            else:
                floor = None
            if 'visibleMapFrame' in request.keys() and request[
                    'visibleMapFrame']:
                t = request['visibleMapFrame']
                frame = MapFrame(
                    Frame(
                        t['x'],
                        t['y'],
                        Size(
                            t['width'],
                            t['height'])),
                    t['mapLevel'])
            else:
                frame = None
            msg = UpdateExhibitRequest(
                request['id'], request['rgbHex'], floor, frame)
            return client.updateExhibit(msg)
        return self._perform_in_single_connection([action])[0]

    def createSimpleQuestion(self, request):
        def action(client):
            if request['answerAsNumber'] == 1:
                answerType = SimpleQuestionAnswerType.NUMBER
            else:
                answerType = SimpleQuestionAnswerType.TEXT
            msg = CreateSimpleQuestionRequest(request['name'].encode(
                'utf-8'), request['question'].encode('utf-8'), answerType)
            return client.createSimpleQuestion(msg)
        return self._perform_in_single_connection([action])[0]

    def createMultipleChoiceQuestion(self, request):
        def action(client):
            options = [opt.encode('utf-8') for opt in request['options']]
            msg = CreateMultipleChoiceQuestionRequest(
                request['name'].encode('utf-8'),
                request['question'].encode('utf-8'),
                request['singleAnswer'],
                options)
            return client.createMultipleChoiceQuestion(msg)
        return self._perform_in_single_connection([action])[0]

    def createAction(self, request):
        def action(client):
            msg = CreateActionRequest(request['text'].encode('utf-8'))
            return client.createAction(msg)
        return self._perform_in_single_connection([action])[0]

    def createSortQuestion(self, request):
        def action(client):
            options = [opt.encode('utf-8') for opt in request['options']]
            msg = CreateSortQuestionRequest(
                request['name'].encode('utf-8'),
                request['question'].encode('utf-8'),
                options)
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

    def _prepareCreateExperimentRequest(self, request):
        sb = request['surveyBefore']
        sa = request['surveyAfter']
        surveyBefore = QuestionsIdsList(
            sb['questionsOrder'],
            sb['simpleQuestions'],
            sb['multipleChoiceQuestions'],
            sb['sortQuestions'])
        surveyAfter = QuestionsIdsList(
            sa['questionsOrder'],
            sa['simpleQuestions'],
            sa['multipleChoiceQuestions'],
            sa['sortQuestions'])
        return CreateExperimentRequest(
            request['name'].encode('utf-8'),
            surveyBefore,
            request['exhibitActions'],
            request['breakActions'],
            surveyAfter)

    def createExperiment(self, request):
        def action(client):
            msg = self._prepareCreateExperimentRequest(request)
            client.createExperiment(msg)
        return self._perform_in_single_connection([action])[0]

    def updateExperiment(self, request):
        def action(client):
            msg = self._prepareCreateExperimentRequest(request)
            client.updateExperiment(request['experimentId'], msg)
        return self._perform_in_single_connection([action])[0]

    def getFinishedExperiments(self):
        def action(client):
            return client.getFinishedExperiments()
        return self._perform_in_single_connection([action])[0]

    def getReadyExperiments(self):
        def action(client):
            return client.getReadyExperiments()
        return self._perform_in_single_connection([action])[0]

    def getActiveExperiment(self):
        def action(client):
            return client.getActiveExperiment()
        return self._perform_in_single_connection([action])[0]

    def getExperiment(self, experimentId):
        def action(client):
            return client.getExperiment(experimentId)
        return self._perform_in_single_connection([action])[0]

    def startExperiment(self, experimentId):
        def action(client):
            return client.startExperiment(experimentId)
        return self._perform_in_single_connection([action])[0]

    def finishExperiment(self):
        def action(client):
            return client.finishExperiment()
        return self._perform_in_single_connection([action])[0]

    def cloneExperiment(self, experimentId, newName):
        def action(client):
            msg = CloneRequest(experimentId, newName.encode('utf-8'))
            return client.cloneExperiment(msg)
        return self._perform_in_single_connection([action])[0]

    def getAllReportsForExperiment(self, experimentId):
        def action(client):
            return client.getAllReportsForExperiment(experimentId)
        return self._perform_in_single_connection([action])[0]

    def getExcelReport(self, reportId):
        def action(client):
            return client.getExcelReport(reportId)
        return self._perform_in_single_connection([action])[0]

    def getCombinedExcelReport(self, experimentId):
        def action(client):
            return client.getCombinedExcelReport(experimentId)
        return self._perform_in_single_connection([action])[0]
