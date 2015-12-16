from django.db import models
import os.path
class MapUploader(models.Model):
    image = models.ImageField(
        upload_to = lambda instance, filename:
                    'map{}'.format(os.path.splitext(filename)[1])
    )
