root = exports ? this
root.ReadonlyExperimentQuestionRow = class ReadonlyExperimentQuestionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.HTML.readonlyExperimentQuestionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.name
    listElement.querySelector("span").innerHTML = element.typeName
    listElement
