root = exports ? this
root.ReadyExperiments = class ReadyExperiments extends root.ExperimentData
  constructor: (_list = []) ->
    super _list

  _elementListFormat: (viewId) =>
    id: @_elementsDict[viewId].experimentId
    name: @escapeText(@_elementsDict[viewId].name)

  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].experimentId
    )
