root = exports ? this
root.CloneExperimentDialog = class CloneExperimentDialog extends root.QuestionDialog
  constructor: (dataDialog, options = {}) ->
    super(dataDialog, options)


  # type CloneExperimentData = {
  #   name :: String
  # }
  extractData: ->
    name = jQuery("#dialog input").val()
    data =
      name: name
    data
