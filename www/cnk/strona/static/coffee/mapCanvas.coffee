root = exports ? this
d3.select "body"
   .append "div"
  .attr(
    "id": "map"
  )

map = L.map('map', {
  minZoom: 1
  maxZoom: 4
  zoom: 1
  crs: L.CRS.Simple
})

w = (root.floorTilesInfo[i].scaledWidth for i in [0..1])
h = (root.floorTilesInfo[i].scaledHeight for i in [0..1])
console.log w
console.log h
exhibits = [new L.LayerGroup(), new L.LayerGroup()]
mapBounds = [
  new L.LatLngBounds(map.unproject([0, h[0]], 3), map.unproject([w[0], 0], 3)),
  new L.LatLngBounds(map.unproject([0, h[1]], 3), map.unproject([w[1], 0], 3))
]
console.log mapBounds

#TODO: Attach ?id=(next_generated_id) at the end of url to force browser to re-render
floorLayer = [
    L.tileLayer(root.urlFloor[0], {
      maxNativeZoom: 3
      tileSize: root.floorTilesInfo[0].tileWidth
      continuousWorld: true
      crs: L.CRS.Simple
      bounds: mapBounds[0]
    }),
    L.tileLayer(root.urlFloor[1], {
      maxNativeZoom: 3
      tileSize: root.floorTilesInfo[1].tileWidth
      continuousWorld: true
      crs: L.CRS.Simple
      bounds: mapBounds[1]
    })
]

console.log floorLayer

labelsButton = L.easyButton({
  states:[
      {
        stateName: 'setLabels'
        icon: 'fa-tags'
        title: 'Pokaż etykiety'
        onClick: (btn, map) ->
          btn.state('removeLabels')
          activeFloor = if map.hasLayer floorLayer[0] then 0 else 1
          exhibits[activeFloor].eachLayer((layer) ->
            layer.showLabel()
          )
          return
      },
      {
        stateName: 'removeLabels'
        icon: 'fa-times'
        title: 'Ukryj etykiety'
        onClick: (btn, map) ->
          btn.state('setLabels')
          activeFloor = if map.hasLayer floorLayer[0] then 0 else 1
          exhibits[activeFloor].eachLayer((layer) ->
            layer.hideLabel()
          )
          return
      }
  ]
}).addTo map

labelsButton.state 'removeLabels'

setFloorLayer = (floor) ->
  (btn, map) ->
    map.removeLayer exhibits[1 - floor]
    map.removeLayer floorLayer[1 - floor]
    map.addLayer floorLayer[floor]
    map.addLayer exhibits[floor]

    if labelsButton._currentState.stateName is 'removeLabels' #setLabels is active
      exhibits[1 - floor].eachLayer((layer) ->
        layer.hideLabel()
      )
      exhibits[floor].eachLayer((layer) ->
        layer.showLabel()
      )

    map.setMaxBounds mapBounds[floor]
    map.setView(map.unproject([0, 0], 1), 1)
    return

floorButton = [
  L.easyButton('<strong>0</strong>', setFloorLayer 0, 'Piętro 0')
  L.easyButton('<strong>1</strong>', setFloorLayer 1, 'Piętro 1')
]
floorButton[0].button.onclick = ->
  root.activeFloor = 1 - root.activeFloor
floorButton[1].button.onclick = ->
  root.activeFloor = 1 - root.activeFloor


floorButton[0].addTo map
floorButton[1].addTo map

# SPAWN EXHIBITS
exhibitCnt = 0
spawnExhibits = (floor) ->
  for e in root.floorExhibits[floor]
    X = e.frame.x
    Y = e.frame.y
    polygonBounds = [
      map.unproject([X, Y], 3),
      map.unproject([X, Y + e.frame.width], 3),
      map.unproject([X + e.frame.height, Y + e.frame.height], 3),
      map.unproject([X + e.frame.height, Y], 3)
    ]
    r = L.polygon(polygonBounds, {
        color: "#ff7800"
        weight: 1
      }).bindLabel(e.exhibitName, {
        noHide: true
        direction: 'auto'
      })
    r._leaflet_id = "exhibit#{exhibitCnt++}"
    console.log r
    exhibits[floor].addLayer(r)
  return

jQuery(document).ready( ->
  spawnExhibits 0
  spawnExhibits 1
  exhibits[0].eachLayer((layer) ->
    layer.on('click', ->
      console.log "hej jestem"
    )
  )
  #set initial state
  activeFloor = root.activeFloor
  map.addLayer floorLayer[activeFloor]
  map.addLayer exhibits[activeFloor]
  exhibits[activeFloor].eachLayer((layer) ->
    layer.showLabel()
  )
  map.setMaxBounds mapBounds[activeFloor]
  map.setView(map.unproject([0, 0], 1), 1)
)

root.refreshMap = (floor, tileSize, scaledSize) ->
  mapBounds = new L.LatLngBounds(
    map.unproject([0, scaledSize.height], 3),
    map.unproject([scaledSize.width, 0], 3)
  )
  map.setMaxBounds mapBounds
  floorLayer[floor]._tileSize = L.point(tileSize.width, tileSize.height)
  map.invalidateSize()
  map._onResize()
