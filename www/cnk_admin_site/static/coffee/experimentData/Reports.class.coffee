root = exports ? this
root.Reports = class Reports extends root.ExperimentData
  ###
  # type Report = {
  #   reportId    :: Int,
  #   receiveDate :: Date
  # }
  ###
  # constructor :: [Report] -> Context
  constructor: (_list = []) ->
    super _list

  ###
  # type ReportListElement = {
  #   id          :: Int
  #   receiveDate :: Date
  # }
  ###
  # _elementListFormat :: String -> ReportListElement
  _elementListFormat: (viewId) =>
    id: @_elementsDict[viewId].reportId
    receiveDate: @_elementsDict[viewId].receiveDate


  # _getViewId :: Int -> String
  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].reportId
    )
