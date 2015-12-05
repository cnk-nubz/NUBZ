root = exports ? this
jQuery(document).ready( ->
  root.loadFloorImage(0, root.url_floor0)
  root.loadFloorImage(1, root.url_floor1)
  #HACK'y wacky - first spawn exhibits on not active floor
  #cause of bug in boxfit's plugin function (cant detect width of hidden elements)
  root.spawnExhibits 1-root.activeFloor
  root.spawnExhibits root.activeFloor
)
