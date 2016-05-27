root = exports ? this
root.FinishedExperiments = class FinishedExperiments extends root.ExperimentData
  ###
  # type FinishedExperiment = {
  #   experimentId :: Int,
  #   finishedDate :: Date,
  #   name         :: String,
  #   startDate    :: Date
  # }
  ###
  # constructor :: [FinishedExperiment] -> Context
  constructor: (_list = []) ->
    super _list


  ###
  # type FinishedExperimentListElement = {
  #   id         :: Int,
  #   name       :: String,
  #   startDate  :: Date,
  #   finishDate :: Date
  # }
  ###
  # _elementListFormat :: String -> FinishedExperimentListElement
  _elementListFormat: (viewId) =>
    id: @_elementsDict[viewId].experimentId
    name: @escapeText(@_elementsDict[viewId].name)
    startDate: @_elementsDict[viewId].startDate
    finishDate: @_elementsDict[viewId].finishDate


  # _getViewId :: Int -> String
  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].experimentId
    )
