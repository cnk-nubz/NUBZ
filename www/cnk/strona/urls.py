from django.conf.urls import url

from . import views

urlpatterns = [
	url(r'^$/(?P<err>[0-4]{1})/(?P<floor>[0-1]{1})$', views.index, name='index'),
	url(r'^$', views.index, name='index'),
	url(r'uploadImage/$', views.uploadImage, name='uploadImage'),
]
