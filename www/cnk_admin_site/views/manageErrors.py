import json, os, logging
from django.http import HttpResponse


def reportError(request):
    data = json.loads(request.POST.get("jsonData"))
    logger = logging.getLogger('jsLogger')
    logger.error(
        ": {0} in {1}, {2}".format(
            data['url'],
            data['lineNumber'],
            data['errorMsg']))
    return HttpResponse()
