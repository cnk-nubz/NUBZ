root = exports ? this
root.RaportRow = class RaportRow
  constructor: ->
    @_baseElement = jQuery(root.HTML.raportRow)[0]

  generateRow: (element) =>
    listElement = @_baseElement.cloneNode(true)
    repairNum = (x) -> if 1 <= x <= 9 then "0#{x}" else "#{x}"
    date = "#{repairNum element.date.day}/#{repairNum element.date.month}/#{element.date.year}"
    listElement.querySelector("td:first-child > span").innerHTML = element.name
    listElement.querySelector("td:last-child > span").innerHTML = date
    listElement
