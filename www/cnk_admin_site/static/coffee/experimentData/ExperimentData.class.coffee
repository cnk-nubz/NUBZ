root = exports ? this
root.ExperimentData = class ExperimentData
  # ======== ABSTRACT FUNCTIONS ========
  # _getViewId         :: Int -> String
  # _elementListFormat :: String -> ListElement
  # ====================================
  ###
  # ListData =
  #     Action
  #   | Question
  #   | FinishedExperiment
  #   | ReadyExperiment
  #   | Report
  ###
  ###
  # ListElement =
  #   ActionListElement
  # | QuestionListElement
  # | FinishedExperimentListElement
  # | ReadyExperimentListElement
  # | ReportListElement
  ###
  constructor: (_list) ->
    # _list :: [ListData]
    @setElements(_list)
    @_newElements = {}


  # setElements :: [ListElement] -> Context
  setElements: (elements) =>
    @_orderedList = elements
    @_elementsDict = {}
    for element, index in @_orderedList
      @_elementsDict[@_getViewId(index)] = element
    @


  # getAllElementsAsDOM :: TableRow -> DocumentFragment
  getAllElementsAsDOM: (rowFactory) =>
    fragment = document.createDocumentFragment()
    for key in Object.keys(@_orderedList)
      viewId = @_getViewId(key)
      # listElement :: ListElement
      listFormat = @_elementListFormat(viewId)
      row = rowFactory.generateRow(listFormat)
      row.data = viewId
      if listFormat.isNew or (@_newElements[viewId])
        @_newElements[viewId] = true
        instance = this
        jQuery(row).addClass('newListEntry')
          .one('click', do (viewId) ->
            ->
              jQuery(this).removeClass('newListEntry')
              instance._newElements[viewId] = false
          )
      fragment.appendChild(row)
    fragment


  # getElementAsDOM :: (String, TableRow) -> DocumentFragment
  getElementAsDOM: (viewId, rowFactory) =>
    fragment = document.createDocumentFragment()
    row = rowFactory.generateRow(@_elementListFormat(viewId))
    row.data = viewId
    fragment.appendChild(row)
    fragment


  # escapeText :: String -> String
  escapeText: (text) ->
    jQuery("<div>").text(text).html()
