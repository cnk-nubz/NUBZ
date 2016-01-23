_data = null
triedToSend = false
jQuery("#simpleQuestion").on "click", ->
  jQuery.ajaxSetup(
    headers: { "X-CSRFToken": getCookie("csrftoken") }
  )
  jQuery.ajax(
    type: 'POST'
    url: '/getSimpleQuestionDialog/'
    success: (data) ->
      _data = data.data
      BootstrapDialog.show(
        message: data.html
        title: _data.utils.text.title
        onshown: dialogCreated
        buttons: [closeButton(), saveButton()]
      )
  )

dialogCreated = ->
  radioGroup = _data.data[-1..][0][1].radioGroup
  jQuery "#dialog label.#{radioGroup}"
    .filter ":first"
    .addClass "active"

  jQuery "#dialog input[type=text]"
    .each( ->
      jQuery(this).parent().next().css("color", _data.utils.style.inputErrorColor)
      jQuery(this).keyup( (e) ->
        obj = jQuery(this)
        if e.keyCode is 8 and triedToSend and obj.val() is ""
            showInputError(obj.parent().next())
        else
          obj.parent().next().html("")
      )
    )
  return

showInputError = (obj) ->
  obj.html(_data.utils.text.inputError)
  return

closeButton = ->
  label: _data.utils.text.cancelButton
  action: (dialog) ->
    dialog.close()

saveButton = ->
  label: _data.utils.text.saveButton
  action: (dialog) ->
    triedToSend = true
    if validateForm()
      dialog.close()

validateForm = ->
  isValid = true
  jQuery "#dialog input[type=text]"
    .each( ->
      if jQuery(this).val() is ""
        isValid = false
        showInputError(jQuery(this).parent().next())
    )
  isValid
