root = exports ? this
jQuery(document).ready( ->
  root.loadFloorImage(0, root.url_floor0)
  root.loadFloorImage(1, root.url_floor1)
  root.setThFloor root.activeFloor
)

delayResize = do ->
  timer = 0
  (callback, ms) ->
    clearTimeout(timer)
    timer = setTimeout(callback, ms)

jQuery(window).resize( ->
  delayResize( ->
    root.updateFloorExhibits(root.activeFloor)
  , 1) # 1ms for fluid transition. To be changed when amount of exhibits will grow
)
