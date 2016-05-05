from django.db import models
import os.path

def getFilePath(instance, filename):
    return 'map{}'.format(os.path.splitext(filename)[1])
class MapUploader(models.Model):
    image = models.ImageField(upload_to = getFilePath)
