root = exports ? this
root.Navbar = class Navbar
  constructor: ->

  getJustMap: (canvas) ->
    jQuery "#imageUploadForm"
      .css(
        "display": "none"
      )
    @_removeActiveClass()
    jQuery "#navJustMap"
      .addClass "active"

    @_handleFormVisibility false
    canvas.rescaleToContainer()
    return



  getEditMap: (canvas) =>
    jQuery "#imageUploadForm"
      .css(
        "display": "initial"
      )

    @_removeActiveClass()
    jQuery "#navEditMap"
      .addClass "active"

    @_handleFormVisibility true
    canvas.rescaleToContainer()
    return



  _removeActiveClass: ->
    jQuery "#navJustMap, #navEditMap"
      .removeClass "active"
    return



  _handleFormVisibility: (visible) ->
    if visible
      jQuery "#map, #control-panel"
        .addClass "with-form"
        .removeClass "without-form"
    else
      jQuery "#map, #control-panel"
        .addClass "without-form"
        .removeClass "with-form
        "
    jQuery "#map"
      .css(
        "top": if visible then '45px' else '0px'
      )
    jQuery "#control-panel"
      .css(
        "top": if visible then '95px' else '51px'
      )

  _init: (canvas) ->
    jQuery "#navJustMap"
      .on('click', @getJustMap.bind(this, canvas))

    jQuery "#navEditMap"
      .on('click', @getEditMap.bind(this, canvas))
