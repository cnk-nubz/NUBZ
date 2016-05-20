root = exports ? this
root.ReadyExperimentRow = class ReadyExperimentRow
  constructor: ->
    @_baseElement = jQuery(root.structures.HTML.readyExperimentRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.name
    listElement
