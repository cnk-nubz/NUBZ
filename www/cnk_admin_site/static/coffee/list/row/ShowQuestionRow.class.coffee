root = exports ? this
root.ShowQuestionRow = class ShowQuestionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.structures.HTML.showQuestionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.name
    listElement.querySelector("span").innerHTML = element.typeName
    listElement
