root = exports ? this
root.ExperimentData = class ExperimentData
  constructor: (@_list = []) ->
    @_init()
    @_newElements = {}

  _init: => @_processElements()

  setElements: (elements) =>
    @_list = elements
    @_processElements()
    @

  _processElements: =>
    @_elements = {}
    for element, index in @_list
      @_elements[@getRealId(index)] = element
    @

  getAllElementsAsDOM: (rowFactory) =>
    fragment = document.createDocumentFragment()
    for key in Object.keys(@_list)
      # iterate over @_list to keep the order
      realId = @getRealId(key)
      listFormat = @_elementListFormat(realId)
      row = rowFactory.generateRow(listFormat)
      row.data = realId
      if listFormat.isNew or (@_newElements[realId])
        @_newElements[realId] = true
        instance = this
        jQuery(row).addClass('newListEntry')
          .one('click', ->
            jQuery(this).removeClass('newListEntry')
            instance._newElements[realId] = false
          )
      fragment.appendChild(row)
    fragment

  getElementAsDOM: (id, rowFactory) =>
    fragment = document.createDocumentFragment()
    row = rowFactory.generateRow(@_elementListFormat(id))
    row.data = id
    fragment.appendChild(row)
    fragment
