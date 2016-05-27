root = exports ? this
root.ShowActionRow = class ShowActionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.structures.HTML.showActionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.text
    listElement
