root = exports ? this
class Logging
  constructor: () ->
  logError: (errorMsg, url, lineNumber) =>
    instance = this
    toSend =
      jsonData:
        JSON.stringify(
          url: url
          lineNumber: lineNumber
          errorMsg: errorMsg
        )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      url: '/errorReporting/'
      data: toSend
      error: () ->
        setTimeout ( -> instance.logError(errorMsg, url, lineNumber)), 1000
    )

logger = new Logging()
window.onerror = logger.logError
