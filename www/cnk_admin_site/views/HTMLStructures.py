from utils import get_const, render_to_stringNNL


def getMapPage():
    return {
        'dialog': {
            'exhibit': _getExhibitDialog(),
            'changeMap': _getChangeMapDialog()
        },
        'HTML': {
            'exhibitListElement':
                render_to_stringNNL('exhibitPanel/exhibitListElement.html')
        }
    }



def experimentsPage():
    return {
        'dialog': {
            'startExperimentConfirmation': get_const("START_EXPERIMENT_CONFIRMATION"),
            'finishExperimentConfirmation': get_const("FINISH_EXPERIMENT_CONFIRMATION"),
            'cloneExperiment': _getDialog("CLONE_EXPERIMENT_DIALOG")
        },
        'HTML': {
            'finishedExperimentRow': render_to_stringNNL('list/row/finishedExperimentRow.html'),
            'readyExperimentRow': render_to_stringNNL('list/row/readyExperimentRow.html'),
            'tableList': render_to_stringNNL('list/dataList.html')
        }
    }


def newExperimentPage():
    return {
        'dialog': {
            'removeExperimentConfirmation': get_const("REMOVE_EXPERIMENT_CONFIRMATION"),
            'question': _getQuestionDialogs(),
            'action': _getDialog("ACTION_DIALOG")

        },
        'HTML': {
            'chooseQuestionType': render_to_stringNNL('dialog/chooseQuestionType.html'),
            'chooseQuestionRow': render_to_stringNNL('list/row/chooseQuestionRow.html'),
            'chooseActionRow': render_to_stringNNL('list/row/chooseActionRow.html'),
            'experimentActionRow': render_to_stringNNL('list/row/experimentActionRow.html'),
            'experimentQuestionRow': render_to_stringNNL('list/row/experimentQuestionRow.html'),
            'tableList': render_to_stringNNL('list/dataList.html')
        }
    }


def newExperimentReadonlyPage():
    return {
        'dialog': {
            'removeExperimentConfirmation': get_const("REMOVE_EXPERIMENT_CONFIRMATION"),
            'question': _getQuestionDialogs(),
            'action': _getDialog("ACTION_DIALOG")
        },
        'HTML': {
            'readonlyExperimentActionRow': render_to_stringNNL('list/row/readonlyExperimentActionRow.html'),
            'readonlyExperimentQuestionRow': render_to_stringNNL('list/row/readonlyExperimentQuestionRow.html'),
            'reportRow': render_to_stringNNL('list/row/reportRow.html'),
            'tableList': render_to_stringNNL('list/dataList.html')
        }
    }

def questionsAndActionsPage():
    return {
        'dialog': {
            'question': _getQuestionDialogs(),
            'action': _getDialog("ACTION_DIALOG"),
        },
        'HTML': {
            'chooseQuestionType': render_to_stringNNL('dialog/chooseQuestionType.html'),
            'showQuestionRow': render_to_stringNNL('list/row/showQuestionRow.html'),
            'showActionRow': render_to_stringNNL('list/row/showActionRow.html'),
            'tableList': render_to_stringNNL('list/dataList.html')
        }
    }


def _getQuestionDialogs():
    return {
        'simple': _getDialog("SIMPLE_QUESTION_DIALOG"),
        'sort': _getDialog("SORT_QUESTION_DIALOG"),
        'multipleChoice': _getDialog("MULTIPLE_CHOICE_QUESTION_DIALOG")
    }


def _getExhibitDialog():
    dialogStructure = get_const("EXHIBIT_DIALOG")
    availableFloors = get_const("AVAILABLE_FLOORS")[:]
    availableFloors.append("brak")
    dialogStructure['data'][1][1]['textList'] = availableFloors
    contextDict = {
        'data': dialogStructure['data']
    }
    return {
        'data': dialogStructure,
        'html': render_to_stringNNL('dialog/dialog.html', contextDict)
    }

def _getDialog(dialogName):
    dialogStructure = get_const(dialogName)
    contextDict = {
        'data': dialogStructure['data']
    }
    return {
        'data': dialogStructure,
        'html': render_to_stringNNL('dialog/dialog.html', contextDict)
    }


def _getChangeMapDialog():
    return {
            'addFloor': render_to_stringNNL('dialog/changeMap/addFloor.html'),
            'removeFloor': render_to_stringNNL('dialog/changeMap/removeFloor.html'),
            'processingMap': render_to_stringNNL('dialog/changeMap/processingMap.html'),
            'chooseMapChangeAction': render_to_stringNNL('dialog/changeMap/chooseMapChangeAction.html'),
            'changeImage': render_to_stringNNL('dialog/changeMap/changeImage.html')}
