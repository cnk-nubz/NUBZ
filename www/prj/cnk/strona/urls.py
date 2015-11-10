from django.conf.urls import url

from . import views

urlpatterns = [
	url(r'pingpong/$', views.pingpong, name='pingpong'),
]