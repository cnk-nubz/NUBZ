import os
from django.conf import settings
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'

from thrift import Thrift
from thrift.transport import TSocket, TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

from server import Server
from server.ttypes import *

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
		msg = structs.ttypes.HelloMsg(number, text)
		return self.client.ping(msg)
