import os, StringIO
from django.http import HttpResponse
from utils import handleException, get_const, maybe
from ..thrift_communication import ThriftCommunicator, toThrift, fromThrift
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()


def getReport(request):
    reportId = maybe(int, request.GET.get("id"))
    try:
        filename = thriftCommunicator.getExcelReport(reportId)
        return _getReportFile(filename)
    except Exception as ex:
        return handleException(ex)


def getAllReports(request):
    experimentId = maybe(int, request.GET.get("id"))
    try:
        filename = thriftCommunicator.getCombinedExcelReport(experimentId)
        return _getReportFile(filename)
    except Exception as ex:
        return handleException(ex)


def _getReportFile(filename):
    path = os.path.join(get_const("EXCEL_FILES_ROOT"), filename)
    myStream = StringIO.StringIO()
    myStream.write(open(path, 'rb').read())
    os.remove(path)
    response = HttpResponse(content_type='application/x-zip-compressed')
    response[
        'Content-Disposition'] = "attachment; filename={}".format(filename)
    response['Content-Length'] = myStream.tell()
    response.write(myStream.getvalue())
    return response
