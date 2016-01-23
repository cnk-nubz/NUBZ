_data = null
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
  jQuery "label.#{radioGroup}"
    .filter ":first"
    .addClass "active"

closeButton = ->
  label: _data.utils.text.cancelButton
  action: (dialog) ->
    dialog.close()

saveButton = ->
  label: _data.utils.text.saveButton
  action: (dialog) ->
    if validateForm()
      dialog.close()

validateForm = ->
  isValid = true
  jQuery "#dialog input[type=text]"
    .each( ->
      isValid = jQuery(this).val() isnt ""
    )
  isValid
