root = exports ? this
root.CloneExperimentDialog = class CloneExperimentDialog extends root.QuestionDialog
  constructor: (url = 'getHTML?name=cloneExperimentDialog', options = {}) ->
    super(url, options)


  # type CloneExperimentData = {
  #   name :: String
  # }
  extractData: ->
    name = jQuery("#dialog input").val()
    data =
      name: name
    data
