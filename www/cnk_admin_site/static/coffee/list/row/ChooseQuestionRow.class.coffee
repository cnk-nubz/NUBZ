root = exports ? this
root.ChooseQuestionRow = class ChooseQuestionRow extends root.View
  constructor: ->
    super()
    @_baseElement = jQuery(root.structures.HTML.chooseQuestionRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("div").innerHTML = element.name
    listElement.querySelector("span").innerHTML = element.typeName
    listElement
