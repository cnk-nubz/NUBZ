from django.conf.urls import url

from . import views

urlpatterns = [
    # main pages
    url(r'^$', views.index, name='index'),
    url(r'edycja-mapy/$', views.editMapPage, name='editMapPage'),
    url(r'nowe-badanie/$', views.newExperimentPage, name='newExperimentPage'),
    # file uploads
    url(r'uploadImage/$', views.uploadImage, name='uploadImage'),
    # actions
    url(r'updateExhibitPosition/$', views.updateExhibitPosition, name='updateExhibitPosition'),
    url(r'createNewExhibit/$', views.createNewExhibit, name='createNewExhibit'),
    # get all the html you need
    url(r'getExhibitPanel/$', views.getExhibitPanel, name='getExhibitPanel'),
    url(r'getExhibitListElement/$', views.getExhibitListElement, name='getExhibitListElement'),
    url(r'getSimpleQuestionDialog/$', views.getSimpleQuestionDialog, name='getSimpleQuestionDialog'),
    url(r'getMultipleChoiceQuestionDialog/$', views.getMultipleChoiceQuestionDialog, name='getMultipleChoiceQuestionDialog'),
    url(r'getSortQuestionDialog/$', views.getSortQuestionDialog, name='getSortQuestionDialog'),
    url(r'getActionDialog/$', views.getActionDialog, name='getActionDialog'),
    url(r'getChangeMapDialog/$', views.getChangeMapDialog, name='getChangeMapDialog'),
    url(r'getChooseQuestionTypeDialog/$', views.getChooseQuestionTypeDialog, name='getChooseQuestionTypeDialog'),
    # dialogs
    url(r'createSimpleQuestion/$', views.createSimpleQuestion, name='createSimpleQuestion'),
    url(r'createMultipleChoiceQuestion/$', views.createMultipleChoiceQuestion, name='createMultipleChoiceQuestion'),
    url(r'createSortQuestion/$', views.createSortQuestion, name='createSortQuestion'),
    url(r'createAction/$', views.createAction, name='createAction'),
    #experiment
    url(r'createExperiment/$', views.createExperiment, name='createExperiment')
]
