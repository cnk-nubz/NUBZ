root = exports ? this
class Handlers
  constructor: (@canvas) ->
    @button =
      plusZoom: "#zoomControls button:first-child"
      minusZoom: "#zoomControls button:last-child"
      groundFloor: "#changeFloor button:first-child"
      firstFloor: "#changeFloor button:last-child"
      labels: "#showLabels button"
    jQuery("#{@button.labels}, #{@button.groundFloor}").addClass "active"
    jQuery(@button.minusZoom).prop "disabled", true
    @_setButtonHandlers()

  _setButtonHandlers: =>
    jQuery(@button.minusZoom).on('click', @zoomOutHandler())
    jQuery(@button.plusZoom).on('click', @zoomInHandler())
    jQuery(@button.groundFloor).on('click', @changeFloorHandler(0))
    jQuery(@button.firstFloor).on('click', @changeFloorHandler(1))
    jQuery(@button.labels).on('click', @showLabelsHandler())

  zoomOutHandler: =>
    instance = this
    ->
      jQuery(this).blur()
      result = instance.canvas.zoomOut()
      if not result
        jQuery(instance.button.minusZoom).prop "disabled", true
      else
        jQuery(instance.button.plusZoom).prop "disabled", false

  zoomInHandler: =>
    instance = this
    ->
      jQuery(this).blur()
      result = instance.canvas.zoomIn()
      if not result
        jQuery(instance.button.plusZoom).prop "disabled", true
      else
        jQuery(instance.button.minusZoom).prop "disabled", false

  showLabelsHandler: =>
    canvas = @canvas
     ->
      obj = jQuery(this)
      obj.blur()
      isActive = obj.hasClass("active")
      obj.removeClass("active") if isActive
      obj.addClass("active") unless isActive
      canvas.changeLabelVisibility(not isActive)

  changeFloorHandler: (floor) =>
    instance = this
    floorButtons = [jQuery(@button.plusZoom), jQuery(@button.minusZoom)]
    ->
      jQuery(this).blur()
      jQuery(floorButtons[1 - floor]).removeClass "active"
      jQuery(floorButtons[floor]).addClass "active"
      instance.canvas.setFloorLayer(floor)
      jQuery(instance.button.plusZoom).prop "disabled", false
      jQuery(instance.button.minusZoom).prop "disabled", true

jQuery(document).ready( ->
  canvas = new root.Canvas('#map')
  handlers = new Handlers()
)
