root = exports ? this
root.TwoColumnList = class TwoColumnList extends root.QuestionsActionsList
  constructor: (@_containerId, @_lastColumnSign) ->
    @url = 'getTwoColumnList/'
    super

  _addElementToDOM: (id, element) =>
    listElement = jQuery(@_listElementHTML)
    instance = this
    jQuery("div:eq(0)", listElement).html(element.first).shortenText()
    jQuery("td:eq(0)", listElement).click( do (id) => (
      ->
        poss = jQuery(this).parent().index()
        instance.fireEvents("firstClicked", id, poss)
      )
    )
    jQuery("td:eq(1)", listElement).click( do (id) => (
      ->
        poss = jQuery(this).parent().index()
        instance.fireEvents("secondClicked", id, poss)
      )
    )
    container = jQuery("#{@_containerId} .myTable")
    rowData = {}
    rowData[id] = element
    jQuery(listElement).data("rowData", jQuery.extend({}, rowData))
    jQuery(listElement).appendTo(container)
    @fireEvents("elementAdded", listElement)

  removeElement: (pos) =>
    jQuery(".myTable tr:eq(#{pos})", @_containerId).remove()
    @_elementsToAdd.splice(pos, 1)
    @
