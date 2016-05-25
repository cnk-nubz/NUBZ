root = exports ? this
window.onerror = do ->
  timeoutId = 0
  (errorMsg, url, lineNumber) ->
    toSend =
      jsonData:
        JSON.stringify(
          url: url
          lineNumber: lineNumber
          errorMsg: errorMsg
        )
    clearTimeout timeoutId
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    timeoutId = setTimeout ( ->
      jQuery.ajax(
        type: 'POST'
        url: '/errorReporting/'
        data: toSend
        error: -> window.onerror(errorMsg, url, lineNumber)
      )), 1000
