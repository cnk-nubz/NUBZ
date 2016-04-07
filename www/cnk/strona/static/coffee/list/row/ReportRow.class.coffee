root = exports ? this
root.ReportRow = class ReportRow
  constructor: ->
    @_baseElement = jQuery(root.HTML.reportRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    repairNum = (x) -> if 1 <= x <= 9 then "0#{x}" else "#{x}"
    date = "#{repairNum element.receiveDate.day}/#{repairNum element.receiveDate.month}/#{element.receiveDate.year}"
    listElement.querySelector("td:first-child > span").innerHTML += "#{element.id}"
    listElement.querySelector("td:last-child > span").innerHTML = date
    listElement
