root = exports ? this
root.FinishedExperimentRow = class FinishedExperimentRow
  constructor: ->
    @_baseElement = jQuery(root.HTML.finishedExperimentRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    repairNum = (x) -> if 1 <= x <= 9 then "0#{x}" else "#{x}"
    startDate = "#{repairNum element.startDate.day}/#{repairNum element.startDate.month}/#{element.startDate.year}"
    finishDate = "#{repairNum element.finishDate.day}/#{repairNum element.finishDate.month}/#{element.finishDate.year}"
    dateTDDOM = listElement.querySelector("td:not(:first-child):not(:last-child) > div")
    dateTDDOM.querySelector("div:first-child").innerHTML = startDate
    dateTDDOM.querySelector("div:last-child").innerHTML = finishDate
    listElement.querySelector("td:first-child > div").innerHTML = element.name
    listElement
