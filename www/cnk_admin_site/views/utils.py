import sys, os, logging
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'
from django.conf            import settings
from django.template.loader import render_to_string
from django.http            import HttpResponse, HttpResponseForbidden, HttpResponseServerError
from ..thrift_communication import ThriftCommunicator
from structs.ttypes         import *
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()


def get_const(name):
    return getattr(settings, name, None)


def set_const(name, value):
    setattr(settings, name, value)


def maybe(f, t):
    if t is None:
        return None
    else:
        return f(t)


def compose(f, g):
    return lambda x: f(g(x))


def standardAjaxCall(f):
    def dec(*args):
        try:
            return f(*args)
        except InternalError:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))
        except InvalidData:
            return HttpResponseForbidden(get_const("INVALID_DATA_ERROR"))
        except DuplicateName:
            return HttpResponseForbidden(get_const("DUPLICATE_NAME_ERROR"))
        except ElementInUse:
            return HttpResponseForbidden(get_const("ELEMENT_IN_USE_ERROR"))
        except Exception as ex:
            msg = str(ex)
            logging.getLogger('django.logger').error(msg)
            printError(msg)
            return HttpResponseServerError(msg)
    return dec


def startingPage(f):
    def dec(*args):
        try:
            return f(*args)
        except Exception as ex:
            msg = str(ex)
            logging.getLogger('django.logger').error(msg)
            printError(msg)
            return HttpResponseServerError(get_const("STARTING_PAGE_ERROR"))
    return dec

def mergeQuestions(questionsList):
    mapQuestions = {
        QuestionType.SIMPLE: iter(questionsList['simpleQuestions']),
        QuestionType.MULTIPLE_CHOICE: iter(questionsList['multipleChoiceQuestions']),
        QuestionType.SORT: iter(questionsList['sortQuestions'])
    }
    return [next(mapQuestions[i]) for i in questionsList['questionsOrder']]


def markQuestionAsNew(questions, newId, newType):
    for q in questions:
        if q['questionId'] == newId and q['type'] == newType:
            q['isNew'] = True
            break

def markActionAsNew(actions, newId):
    for a in actions:
        if a['actionId'] == newId:
            a['isNew'] = True
            break

def render_to_stringNNL(name, d = {}):
    return render_to_string(name, d).replace('\n', '')


def printError(msg):
    print >>sys.stderr, "ERROR: %s" % msg
