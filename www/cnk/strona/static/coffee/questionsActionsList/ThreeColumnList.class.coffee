root = exports ? this
root.ThreeColumnList = class ThreeColumnList extends root.questionsActionsTable
  constructor: (@_containerId, @_lastColumnSign) ->
    @url = 'getThreeColumnList/'
    super

  _addElementToDOM: (id, element) =>
    listElement = jQuery(@_listElementHTML)
    jQuery("div:first", listElement).html(element.first)
    jQuery("div > span", listElement).html(element.second)
    jQuery("td:eq(0)", listElement).click( do (id) => (
      =>
        pos = jQuery(this).parent().index()
        @fireEvents("firstClicked", id, pos)
      )
    )
    jQuery("td:eq(1)", listElement).click( do (id) => (
      =>
        pos = jQuery(this).parent().index()
        @fireEvents("secondClicked", id, pos)
      )
    )
    jQuery("td:eq(2)", listElement).click( do (id) => (
      =>
        pos = jQuery(this).parent().index()
        @fireEvents("thirdClicked", id, pos)
      )
    )
    container = jQuery("#{@_containerId} .questionsActionsTable")
    rowData = {}
    rowData[id] = element
    jQuery(listElement).data("rowData", jQuery.extend({}, rowData))
    jQuery(listElement).appendTo(container)
    jQuery("div:first", listElement).shortenText()
    @fireEvents("elementAdded", listElement)
