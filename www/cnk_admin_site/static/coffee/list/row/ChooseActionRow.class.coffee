root = exports ? this
root.ChooseActionRow = class ChooseActionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.structures.HTML.chooseActionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.text
    listElement
