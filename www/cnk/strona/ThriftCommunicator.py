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
		self.host = getattr('settings', 'THRIFT_HOST', 'localhost')
		self.port = int(getattr('settings', 'THRIFT_PORT', 9090))
		self.transport = None
		self.protocol = None
		self.client = None

	def start_connection(self):
		self.transport = TSocket.TSocket(self.host, self.port)
  		self.transport = TTransport.TBufferedTransport(self.transport)
  		self.protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
		self.transport.open()
		self.client = Server.Client(self.protocol)

	def end_connection(self):
		self.transport.close()

	def ping(self, number, text):
		msg = HelloMsg(number, text)
		self.start_connection()
		ret = self.client.ping(msg)
		self.end_connection()
		return ret

	def getMapImages(self):
		msg = MapImagesRequest()
		self.start_connection()
		ret = self.client.getMapImages(msg)
		self.end_connection()
		return ret

	def setMapImage(self, floor, filename):
		msg = SetMapImageRequest(floor, filename)
		ret = True
		self.start_connection()
		try:
			self.client.setMapImage(msg)
		except:
			ret = False #failed to set the map
		self.end_connection()
		return ret
