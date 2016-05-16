import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from cnk.settings import *
from cnk_admin_site.constants import *
THRIFT_HOST = 'localhost'
THRIFT_PORT = 9090
FLOOR_TILES_DIRECTORY = "{}".format('/static/map_tiles/{f}/{z}/{x}/{y}.jpg')
EXCEL_FILES_ROOT = os.path.join(BASE_DIR, 'download/excel_files')
