root = exports ? this
root.ChooseActionRow = class ChooseActionRow extends root.View
  constructor: ->
    super()
    @_rowData = root.HTML.chooseActionRow

  generateRow: (id, element) =>
    listElement = jQuery(@_rowData)
    instance = this
    jQuery("td:eq(0) > div", listElement).html(element.text)
    jQuery("td:eq(0)", listElement)
      .click( do (id) =>
        ->
          pos = jQuery(this).parent().index()
          instance.fireEvents("allEvents", "nameClicked", id, pos)
      )
    rowData = {}
    rowData[id] = element
    jQuery(listElement).data("rowData", jQuery.extend({}, rowData))
