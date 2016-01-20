jQuery("button").on "click", ->
  jQuery.ajaxSetup(
    headers: { "X-CSRFToken": getCookie("csrftoken") }
  )
  jQuery.ajax(
    type: 'POST'
    url: '/getSimpleQuestionDialog/'
    success: (data) ->
      BootstrapDialog.show(
        message: data
        title: "Tworzenie prostego pytania"
        buttons: [closeButton(), saveButton()]
      )
  )

closeButton = ->
  label: "Zamknij"
  action: (dialog) ->
    dialog.close()

saveButton = ->
  label: "Wyślij"
  action: (dialog) ->
    if validateForm()
      dialog.close()

validateForm = ->
  true
