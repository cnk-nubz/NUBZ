root = exports ? this
root.QuestionsActionsList = class QuestionsActionsList extends root.View
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
    jQuery(".myTable", container).remove()
    jQuery(@_listHTML).appendTo(@_containerId)
    @_addElementToDOM(id, element) for [id, element] in @_elementsToAdd
    @
