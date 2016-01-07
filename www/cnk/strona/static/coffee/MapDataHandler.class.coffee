root = exports ? this
#just call new MapDataHandler()
root.MapDataHandler = class MapDataHandler
  instance = null
  constructor: (activeFloor, floorUrl, exhibits, floorTilesInfo) ->
    return instance if instance
    instance = this
    @activeFloor = activeFloor
    @floorUrl = floorUrl
    @exhibits = exhibits
    @floorTilesInfo = floorTilesInfo
    @visibleExhibits = [
      (e for e in @exhibits when e.frame?.mapLevel is 0),
      (e for e in @exhibits when e.frame?.mapLevel is 1)
    ]
