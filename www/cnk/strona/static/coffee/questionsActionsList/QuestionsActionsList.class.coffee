root = exports ? this
root.questionsActionsTable = class QuestionsActionsList extends root.View
  constructor: (@_containerId, @_lastColumnSign) ->
    super @_containerId
    @_elementsToAdd = []

  init: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      dataType: 'json'
      url: @url
      success: (data) =>
        @_listHTML = data.list
        row = jQuery(data.row)
        jQuery("i", row).addClass("fa-#{@_lastColumnSign}")
        @_listElementHTML = row.prop("outerHTML")
        @fireEvents("init")
    )

  # add whole list all at once
  replaceElements: (elements) =>
    @removeElements()
    @addElements(elements)
    @

  addElements: (elements) =>
    @_elementsToAdd.push [id, element] for id, element of elements
    @

  removeElements: =>
    @_elementsToAdd = []
    @

  show: =>
    container = jQuery(@_containerId)
    jQuery(".questionsActionsTable", container).remove()
    jQuery(@_listHTML).appendTo(@_containerId)
    @_addElementToDOM(id, element) for [id, element] in @_elementsToAdd
    @

  showLast: =>
    @_addElementToDOM(id, element) for [id, element] in @_elementsToAdd[-1..]
    @

  getAllElementsId: =>
    container = jQuery(@_containerId)
    elements = []
    jQuery(".questionsActionsTable tr", container).each( ->
      elements.push id for id, element of jQuery(this).data("rowData")
    )
    elements

  removeElement: (pos) =>
    jQuery(".questionsActionsTable tr:eq(#{pos})", @_containerId).remove()
    @_elementsToAdd.splice(pos, 1)
    @
