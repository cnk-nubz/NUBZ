root = exports ? this
root.MainPage = class MainPage extends root.Controller
  constructor: ->
    super
    @_init()

  _init: =>
    @addView("#a", new Navbar "#a")
