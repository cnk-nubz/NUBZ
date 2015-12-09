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
      .style(
        "fill": "lightblue"
        "stroke": "blue"
        "pointer": "default"
      )

    d3.select this #kind of hack...
       .append "foreignObject"
      .attr(
        "fill": "none"
      )
       .append "xhtml:div"
      .style(
        "font-family": "Open Sans"
      )
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
