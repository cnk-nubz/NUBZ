from django.conf.urls import url

from . import views

urlpatterns = [
	url(r'^$', views.index, name='index'),
	url(r'uploadImage/$', views.uploadImage, name='uploadImage'),
	url(r'updateExhibitPosition/$', views.updateExhibitPosition, name='updateExhibitPosition'),
    url(r'createNewExhibit/$', views.createNewExhibit, name='createNewExhibit'),
	url(r'badania/$', views.surveys, name='surveys'),
    url(r'getSimpleQuestionDialog/$', views.getSimpleQuestionDialog, name='getSimpleQuestionDialog'),
    url(r'getMultipleChoiceQuestionDialog/$', views.getMultipleChoiceQuestionDialog, name='getMultipleChoiceQuestionDialog'),
    url(r'getExhibitPanel/$', views.getExhibitPanel, name='getExhibitPanel'),
    url(r'getExhibitListElement/$', views.getExhibitListElement, name='getExhibitListElement')
]
