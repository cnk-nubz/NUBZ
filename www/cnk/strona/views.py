import sys
import os
import json
from enum import Enum
from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse
from django.template import RequestContext, loader
from django.template.loader import render_to_string
from django.core.urlresolvers import reverse
from django.views.decorators.csrf import ensure_csrf_cookie
from django.utils.safestring import mark_safe
from django.conf import settings
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

thriftCommunicator = ThriftCommunicator()
def get_const(name):
    return getattr(settings, name, None)

defaultImage = {
    'tileWidth': 2200,
    'tileHeight': 1700,
    'scaledWidth': 2200,
    'scaledHeight': 1700
}

class uploadError(Enum):
	SUCCESS = 1
	NOT_AN_IMAGE = 2
	SERVER_PROBLEM = 3
	WRONG_REQUEST_METHOD = 4
	INVALID_DATA = 5

class QuestionType(Enum):
    SIMPLE = 0
    MULTIPLE = 1
    SORT = 2

class ActiveLink(Enum):
    JUST_MAP = '0'
    EDIT_MAP = '1'
    NEW_EXPERIMENT = '2'
    QUESTIONS_ACTIONS_PAGE = '3'

def _pingServer():
    result = thriftCommunicator.ping(1, 'x')
    return result == 1

def _getMapImageInfo():
	floorTiles = thriftCommunicator.getMapImageTiles()
	floorTilesInfo = {}

	for i in xrange(0, 2):
		if not floorTiles[i]:
			continue

		if not floorTiles[i].zoomLevels:
			floorTilesInfo[i] = {
			0: {
				'tileWidth': defaultImage['tileWidth'],
				'tileHeight': defaultImage['tileHeight'],
				'scaledWidth': defaultImage['scaledWidth'],
				'scaledHeight': defaultImage['scaledHeight']
			}}
		else:
			floorTilesInfo[i] = {
			idx: {
				'tileWidth': zoom.tileSize.width,
				'tileHeight': zoom.tileSize.height,
				'scaledWidth': zoom.scaledSize.width,
				'scaledHeight': zoom.scaledSize.height
			} for idx, zoom in enumerate(floorTiles[i].zoomLevels)}

	return floorTilesInfo

def _getExhibits():
	result = thriftCommunicator.getExhibits()

	exhibitsDict = {}
	for k in result.exhibits:
		e = result.exhibits[k]
		frame = None
		if e.mapFrame != None:
			frame = {
				'x': e.mapFrame.frame.x,
				'y': e.mapFrame.frame.y,
				'width': e.mapFrame.frame.size.width,
				'height': e.mapFrame.frame.size.height,
				'mapLevel': e.mapFrame.floor
			}
		exhibitsDict[k] = {
			'name': e.name,
			'id': k,
			'frame': frame
		}
	return exhibitsDict

@ensure_csrf_cookie
def getMapPage(request, file, activeLink):
	try:
		_pingServer()
		floorTilesInfo = _getMapImageInfo()
		exhibits = _getExhibits()
	except Exception as ex:
		return HttpResponse('<h1>{}</h1>'.format(str(ex)))

	template = loader.get_template(file)

	if len(floorTilesInfo[0]) == 1: #just default image
		urlFloor0 = r"/static/floorplan0.jpg"
	else: #get from config file
		urlFloor0 = getattr(settings, 'FLOOR0_TILES_DIRECTORY', r"/static/floorplan0.jpg")

	if len(floorTilesInfo[1]) == 1:
		urlFloor1 = r"/static/floorplan1.jpg"
	else:
		urlFloor1 = getattr(settings, 'FLOOR1_TILES_DIRECTORY', r"/static/floorplan1.jpg")

	context = RequestContext(request, {
		'activeFloor': 0,
		'exhibits': exhibits,
		'floorTilesInfo': floorTilesInfo,
		'urlFloor0': urlFloor0,
		'urlFloor1': urlFloor1,
        'activeLink': activeLink
	})
	return HttpResponse(template.render(context))

@ensure_csrf_cookie
def index(request):
	return getMapPage(request, 'map/justMap.html', ActiveLink.JUST_MAP.value)

@ensure_csrf_cookie
def editMapPage(request):
    return getMapPage(request, 'map/editMap.html', ActiveLink.EDIT_MAP.value)

def uploadImage(request):
	if request.method != 'POST':
		data = {
			"err": uploadError.WRONG_REQUEST_METHOD.value,
		}
		return JsonResponse(data)

	form = MapUploadForm(request.POST, request.FILES)
	if not form.is_valid():
		data = {
			"err": uploadError.NOT_AN_IMAGE.value,
		}
		return JsonResponse(data)

	m = MapUploader(image = form.cleaned_data['image'])
	m.save()
	floor = form.cleaned_data['floor']
	#send information about update
	filename = m.image.name
	#extract filename
	try:
		setResult = thriftCommunicator.setMapImage(floor, os.path.basename(filename))
		floorTilesInfo = _getMapImageInfo()
	except Exception as ex:
		data = {
			"err": uploadError.SERVER_PROBLEM.value,
		}
		return JsonResponse(data)

	if len(floorTilesInfo[floor]) == 1: #just default image
		floorUrl = r"/static/floorplan0.jpg"
	else: #get from config file
		if floor == 0:
			floorUrl = getattr(settings, 'FLOOR0_TILES_DIRECTORY', r"/static/floorplan0.jpg")
		else:
			floorUrl = getattr(settings, 'FLOOR1_TILES_DIRECTORY', r"/static/floorplan1.jpg")
	data = {
		"err": uploadError.SUCCESS.value,
		"floor": floor,
		"floorTilesInfo": floorTilesInfo,
		"floorUrl": floorUrl
	}
	return JsonResponse(data)

def updateExhibitPosition(request):
	data = {
		"success": False
	}
	if request.method != 'POST':
		return JsonResponse(data)
	jsonData = request.POST.get("jsonData")
	frame = json.loads(jsonData)

	try:
		thriftCommunicator.setExhibitFrame(frame)
	except Exception as ex:
		data['message'] = str(ex)
		return JsonResponse(data)

	data = {
		"success": True,
		"id": int(frame['id']),
		"x": frame['x'],
		"y": frame['y'],
        "width": frame['width'],
        "height": frame['height']
	}
	return JsonResponse(data)

def createNewExhibit(request):
	data = {
		"success": False
	}
	if request.method != 'POST':
		return JsonResponse(data)
	jsonData = request.POST.get("jsonData")
	exhibitRequest = json.loads(jsonData)

	try:
		newExhibit = thriftCommunicator.createNewExhibit(exhibitRequest)
	except Exception as ex:
		data['message'] = str(ex)
		return JsonResponse(data)

	if newExhibit.mapFrame:
		frame = newExhibit.mapFrame
		exhibitFrame = {
			"x": frame.frame.x,
			"y": frame.frame.y,
			"width": frame.frame.size.width,
			"height": frame.frame.size.height,
			"mapLevel": frame.floor
		}
	else:
		exhibitFrame = None
	data = {
		"success": True,
		"id": int(newExhibit.exhibitId),
		"name": newExhibit.name,
		"frame": exhibitFrame
	}
	return JsonResponse(data)

def getDialog(request, dialogName):
	contextDict = {
		'data': get_const(dialogName)['data']
	}
	html = render_to_string('dialog/dialog.html', contextDict)
	retDict = {
		'data': get_const(dialogName),
		'html': html.replace("\n", "")
	}
	return JsonResponse(retDict)

def getSimpleQuestionDialog(request):
    return getDialog(request, "SIMPLE_QUESTION_DIALOG")

def getMultipleChoiceQuestionDialog(request):
    return getDialog(request, "MULTIPLE_CHOICE_QUESTION_DIALOG")

def getExhibitPanel(request):
    html = render_to_string('exhibitPanel/exhibitPanel.html')
    return JsonResponse({'html': html})

def getExhibitListElement(request):
    html = render_to_string('exhibitPanel/exhibitListElement.html')
    return JsonResponse({'html': html})

def getSortQuestionDialog(request):
    return getDialog(request, "SORT_QUESTION_DIALOG")

def getActionDialog(request):
    return getDialog(request, "NEW_ACTION_DIALOG")

def getChangeMapDialog(request):
    floor = request.POST.get("floor")
    html = render_to_string('dialog/changeMap.html', {'floor': floor})
    return JsonResponse({'html': html.replace("\n", "")})

def getChooseQuestionTypeDialog(request):
    html = render_to_string('dialog/chooseQuestionType.html')
    return JsonResponse({'html': html.replace("\n", "")})

HTMLRequests = {
    'simpleQuestionDialog': getSimpleQuestionDialog,
    'sortQuestionDialog': getSortQuestionDialog,
    'multipleChoiceQuestionDialog': getMultipleChoiceQuestionDialog,
    'actionDialog': getActionDialog,
    'chooseQuestionTypeDialog': getChooseQuestionTypeDialog,
    'changeMapDialog': getChangeMapDialog,
    'exhibitPanel': getExhibitPanel,
    'exhibitListElement': getExhibitListElement
}

def getHTML(request):
    name = request.GET.get("name")
    return HTMLRequests[name](request)

def _getAllQuestions(newId = None, newType = None):
    allQuestions = thriftCommunicator.getAllQuestions()
    idxSimple = 0
    idxMultiple = 0
    idxSort = 0
    questionsList = []
    # sort questions in order
    for order in allQuestions.questionsOrder:
        if order == QuestionType.SIMPLE.value:
            q = allQuestions.simpleQuestions[idxSimple]
            idxSimple += 1
            questionsList.append({
                'questionId': q.questionId,
                'isNew': newId == q.questionId and order == newType,
                'type': QuestionType.SIMPLE.value,
                'name': q.name,
                'question': q.question,
                'answerType': q.answerType
            })
        elif order == QuestionType.MULTIPLE.value:
            q = allQuestions.multipleChoiceQuestions[idxMultiple]
            idxMultiple += 1
            questionsList.append({
                'questionId': q.questionId,
                'isNew': newId == q.questionId and order == newType,
                'type': QuestionType.MULTIPLE.value,
                'name': q.name,
                'question': q.question,
                'singleAnswer': q.singleAnswer,
                'options': [o.text for o in q.options]
            })
        else:
            q = allQuestions.sortQuestions[idxSort]
            idxSort += 1
            questionsList.append({
                'questionId': q.questionId,
                'isNew': newId == q.questionId and order == newType,
                'type': QuestionType.SORT.value,
                'name': q.name,
                'question': q.question,
                'options': [o.text for o in q.options]
            })
    return questionsList

def _getAllActions(newId = None):
    allActions = thriftCommunicator.getAllActions()
    actionsList = [{
        'actionId': a.actionId,
        'isNew': newId == a.actionId,
        'text': a.text
    } for a in allActions]
    return actionsList

def newExperimentPage(request):
    try:
        result = {
            'success': True,
            'activeLink' : ActiveLink.NEW_EXPERIMENT.value,
            'questionsList': _getAllQuestions(),
            'actionsList': _getAllActions(),
            'inputRegex': get_const("DEFAULT_CONSTANTS")['utils']['regex']['input'],
            'chooseQuestionRow': render_to_string('list/row/chooseQuestionRow.html'),
            'chooseActionRow': render_to_string('list/row/chooseActionRow.html'),
            'experimentActionRow': render_to_string('list/row/experimentActionRow.html'),
            'experimentQuestionRow': render_to_string('list/row/experimentQuestionRow.html'),
            'tableList': render_to_string('list/dataList.html')
        }
    except Exception as ex:
        result = {
            'success': False,
            'activeLink': ActiveLink.NEW_EXPERIMENT.value,
            'message': str(ex)
        }
    template = loader.get_template('newExperimentPage.html')
    return HttpResponse(template.render(RequestContext(request, result)))

def questionsAndActionsPage(request):
    try:
        result = {
            'success': True,
            'activeLink': ActiveLink.QUESTIONS_ACTIONS_PAGE.value,
            'questionsList': _getAllQuestions(),
            'actionsList': _getAllActions(),
            'showQuestionRow': render_to_string('list/row/showQuestionRow.html'),
            'showActionRow': render_to_string('list/row/showActionRow.html'),
            'tableList': render_to_string('list/dataList.html')
        }
    except Exception as ex:
        result = {
            'success': False,
            'activeLink': ActiveLink.QUESTIONS_ACTIONS_PAGE.value,
            'message': str(ex)
        }
    template = loader.get_template('questionsAndActionsPage.html')
    return HttpResponse(template.render(RequestContext(request, result)))

def createSimpleQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        simpleQuestion = thriftCommunicator.createSimpleQuestion(data)
        result = {
            'success': True,
            'questionsList': _getAllQuestions(simpleQuestion.questionId, QuestionType.SIMPLE.value)
        }
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    return JsonResponse(result)

def createMultipleChoiceQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        multipleChoiceQuestion = thriftCommunicator.createMultipleChoiceQuestion(data)
        result = {
            'success': True,
            'questionsList': _getAllQuestions(multipleChoiceQuestion.questionId, QuestionType.MULTIPLE.value)
        }
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    return JsonResponse(result)

def createSortQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        sortQuestion = thriftCommunicator.createSortQuestion(data)
        result = {
            'success': True,
            'questionsList': _getAllQuestions(sortQuestion.questionId, QuestionType.SORT.value)
        }
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    return JsonResponse(result)

def createAction(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
    	action = thriftCommunicator.createAction(data)
        result = {
            'success': True,
            'actionsList': _getAllActions(action.actionId)
        }

    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    return JsonResponse(result)

def createExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    # TODO
    result = {
        'success': False,
        'message': "TODO"
    }
    return JsonResponse(result)
