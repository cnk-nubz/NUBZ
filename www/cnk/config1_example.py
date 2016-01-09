import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from cnk.settings import *
THRIFT_HOST = 'localhost'
THRIFT_PORT = 9090
FLOOR0_TILES_DIRECTORY = "{}".format('/static/map_tiles/0/{z}/{x}/{y}.jpg')
FLOOR1_TILES_DIRECTORY = "{}".format('/static/map_tiles/1/{z}/{x}/{y}.jpg')