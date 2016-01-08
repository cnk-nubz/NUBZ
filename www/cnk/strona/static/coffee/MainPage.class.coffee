root = exports ? this
root.MainPage = class MainPage extends root.View
  constructor: ->
    super
    @_init()

  _init: =>
    mainPageStyle = {
      "overflow": "hidden"
      "width": "100vw"
      "height": "100vh"
    }
    @addView("navbar", new Navbar "#a")
    d3.select("#a")
      .style(mainPageStyle)
