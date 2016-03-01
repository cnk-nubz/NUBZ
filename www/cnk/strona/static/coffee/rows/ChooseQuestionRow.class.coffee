root = exports ? this
root.ChooseQuestionRow = class ChooseQuestionRow extends root.View
  constructor: ->
    super()
    @_rowData = root.HTML.chooseQuestionRow

  generateRow: (id, element) =>
    listElement = jQuery(@_rowData)
    instance = this
    jQuery("td:eq(0) > div", listElement).html(element.name)
    jQuery("div:eq(1) > span", listElement).html(element.typeName)
    jQuery("td:eq(0)", listElement).click( do (id) => (
      ->
        pos = jQuery(this).parent().index()
        instance.fireEvents("allEvents", "nameClicked", id, pos)
      )
    )
    rowData = {}
    rowData[id] = element
    jQuery(listElement).data("rowData", jQuery.extend({}, rowData))
