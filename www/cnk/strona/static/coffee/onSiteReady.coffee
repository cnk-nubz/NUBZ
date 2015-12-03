root = exports ? this
(($) ->

) jQuery
$(document).ready( ->
  root.loadFloorImage(0, root.url_floor0)
  root.loadFloorImage(1, root.url_floor1)
  root.spawnExhibits(0)
  root.spawnExhibits(1)
  root.setThFloor(root.activeFloor)
)
