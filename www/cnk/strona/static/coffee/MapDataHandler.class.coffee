root = exports ? this
#just call new MapDataHandler()
root.MapDataHandler = class MapDataHandler
  instance = null
  constructor: (activeFloor, floorUrl, exhibits, floorTilesInfo, avaibleFloors) ->
    return instance if instance
    instance = this
    # activeFloor :: Int
    @activeFloor = activeFloor
    # activeLabels :: Boolean
    @activeLabels = true
    # floorUrl :: String
    @floorUrl = floorUrl
    ###
    # type ExhibitFrame = {
    #   height   :: Int,
    #   width    :: Int,
    #   x        :: Int,
    #   y        :: Int,
    #   mapLevel :: Int
    # }
    ###
    ###
    # type Exhibit = {
    #   colorHex :: String,
    #   frame    :: Maybe ExhibitFrame,
    #   name     :: String
    # }
    ###
    # exhibitsList :: [Exhibit]
    @exhibitsList = exhibits
    ###
    # exhibits :: (JsObject :: (Int, Exhibit))
    ###
    @exhibits = {}
    if exhibits?
      for e in exhibits
        @exhibits[e.id] = {
          frame: e.frame
          name: e.name
          colorHex: e.colorHex
        }
    ###
    # type TilesInfo = {
    #   scaledHeight :: Int,
    #   scaledWidth  :: Int,
    #   tileHeight   :: Int,
    #   tileWidth    :: Int
    # }
    ###
    # type SingleFloorTilesInfo :: [TileInfo]
    # floorTilesInfo :: [SingleFloorTilesInfo]
    @floorTilesInfo = floorTilesInfo
    # avaibleFloors :: [Int]
    @avaibleFloors = avaibleFloors
    # minZoom :: Int
    @minZoom = 1
    # maxZoom :: [Int]
    if not @floorTilesInfo?
      @maxZoom = [1]
    else
      @maxZoom = @floorTilesInfo.map((x) => if x? then Object.keys(x).length else @minZoom)
    # numberOfFloors :: Int
    @numberOfFloors = Math.max.apply(null, @avaibleFloors)


  # getFloorUrl :: Int -> String
  getFloorUrl: (floor) =>
    @floorUrl.replace("{f}", "#{floor}")
