from django.conf.urls import url

from . import views
from views import mainPage, mapControl, experimentData, manageExperiment, manageReports, manageErrors

urlpatterns = [
    # main pages
    url(r'^$', mainPage.index, name='index'),
    url(r'edycja-mapy/$', mainPage.editMapPage, name='editMapPage'),
    url(r'badanie/$', mainPage.newExperimentPage, name='newExperimentPage'),
    url(r'badania/$', mainPage.experimentsPage, name='experimentsPage'),
    url(r'pytania-i-akcje/$', mainPage.questionsAndActionsPage, name='questionsAndActionsPage'),
    url(r'android/$', mainPage.androidApp, name='androidApp'),

    # # map control
    url(r'uploadImage/$', mapControl.uploadImage, name='uploadImage'),
    url(r'removeFloor/$', mapControl.removeFloor, name='removeFloor'),
    url(r'updateExhibitPosition/$', mapControl.updateExhibitPosition, name='updateExhibitPosition'),
    url(r'exhibit/$', mapControl.exhibit, name='exhibit'),
    #
    # # experiment data
    url(r'question/$', experimentData.question, name='question'),
    url(r'action/$', experimentData.action, name='action'),
    #
    #
    #experiment
    url(r'experiment/$', manageExperiment.experiment, name='experiment'),
    url(r'activeExperiment/$', manageExperiment.activeExperiment, name='activeExperiment'),
    url(r'cloneExperiment/$', manageExperiment.cloneExperiment, name='cloneExperiment'),
    url(r'removeExperiment/$', manageExperiment.removeExperiment, name='removeExperiment'),

    #reports
    url(r'getReport/$', manageReports.getReport, name='getReport'),
    url(r'getAllReports/$', manageReports.getAllReports, name='getAllReports'),

    #errors_reporting
    url(r'errorReporting/$', manageErrors.reportError, name='reportError'),
]
