from structs.ttypes import QuestionType
from ..views.utils  import mergeQuestions


def maybe(f, t):
    if t is None:
        return None
    else:
        return f(t)


#################################################
## Map
#################################################


def size(t):
    return {
        'width': t.width,
        'height': t.height}


def frame(t):
    return {
        'x': t.x,
        'y': t.y,
        'size': size(t.size)}


def mapFrame(t):
    return {
        'frame': frame(t.frame),
        'floor': t.floor}


def zoomLevel(t):
    return {
        'scaledSize': size(t.scaledSize),
        'tileSize': size(t.tileSize)}
        # ommiting 'tilesUrls'


def mapImage(t):
    return {
        'floor': t.floor,
        'resolution': size(t.resolution),
        'zoomLevels': [zoomLevel(z) for z in t.zoomLevels]}


#################################################
## Exhibits
#################################################


def exhibit(t):
    return {
        'exhibitId': t.exhibitId,
        'name': t.name,
        'rgbHex': _intToRgbHex(t.rgbHex),
        'mapFrame': maybe(mapFrame, t.mapFrame)}


def _intToRgbHex(val):
    return '#' + hex(val).split('x')[1].rjust(6, '0')


#################################################
## Actions
#################################################


def action(t):
    return {
        'actionId': t.actionId,
        'text': t.text}


#################################################
## Simple Question
#################################################


def simpleQuestion(t):
    return {
        'questionId': t.questionId,
        'name': t.name,
        'question': t.question,
        'type': QuestionType.SIMPLE,
        'answerType': t.answerType}


#################################################
## Multiple Choice Question
#################################################


def multipleChoiceQuestionOption(t):
    return {
        'optionId': t.optionId,
        'text': t.text}


def multipleChoiceQuestion(t):
    return {
        'questionId': t.questionId,
        'name': t.name,
        'question': t.question,
        'singleAnswer': t.singleAnswer,
        'type': QuestionType.MULTIPLE_CHOICE,
        'options': [multipleChoiceQuestionOption(o) for o in t.options]}


#################################################
## Sort Question
#################################################


def sortQuestionOption(t):
    return {
        'optionId': t.optionId,
        'text': t.text}


def sortQuestion(t):
    return {
        'questionId': t.questionId,
        'name': t.name,
        'question': t.question,
        'type': QuestionType.SORT,
        'options': [sortQuestionOption(o) for o in t.options]}


#################################################
## Questions List
#################################################


def questionsList(t):
    return {
        'questionsOrder': t.questionsOrder,
        'simpleQuestions': [simpleQuestion(q) for q in t.simpleQuestions],
        'multipleChoiceQuestions': [multipleChoiceQuestion(q) for q in t.multipleChoiceQuestions],
        'sortQuestions': [sortQuestion(q) for q in t.sortQuestions]}


#################################################
## Experiment
#################################################


def date(t):
    return {
        'day': t.day,
        'month': t.month,
        'year': t.year}


def experiment(t):
    return {
        'experimentId': t.experimentId,
        'name': t.name,
        'surveyBefore': mergeQuestions(questionsList(t.surveyBefore)),
        'exhibitActions': [action(a) for a in t.exhibitActions],
        'breakActions': [action(a) for a in t.breakActions],
        'surveyAfter': mergeQuestions(questionsList(t.surveyAfter))}


def experimentInfo(t):
    return {
        'experimentId': t.id,
        'name': t.name,
        'startDate': maybe(_dateToString, maybe(date, t.startDate)),
        'finishDate': maybe(_dateToString, maybe(date, t.finishDate))}


def _dateToString(date):
    repairNum = lambda x: "0{}".format(x) if x < 10 else "{}".format(x)
    return "{}/{}/{}".format(repairNum(date['day']), repairNum(date['month']), date['year'])


def singleExperimentInfo(t):
    return maybe(experimentInfo, t.info)


#################################################
## Report
#################################################


def reportInfo(t):
    return {
        'reportId': t.reportId,
        'receiveDate': _dateToString(date(t.receiveDate))
    }
