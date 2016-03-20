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
      }
    @floorTilesInfo = floorTilesInfo
    @minZoom = 1
    @maxZoom = [
      if @floorTilesInfo[0].length is 0 then 1 else @floorTilesInfo[0].length,
      if @floorTilesInfo[1].length is 0 then 1 else @floorTilesInfo[1].length
    ]
    @currentZoom = [1, 1]
    @currentCenter = [[0, 0], [0, 0]]
