root = exports ? this
root.MainPage = class MainPage extends root.View
  constructor: ->
    super
    @_init()

  _init: =>
    @addView("navbar", new Navbar "#a")
    @_initCss()

  _initCss: =>
    mainPageStyle = {
      "overflow": "hidden"
      "width": "100vw"
      "height": "100vh"
    }
    d3.select("#a")
      .style(mainPageStyle)
