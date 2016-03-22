from django.conf.urls import url

from . import views

urlpatterns = [
    # main pages
    url(r'^$', views.index, name='index'),
    url(r'edycja-mapy/$', views.editMapPage, name='editMapPage'),
    url(r'nowe-badanie/$', views.newExperimentPage, name='newExperimentPage'),
    url(r'pytania-i-akcje/$', views.questionsAndActionsPage, name='questionsAndActionsPage'),
    url(r'android/$', views.androidApp, name='androidApp'),

    url(r'uploadImage/$', views.uploadImage, name='uploadImage'),

    url(r'updateExhibitPosition/$', views.updateExhibitPosition, name='updateExhibitPosition'),
    url(r'createNewExhibit/$', views.createNewExhibit, name='createNewExhibit'),
    url(r'updateExhibit/$', views.updateExhibit, name='updateExhibit'),

    # get all the html you need
    url(r'getHTML/$', views.getHTML, name='getHTML'),
    # dialogs
    url(r'createSimpleQuestion/$', views.createSimpleQuestion, name='createSimpleQuestion'),
    url(r'createMultipleChoiceQuestion/$', views.createMultipleChoiceQuestion, name='createMultipleChoiceQuestion'),
    url(r'createSortQuestion/$', views.createSortQuestion, name='createSortQuestion'),
    url(r'createAction/$', views.createAction, name='createAction'),
    #experiment
    url(r'createExperiment/$', views.createExperiment, name='createExperiment'),

    #errors_reporting
    url(r'errorReporting/$', views.reportError, name='reportError'),
]
