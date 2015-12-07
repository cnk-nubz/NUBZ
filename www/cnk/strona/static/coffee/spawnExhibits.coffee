root = exports ? this
spawnExhibits = (floor) ->
  exhibitsData = root.floorExhibits[floor]
  exhibitGroup = d3.select "#exhibitsFloor#{floor}"
    .selectAll "g"
    .data(exhibitsData)
    .enter()
    .append "g"
    .classed(
      "exhibitFloor#{floor}": true
    )

  exhibitGroup.each((d, _) ->
    d3.select this
       .append "rect"
      .attr(
        "rx": "3px"
        "ry": "3px"
      )
      .style(
        "fill": "lightblue"
        "stroke": "blue"
        "stroke-width": "1px"
        "pointer": "default"
      )

    d3.select this #kind of hack...
       .append "foreignObject"
      .attr(
        "fill": "none"
      )
      .append "xhtml:div"
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
