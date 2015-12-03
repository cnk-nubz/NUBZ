root = exports ? this
spawnExhibits = (floor) ->
	exhibitsData = floorExhibits[floor]
	d3.select "#exhibitsFloor#{floor}"
		.selectAll "rect"
		.data(exhibitsData)
		.enter()
		 .append "rect"
		.attr(
			"x": (d) -> "#{d.x}px"
			"y": (d) -> "#{d.y}px"
			"width": (d) -> "#{d.width}px"
			"height": (d) -> "#{d.height}px"
			"rx": "3px"
			"ry": "3px"
		)
		.style(
			"fill": "none"
			"stroke-width": "2px"
			"stroke": "blue"
			"pointer": "default"
		)
		.classed(
			"exhibitFloor#{floor}": true
		)
	return
root.spawnExhibits = spawnExhibits

d3.select "#mapZoom"
	 .append "g"
	.attr(
		"id": "exhibitsFloor0"
	)

d3.select "#mapZoom"
	 .append "g"
	.attr(
		"id": "exhibitsFloor1"
	)
