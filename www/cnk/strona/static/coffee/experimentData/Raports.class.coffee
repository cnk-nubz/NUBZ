root = exports ? this
root.Raports = class Raports extends root.ExperimentData
  constructor: -> super

  _elementListFormat: (viewId) =>
    id: @_elementsDict[viewId].raportId
    name: @escapeText(@_elementsDict[viewId].name)
    date: @_elementsDict[viewId].date

  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].raportId
    )
