root = exports ? this
root.FinishedExperimentRow = class FinishedExperimentRow
  constructor: ->
    @_baseElement = jQuery(root.HTML.finishedExperimentRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    repairNum = (x) -> if 1 <= x <= 9 then "0#{x}" else "#{x}"
    startDate = "#{repairNum element.startDate.day}/#{repairNum element.startDate.month}/#{element.startDate.year}"
    finishDate = "#{repairNum element.finishDate.day}/#{repairNum element.finishDate.month}/#{element.finishDate.year}"
    listElement.querySelector("td:last-child > div:last-child > div:first-child").innerHTML = startDate
    listElement.querySelector("td:last-child > div:last-child > div:last-child").innerHTML = finishDate
    listElement.querySelector("td:first-child > div").innerHTML = element.name
    listElement
