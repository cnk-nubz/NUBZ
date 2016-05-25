root = exports ? this
root.ExperimentQuestionRow = class ExperimentQuestionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.structures.HTML.experimentQuestionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.name
    listElement.querySelector("span").innerHTML = element.typeName
    listElement
