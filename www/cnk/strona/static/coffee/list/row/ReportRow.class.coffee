root = exports ? this
root.ReportRow = class ReportRow
  constructor: ->
    @_baseElement = jQuery(root.HTML.reportRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    listElement.querySelector("td:first-child > span").innerHTML += "#{element.id}"
    listElement.querySelector("td:last-child > span").innerHTML = element.receiveDate
    listElement
