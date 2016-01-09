root = exports ? this
root.MutableCanvas = class MutableCanvas extends root.Canvas
  constructor: ->
    super
    @changeMapButton = L.easyButton('fa-map-o',
                                    => @fireEvents "mapChangeRequest",
                                    'Zmień obrazek piętra',
                                    {position: 'bottomleft'}).addTo(@_map)
