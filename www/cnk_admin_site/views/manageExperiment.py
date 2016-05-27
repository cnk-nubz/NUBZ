import json, os
from utils                  import standardAjaxCall, mergeQuestions
from django.http            import HttpResponse, JsonResponse, QueryDict
from ..thrift_communication import ThriftCommunicator, toThrift, fromThrift
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()


@standardAjaxCall
def experiment(request):
    if request.method == "POST":
        return createExperiment(request)
    elif request.method == "PUT":
        return updateExperiment(request)


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


@standardAjaxCall
def activeExperiment(request):
    if request.method == "POST":
        return startExperiment(request)
    elif request.method == "DELETE":
        return finishExperiment(request)


def startExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    experimentId = data.get('experimentId')
    thriftCommunicator.startExperiment(experimentId)
    return HttpResponse()


def finishExperiment(request):
    thriftCommunicator.finishExperiment()
    return HttpResponse()


@standardAjaxCall
def cloneExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    tData = toThrift.cloneExperimentRequest(data)
    thriftCommunicator.cloneExperiment(tData)
    return HttpResponse()


@standardAjaxCall
def removeExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    experimentId = data.get('experimentId')
    thriftCommunicator.removeExperiment(experimentId)
    return JsonResponse({
        "redirect": "/badania/"
    })
