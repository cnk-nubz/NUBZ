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
		ret = True
		try:
			self.transport = TSocket.TSocket(self.host, self.port)
	  		self.transport = TTransport.TBufferedTransport(self.transport)
	  		self.protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
			self.transport.open()
			self.client = Server.Client(self.protocol)
		except:
			ret = False
		return ret

	def end_connection(self):
		ret = True
		try:
			self.transport.close()
		except:
			ret = False
		return ret

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
		except:
			ret = None

		if not self.end_connection():
			return None

		return ret

	def setMapImage(self, floor, filename):
		msg = SetMapImageRequest(floor, filename)
		ret = True
		if not self.start_connection():
			return None
		try:
			self.client.setMapImage(msg)
		except:
			ret = None #failed to set the map

		if not self.end_connection():
			return None
		return ret
