from django.conf.urls import url

from . import views

urlpatterns = [
	url(r'^$', views.index, name='index'),
    url(r'edycja-mapy/$', views.editMapPage, name='editMapPage'),
    url(r'badania/$', views.surveys, name='surveys'),
    url(r'pytania-i-akcje/$', views.questionsAndActionsPage, name='questionsAndActionsPage'),

	url(r'uploadImage/$', views.uploadImage, name='uploadImage'),

	url(r'updateExhibitPosition/$', views.updateExhibitPosition, name='updateExhibitPosition'),
    url(r'createNewExhibit/$', views.createNewExhibit, name='createNewExhibit'),

    url(r'getExhibitPanel/$', views.getExhibitPanel, name='getExhibitPanel'),
    url(r'getExhibitListElement/$', views.getExhibitListElement, name='getExhibitListElement'),
    url(r'getSimpleQuestionDialog/$', views.getSimpleQuestionDialog, name='getSimpleQuestionDialog'),
    url(r'getMultipleChoiceQuestionDialog/$', views.getMultipleChoiceQuestionDialog, name='getMultipleChoiceQuestionDialog'),
    url(r'getSortQuestionDialog/$', views.getSortQuestionDialog, name='getSortQuestionDialog'),
    url(r'getNewActionDialog/$', views.getNewActionDialog, name='getNewActionDialog'),
    url(r'getChangeMapDialog/$', views.getChangeMapDialog, name='getChangeMapDialog'),
    url(r'getChooseQuestionTypeDialog/$', views.getChooseQuestionTypeDialog, name='getChooseQuestionTypeDialog'),

    url(r'setSimpleQuestion/$', views.setSimpleQuestion, name='setSimpleQuestion'),
    url(r'setSortQuestion/$', views.setSortQuestion, name='setSortQuestion'),
    url(r'setMultipleChoiceQuestion/$', views.setMultipleChoiceQuestion, name='setMultipleChoiceQuestion'),
    url(r'setAction/$', views.setAction, name='setAction')
]
