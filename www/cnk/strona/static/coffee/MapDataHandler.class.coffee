root = exports ? this
#just call new MapDataHandler()
root.MapDataHandler = class MapDataHandler
  instance = null
  constructor: (activeFloor, floorUrl, exhibits, floorTilesInfo) ->
    return instance if instance
    instance = this
    @activeFloor = activeFloor
    @floorUrl = floorUrl
    @exhibits = {}
    for e in exhibits
      @exhibits[e.id] = {
        frame: e.frame
        name: e.name
      }
    @floorTilesInfo = floorTilesInfo
    @minZoom = 1
    @maxZoom = [@floorTilesInfo[0].length, @floorTilesInfo[1].length]
