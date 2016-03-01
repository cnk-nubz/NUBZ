root = exports ? this
root.ExperimentQuestionRow = class ExperimentQuestionRow extends root.View
  constructor: ->
    super()
    @_rowData = root.HTML.experimentQuestionRow

  generateRow: (id, element) =>
    listElement = jQuery(@_rowData)
    instance = this
    jQuery("td:eq(0) > div", listElement).html(element.name)
    jQuery("td:eq(0)", listElement)
      .click( do (id) =>
        ->
          pos = jQuery(this).parent().index()
          instance.fireEvents("allEvents", "nameClicked", id, pos)
      )
    jQuery("td:eq(1) span", listElement).html(element.typeName)
    jQuery("td:eq(2)", listElement)
      .click( do (id) =>
        ->
          pos = jQuery(this).parent().index()
          instance.fireEvents("allEvents", "removeFromExperiment", id, pos)
      )
    rowData = {}
    rowData[id] = element
    jQuery(listElement).data("rowData", jQuery.extend({}, rowData))
