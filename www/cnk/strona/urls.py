from django.conf.urls import url

from . import views

urlpatterns = [
    # main pages
    url(r'^$', views.index, name='index'),
    url(r'edycja-mapy/$', views.editMapPage, name='editMapPage'),
    url(r'badanie/$', views.newExperimentPage, name='newExperimentPage'),
    url(r'pytania-i-akcje/$', views.questionsAndActionsPage, name='questionsAndActionsPage'),
    url(r'badania/$', views.experimentsPage, name='experimentsPage'),
    url(r'android/$', views.androidApp, name='androidApp'),

    # map control
    url(r'uploadImage/$', views.uploadImage, name='uploadImage'),
    url(r'removeFloor/$', views.removeFloor, name='removeFloor'),

    url(r'updateExhibitPosition/$', views.updateExhibitPosition, name='updateExhibitPosition'),
    url(r'createNewExhibit/$', views.createNewExhibit, name='createNewExhibit'),
    url(r'getAllExhibits/$', views.getAllExhibits, name='getAllExhibits'),
    url(r'updateExhibit/$', views.updateExhibit, name='updateExhibit'),

    # get all the html you need
    url(r'getHTML/$', views.getHTML, name='getHTML'),
    # dialogs
    url(r'createSimpleQuestion/$', views.createSimpleQuestion, name='createSimpleQuestion'),
    url(r'createMultipleChoiceQuestion/$', views.createMultipleChoiceQuestion, name='createMultipleChoiceQuestion'),
    url(r'createSortQuestion/$', views.createSortQuestion, name='createSortQuestion'),
    url(r'createAction/$', views.createAction, name='createAction'),

    #errors_reporting
    url(r'errorReporting/$', views.reportError, name='reportError'),

    #experiment
    url(r'saveExperiment/$', views.saveExperiment, name='saveExperiment'),
    url(r'startExperiment/$', views.startExperiment, name='startExperiment'),
    url(r'finishExperiment/$', views.finishExperiment, name='finishExperiment'),
    url(r'cloneExperiment/$', views.cloneExperiment, name='cloneExperiment'),

    #reports
    url(r'getReport/$', views.getReport, name='getReport'),
    url(r'getAllReports/$', views.getAllReports, name='getAllReports')
]
