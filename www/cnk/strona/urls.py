from django.conf.urls import url

from . import views

urlpatterns = [
	url(r'^$', views.index, name='index'),
	url(r'uploadImage/$', views.uploadImage, name='uploadImage'),
	url(r'updateExhibitPosition/$', views.updateExhibitPosition, name='updateExhibitPosition')
]
