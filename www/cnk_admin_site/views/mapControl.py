import json, os, copy
from utils import handleException, get_const, getMapImageInfo
from django.http import HttpResponseBadRequest, HttpResponse, JsonResponse, QueryDict, HttpResponseForbidden
from ..thrift_communication import ThriftCommunicator, toThrift, fromThrift
from ..models import MapUploader
from ..forms import MapUploadForm
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()

def uploadImage(request):
    form = MapUploadForm(request.POST, request.FILES)
    if not form.is_valid():
        return HttpResponseBadRequest(get_const("NEW_FLOOR_IMAGE_WRONG_FORMAT"))
    try:
        m = MapUploader(image=form.cleaned_data['image'])
        m.save()
        floor = form.cleaned_data['floor']
        filename = m.image.name
        thriftCommunicator.setMapImage(toThrift.setMapImageRequest({
            'floor': floor,
            'filename': os.path.basename(filename)}))

        floorTilesInfo = getMapImageInfo()
        dialog = copy.copy(get_const("NEW_FLOOR_IMAGE_SUCCESS"))
        dialog['message'] = "{} {}".format(dialog['message'], floor)
        return JsonResponse(dialog)
    except Exception as ex:
        return handleException(ex)


def updateExhibitPosition(request):
    jsonData = request.POST.get("jsonData")
    frame = json.loads(jsonData)
    try:
        thriftCommunicator.setExhibitFrame(toThrift.setExhibitFrameRequest(frame))
        return HttpResponse()
    except Exception as ex:
        return handleException(ex)


def exhibit(request):
    try:
        if request.method == "POST":
            return createNewExhibit(request)
        elif request.method == "PUT":
            return updateExhibit(request)
        elif request.method == "DELETE":
            return removeExhibit(request)
        elif request.method == "GET":
            return getAllExhibits(request)
    except Exception as ex:
        return handleException(ex)


def createNewExhibit(request):
    data = json.loads(request.POST.get("jsonData"))
    tData = toThrift.createExhibitRequest(data)
    newExhibit = thriftCommunicator.createExhibit(tData)
    return JsonResponse(fromThrift.exhibit(newExhibit))


def updateExhibit(request):
    PUT = QueryDict(request.body)
    data = json.loads(PUT.get("jsonData"))
    tData = toThrift.updateExhibitRequest(data)
    newExhibit = thriftCommunicator.updateExhibit(tData)
    return JsonResponse(fromThrift.exhibit(newExhibit))


def removeExhibit(request):
    DELETE = QueryDict(request.body)
    data = json.loads(DELETE.get("jsonData"))
    exhibitId = data.get('exhibitId')
    thriftCommunicator.removeExhibit(exhibitId)
    return HttpResponse()


def getAllExhibits(request):
    return JsonResponse({
        'exhibits': map(fromThrift.exhibit, thriftCommunicator.getAllExhibits())
    })


def removeFloor(request):
    floor = int(request.POST.get("floor"))
    try:
        thriftCommunicator.removeFloor(floor)
        return HttpResponse()
    except Exception as ex:
        if type(ex).__name__ == 'InvalidData':
            return HttpResponseForbidden(
                "{} {}".format(get_const("NO_SUCH_FLOOR_ERROR"), floor))
        else:
            return handleException(ex)
