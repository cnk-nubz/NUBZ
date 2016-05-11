root = exports ? this
root.FinishedExperimentRow = class FinishedExperimentRow
  constructor: ->
    @_baseElement = jQuery(root.HTML.finishedExperimentRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    dateTDDOM = listElement.querySelector("td:not(:first-child):not(:last-child) > div")
    dateTDDOM.querySelector("div:first-child").innerHTML = element.startDate
    dateTDDOM.querySelector("div:last-child").innerHTML = element.finishDate
    listElement.querySelector("td:first-child > div").innerHTML = element.name
    listElement
