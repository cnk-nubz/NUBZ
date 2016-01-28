root = exports ? this
root.Appearance = class Appearance
  instance = null
  constructor: ->
    return instance if instance
    instance = this
    @exhibit = {
      fillColor: '#ff7800'
      fillOpacity: 0.7
      weight: 1
      strokeColor: 'darkblue'
      strokeOpacity: 1
    }
    @navbar = {
      height: "50px"
    }
    @panel = {
      background: "rgba(221, 221, 221, 1)"
      border: "1px solid #aaa"
      zindex: 1029
      mapControls:
        width: "46px"
    }
