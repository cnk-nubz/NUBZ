from django.conf.urls import url

from . import views

urlpatterns = [
    # main pages
    url(r'^$', views.index, name='index'),
    url(r'edycja-mapy/$', views.editMapPage, name='editMapPage'),
    url(r'badania/$', views.surveys, name='surveys'),
    url(r'nowy-eksperyment/$', views.newExperimentPage, name='newExperimentPage'),
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
    url(r'getTwoColumnList/$', views.getTwoColumnList, name='getTwoColumnList'),
    url(r'getThreeColumnList/$', views.getThreeColumnList, name='getThreeColumnList'),
    # dialogs
    url(r'setSimpleQuestion/$', views.setSimpleQuestion, name='setSimpleQuestion'),
    url(r'setMultipleChoiceQuestion/$', views.setMultipleChoiceQuestion, name='setMultipleChoiceQuestion'),
    url(r'setSortQuestion/$', views.setSortQuestion, name='setSortQuestion'),
    url(r'setAction/$', views.setAction, name='setAction')
]
