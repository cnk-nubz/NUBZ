root = exports ? this
root.ListView = class ListView extends root.View
  constructor: (@_containerId, @_rowFactory) ->
    super()
    @_rowFactory.on("allEvents", (event, id, pos) => @fireEvents(event, id, pos))
    @_tableListHTML = root.HTML.tableList
    @_elementsOnList = []
    @_newElementsId = {}

  # add whole list all at once
  replaceElements: (elements) =>
    @removeElements()
    @addElements(elements)
    @

  addElements: (elements) =>
    for orderId, element of elements
      row = @_rowFactory.generateRow orderId, element
      id = @generateId(element) #function to implement in subclasses
      if element.isNew or (@_newElementsId[id]? and @_newElementsId[id])
        @_newElementsId[id] = true
        instance = this
        row.addClass('newListEntry')
          .one('click', do (id, row) ->
            ->
              # remove looks, remove from newElements and set as not new
              jQuery(this).removeClass("newListEntry")
              row.removeClass("newListEntry")
              rowData = row.data("rowData")
              rowDataId = Object.keys(rowData)[0]
              rowData[rowDataId].isNew = false
              row.data("rowData", rowData)
              instance._newElementsId[id] = false
          )
        @_newElementsId[id] = true
      @_elementsOnList.push row
    @

  removeElements: =>
    @_elementsOnList = []
    @

  show: =>
    container = jQuery(@_containerId)
    jQuery(".questionsActionsTable", container).remove()
    jQuery(@_tableListHTML).appendTo(@_containerId)
    for row in @_elementsOnList
      list = jQuery(".questionsActionsTable", container)
      rowClone = jQuery(row).clone(true, true)
      rowClone = rowClone.appendTo(list)
      jQuery("td:eq(0) > div", rowClone).shortenText()
      @fireEvents("elementAdded", rowClone)
    @

  showLast: =>
    container = jQuery(@_containerId)
    for row in @_elementsOnList[-1..]
      list = jQuery(".questionsActionsTable", container)
      rowClone = jQuery(row).clone(true, true)
      rowClone = jQuery(rowClone).appendTo(list)
      jQuery("td:eq(0) > div", rowClone).shortenText()
      @fireEvents("elementAdded", rowClone)
    @

  getAllElementsId: =>
    elements = []
    jQuery(".questionsActionsTable tr", @_containerId).each( ->
      elements.push id for id, _ of jQuery(this).data("rowData")
    )
    elements

  removeElement: (pos) =>
    jQuery(".questionsActionsTable tr:eq(#{pos})", @_containerId).remove()
    @_elementsOnList.splice(pos, 1)
    @

  isElementOnList: (id) =>
    for element in @_elementsOnList
      elementId = tmpId for tmpId, _ of element.data("rowData")
      if id is elementId
        return true
    false
