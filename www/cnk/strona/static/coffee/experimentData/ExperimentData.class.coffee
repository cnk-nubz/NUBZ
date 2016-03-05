root = exports ? this
root.ExperimentData = class ExperimentData
  # functions to implement by subclasses:
  # _getViewId(index)
  # _elementListFormat(id)
  constructor: (@_orderedList) ->
    @_init()
    @_newElements = {}

  _init: => @_processElements()

  setElements: (elements) =>
    @_orderedList = elements
    @_processElements()
    @

  _processElements: =>
    @_elementsDict = {}
    for element, index in @_orderedList
      @_elementsDict[@_getViewId(index)] = element
    @

  getAllElementsAsDOM: (rowFactory) =>
    fragment = document.createDocumentFragment()
    for key in Object.keys(@_orderedList)
      viewId = @_getViewId(key)
      listFormat = @_elementListFormat(viewId)
      row = rowFactory.generateRow(listFormat)
      row.data = viewId
      if listFormat.isNew or (@_newElements[viewId])
        @_newElements[viewId] = true
        instance = this
        jQuery(row).addClass('newListEntry')
          .one('click', ->
            jQuery(this).removeClass('newListEntry')
            instance._newElements[viewId] = false
          )
      fragment.appendChild(row)
    fragment

  getElementAsDOM: (viewId, rowFactory) =>
    fragment = document.createDocumentFragment()
    row = rowFactory.generateRow(@_elementListFormat(viewId))
    row.data = viewId
    fragment.appendChild(row)
    fragment
