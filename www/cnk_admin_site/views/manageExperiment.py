import json, os
from utils import handleException, mergeQuestions
from django.http import HttpResponse, JsonResponse, QueryDict
from ..thrift_communication import ThriftCommunicator, toThrift, fromThrift
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()


def experiment(request):
    try:
        if request.method == "POST":
            return createExperiment(request)
        elif request.method == "PUT":
            return updateExperiment(request)
    except Exception as ex:
        return handleException(ex)


def createExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    tData = toThrift.createExperimentRequest(data)
    thriftCommunicator.createExperiment(tData)
    return JsonResponse({
        "redirect": '/badania'
    })


def updateExperiment(request):
    PUT = QueryDict(request.body)
    data = json.loads(PUT.get("jsonData"))
    tData = toThrift.createExperimentRequest(data)
    thriftCommunicator.updateExperiment(data['experimentId'], tData)
    return JsonResponse({
        "redirect": '/badania'
    })


def activeExperiment(request):
    try:
        if request.method == "POST":
            return startExperiment(request)
        elif request.method == "DELETE":
            return finishExperiment(request)
    except Exception as ex:
        return handleException(ex)


def startExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    experimentId = data.get('experimentId')
    thriftCommunicator.startExperiment(experimentId)
    return HttpResponse()


def finishExperiment(request):
    thriftCommunicator.finishExperiment()
    return HttpResponse()


def cloneExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    tData = toThrift.cloneRequest(data)
    try:
        thriftCommunicator.cloneExperiment(tData)
        return HttpResponse()
    except Exception as ex:
        return handleException(ex)


def removeExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    experimentId = data.get('experimentId')
    try:
        thriftCommunicator.removeExperiment(experimentId)
        return JsonResponse({
            "redirect": "/badania/"
        })
    except Exception as ex:
        return handleException(ex)
