root = exports ? this
root.ExhibitPanel = class ExhibitPanel extends root.View
  constructor: ->
    super
    @_init()

  _init: =>
    addExhibitButtonStyle = {
      "height": "35px"
      "width": "100%"
    }
    @select(@_containerId)
      .append "div"
      .style(
        "padding": "5px 10px 5px 10px"
      )
      .append "button"
      .style(addExhibitButtonStyle)
      .classed "leaflet-bar", true
      .text "Dodaj eksponat"
      .on("click", => @fireEvents("addExhibit"))
