root = exports ? this
root.FinishedExperiments = class FinishedExperiments extends root.ExperimentData
  constructor: (_list = []) ->
    super _list

  _elementListFormat: (viewId) =>
      id: @_elementsDict[viewId].experimentId
      name: @_elementsDict[viewId].name
      startDate: @_elementsDict[viewId].startDate
      finishDate: @_elementsDict[viewId].finishDate

  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].experimentId
    )
