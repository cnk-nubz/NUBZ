root = exports ? this
root.ExperimentActionRow = class ExperimentActionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.HTML.experimentActionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.text
    listElement
