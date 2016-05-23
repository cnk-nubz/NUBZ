import os
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'
from django.conf      import settings
from thrift           import Thrift
from thrift.transport import TSocket, TTransport
from thrift.protocol  import TBinaryProtocol
from thrift.server    import TServer
from server           import Server
from structs.ttypes   import *

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

    def _perform_in_connection(self, action):
        client = self._start_connection()
        ret = action(client)
        self._end_connection()
        return ret

    def ping(self, t):
        def action(client): return client.ping(t)
        return self._perform_in_connection(action)

    def setMapImage(self, msg):
        def action(client): return client.setMapImage(msg)
        return self._perform_in_connection(action)

    def getAllExhibits(self):
        def action(client): return client.getAllExhibits()
        return self._perform_in_connection(action)

    def getMapImages(self):
        def action(client): return client.getMapImages()
        return self._perform_in_connection(action)

    def setExhibitFrame(self, msg):
        def action(client): return client.setExhibitFrame(msg)
        return self._perform_in_connection(action)

    def createExhibit(self, msg):
        def action(client): return client.createExhibit(msg)
        return self._perform_in_connection(action)

    def updateExhibit(self, msg):
        def action(client): return client.updateExhibit(msg)
        return self._perform_in_connection(action)

    def removeExhibit(self, msg):
        def action(client): return client.removeExhibit(msg)
        return self._perform_in_connection(action)

    def createSimpleQuestion(self, msg):
        def action(client): return client.createSimpleQuestion(msg)
        return self._perform_in_connection(action)

    def createMultipleChoiceQuestion(self, msg):
        def action(client): return client.createMultipleChoiceQuestion(msg)
        return self._perform_in_connection(action)

    def createAction(self, msg):
        def action(client): return client.createAction(msg)
        return self._perform_in_connection(action)

    def createSortQuestion(self, msg):
        def action(client): return client.createSortQuestion(msg)
        return self._perform_in_connection(action)

    def getAllQuestions(self):
        def action(client): return client.getAllQuestions()
        return self._perform_in_connection(action)

    def getAllActions(self):
        def action(client): return client.getAllActions()
        return self._perform_in_connection(action)

    def createExperiment(self, msg):
        def action(client): return client.createExperiment(msg)
        return self._perform_in_connection(action)

    def updateExperiment(self, experimentId, msg):
        def action(client): return client.updateExperiment(experimentId, msg)
        return self._perform_in_connection(action)

    def getFinishedExperiments(self):
        def action(client): return client.getFinishedExperiments()
        return self._perform_in_connection(action)

    def getReadyExperiments(self):
        def action(client): return client.getReadyExperiments()
        return self._perform_in_connection(action)

    def getActiveExperiment(self):
        def action(client): return client.getActiveExperiment()
        return self._perform_in_connection(action)

    def getExperiment(self, experimentId):
        def action(client): return client.getExperiment(experimentId)
        return self._perform_in_connection(action)

    def startExperiment(self, experimentId):
        def action(client): return client.startExperiment(experimentId)
        return self._perform_in_connection(action)

    def finishExperiment(self):
        def action(client): return client.finishExperiment()
        return self._perform_in_connection(action)

    def cloneExperiment(self, msg):
        def action(client): return client.cloneExperiment(msg)
        return self._perform_in_connection(action)

    def getAllReportsForExperiment(self, experimentId):
        def action(client): return client.getAllReportsForExperiment(experimentId)
        return self._perform_in_connection(action)

    def getExcelReport(self, reportId):
        def action(client): return client.getExcelReport(reportId)
        return self._perform_in_connection(action)

    def getCombinedExcelReport(self, experimentId):
        def action(client): return client.getCombinedExcelReport(experimentId)
        return self._perform_in_connection(action)

    def removeFloor(self, floor):
        def action(client): return client.removeFloor(floor)
        return self._perform_in_connection(action)

    def removeAction(self, actionId):
        def action(client): return client.removeAction(actionId)
        return self._perform_in_connection(action)

    def removeSimpleQuestion(self, questionId):
        def action(client): return client.removeSimpleQuestion(questionId)
        return self._perform_in_connection(action)

    def removeMultipleChoiceQuestion(self, questionId):
        def action(client): return client.removeMultipleChoiceQuestion(questionId)
        return self._perform_in_connection(action)

    def removeSortQuestion(self, questionId):
        def action(client): return client.removeSortQuestion(questionId)
        return self._perform_in_connection(action)

    def removeExperiment(self, experimentId):
        def action(client): return client.removeExperiment(experimentId)
        return self._perform_in_connection(action)
