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
    url(r'exhibit/$', views.exhibit, name='exhibit'),

    # get all the html you need
    url(r'getHTML/$', views.getHTML, name='getHTML'),
    # dialogs
    url(r'question/$', views.question, name='question'),
    url(r'action/$', views.action, name='action'),

    #errors_reporting
    url(r'errorReporting/$', views.reportError, name='reportError'),

    #experiment
    url(r'experiment/$', views.experiment, name='experiment'),
    url(r'activeExperiment/$', views.activeExperiment, name='activeExperiment'),
    url(r'cloneExperiment/$', views.cloneExperiment, name='cloneExperiment'),
    url(r'removeExperiment/$', views.removeExperiment, name='removeExperiment'),

    #reports
    url(r'getReport/$', views.getReport, name='getReport'),
    url(r'getAllReports/$', views.getAllReports, name='getAllReports')
]
