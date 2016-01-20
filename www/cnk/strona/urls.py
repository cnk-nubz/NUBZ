from django.conf.urls import url

from . import views

urlpatterns = [
	url(r'^$', views.index, name='index'),
	url(r'uploadImage/$', views.uploadImage, name='uploadImage'),
	url(r'updateExhibitPosition/$', views.updateExhibitPosition, name='updateExhibitPosition'),
    url(r'createNewExhibit/$', views.createNewExhibit, name='createNewExhibit'),
	url(r'badania/$', views.badania, name='badania'),
	url(r'getExhibitDialog/$', views.getExhibitDialog, name='getExhibitDialog'),
    url(r'getSimpleQuestionDialog/$', views.getSimpleQuestionDialog, name='getSimpleQuestionDialog')
]
