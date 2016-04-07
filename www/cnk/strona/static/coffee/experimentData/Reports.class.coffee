root = exports ? this
root.Reports = class Reports extends root.ExperimentData
  constructor: -> super

  _elementListFormat: (viewId) =>
    id: @_elementsDict[viewId].reportId
    receiveDate: @_elementsDict[viewId].receiveDate

  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].reportId
    )
