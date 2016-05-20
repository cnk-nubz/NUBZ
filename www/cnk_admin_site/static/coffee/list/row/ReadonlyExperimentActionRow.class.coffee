root = exports ? this
root.ReadonlyExperimentActionRow = class ReadonlyExperimentActionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.structures.HTML.readonlyExperimentActionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.text
    listElement
