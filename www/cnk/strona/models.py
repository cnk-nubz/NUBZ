from django.db import models

class MapUploader(models.Model):
    image = models.ImageField(
        upload_to = lambda instance, filename:
                    'map{}'.format(filename[filename.find('.'):])
    )
