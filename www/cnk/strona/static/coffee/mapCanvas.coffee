root = exports ? this
d3.select "body"
   .append "div"
  .attr(
    "id": "map"
  )
  .classed(
    'without-form': true
  )

map = L.map('map', {
  minZoom: 1
  maxZoom: 3
  zoom: 1
  crs: L.CRS.Simple
})
#boundaries will be set according to highest zoom
mapWidth = (root.floorTilesInfo[i][-1..][0].scaledWidth for i in [0..1])
mapHeight = (root.floorTilesInfo[i][-1..][0].scaledHeight for i in [0..1])
mapMargin = {left: 190}
console.log 'mapWidth'
console.log mapWidth
console.log 'mapHeight'
console.log mapHeight
exhibits = [new L.LayerGroup(), new L.LayerGroup()]
mapBounds = [
  new L.LatLngBounds(map.unproject([-mapMargin.left, mapHeight[0]], 3), map.unproject([mapWidth[0], 0], 3)),
  new L.LatLngBounds(map.unproject([-mapMargin.left, mapHeight[1]], 3), map.unproject([mapWidth[1], 0], 3))
]
console.log 'mapBounds'
console.log mapBounds

floorLayer = [new L.LayerGroup(), new L.LayerGroup()]
for i in [0..1]
  len = root.floorTilesInfo[i].length
  for j in [0...len]
    zoomLayer = L.tileLayer(root.urlFloor[i].replace('{z}', "#{j+1}"), {
        minZoom: j+1
        maxZoom: j+1
        tileSize: root.floorTilesInfo[i][j].tileWidth
        errorTileUrl: '/static/inactiveTile.png'
        continuousWorld: true
        crs: L.CRS.Simple
        bounds: mapBounds[i]
    })
    floorLayer[i].addLayer zoomLayer

console.log 'floorLayer'
console.log floorLayer


labelsButton = L.easyButton({
  states:[
      {
        stateName: 'setLabels'
        icon: 'fa-comment-o'
        title: 'Pokaż etykiety'
        onClick: (btn, map) ->
          btn.state('removeLabels')
          activeFloor = root.activeFloor
          exhibits[activeFloor].eachLayer((layer) ->
            layer.showLabel()
          )
          return
      },
      {
        stateName: 'removeLabels'
        icon: 'fa-comment'
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
    root.activeFloor = floor
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

floorButton[0].addTo map
floorButton[1].addTo map

# SPAWN EXHIBITS
spawnExhibits = (floor) ->
  for e in root.visibleExhibits[floor]
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
      }).bindLabel(e.name, {
        noHide: true
        direction: 'auto'
      })
    exhibits[floor].addLayer(r)
  return

jQuery(document).ready( ->
  spawnExhibits 0
  spawnExhibits 1
  #set initial state
  #TODO: somehow fire click on active floor's button
  activeFloor = root.activeFloor
  map.addLayer floorLayer[activeFloor]
  map.addLayer exhibits[activeFloor]
  map.setMaxBounds mapBounds[activeFloor]
  map.setView(map.unproject([0, 0], 1), 1)
)

root.refreshMap = (floor) ->
  floorLayer[floor].setUrl("#{urlFloor[floor]}?t=#{Math.floor(Math.random() * 1024)}")
