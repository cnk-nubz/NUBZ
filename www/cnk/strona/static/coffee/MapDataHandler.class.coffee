root = exports ? this
#just call new MapDataHandler()
root.MapDataHandler = class MapDataHandler
  instance = null
  constructor: (activeFloor, floorUrl, exhibits, floorTilesInfo) ->
    return instance if instance
    instance = this
    @activeFloor = activeFloor
    @activeLabels = true
    @floorUrl = floorUrl
    @exhibits = {}
    for e in exhibits
      @exhibits[e.id] = {
        frame: e.frame
        name: e.name
        colorHex: e.colorHex
      }
    @floorTilesInfo = floorTilesInfo
    @minZoom = 1
    @maxZoom = [@floorTilesInfo[0].length, @floorTilesInfo[1].length]
    @currentZoom = [1, 1]
    @currentCenter = [[0, 0], [0, 0]]
