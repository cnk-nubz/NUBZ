root = exports ? this
root.Appearance = class Appearance
  instance = null
  constructor: ->
    return instance if instance
    instance = this
    @panelBackground =
    @exhibit = {
      fillColor: '#ff7800'
      fillOpacity: 0.7
      weight: 1
      color: 'darkblue'
      opacity: 1
    }
    @navbar = {
      height: "50px"
    }
    @panel = {
      background: "rgba(91, 91, 91, 1)"
      left:
        width: "46px"
      right:
        width: "250px"
    }
