root = exports ? this
root.ReadyExperiments = class ReadyExperiments extends root.ExperimentData
  ###
  # type ReadyExperiment = {
  #   experimentId :: Int,
  #   name         :: String,
  #   startDate    :: Date
  # }
  ###
  # constructor :: [ReadyExperiment] -> Context
  constructor: (_list = []) ->
    super _list

  ###
  # ReadyExperimentListElement = {
  #   id :: Int,
  #   name :: String
  # }
  ###
  # _elementListFormat :: String -> ReadyExperimentListElement
  _elementListFormat: (viewId) =>
    id: @_elementsDict[viewId].experimentId
    name: @escapeText(@_elementsDict[viewId].name)


  # _getViewId :: Int -> String
  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].experimentId
    )
