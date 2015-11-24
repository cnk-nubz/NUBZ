root = exports ? this
margin = {"top": 25, "left": 25}
width = 50
height = 50

div = d3.select "#mapImage"

zoomed = ->
	d3.select("#mapZoom").attr("transform",
								"translate(#{d3.event.translate}) " +
								"scale(#{d3.event.scale})")
	return

zoom = root.zoom

svgWidth = root.svgWidth
svgHeight = root.svgHeight
zoomByFactor = (factor) ->
	scale = zoom.scale()
	extent = zoom.scaleExtent()
	newScale = scale * factor
	if extent[0] > newScale
		newScale = 1
	if extent[1] < newScale
		newScale = 5

	t = zoom.translate()
	c = [svgWidth / 2, svgHeight / 2]
	zoom.scale(newScale)
		.translate([
			(c[0] + (t[0] - c[0]) / scale * newScale),
			(c[1] + (t[1] - c[1]) / scale * newScale)
		]).event(d3.select("#mapZoom"))
	return

zoomIn = ->
	zoomByFactor(1.2)
	return

zoomOut = ->
	zoomByFactor(0.8)
	return

resetZoom = ->
	zoom.scale(1).translate([0, 0])
	d3.select("#mapZoom")
		.attr("transform", "translate(0, 0) scale(1)")
	return

plusGroup = div.append "g"
	.attr(
		"id": "plusGroup"
	)
	.classed(
		"btn": true
	)

plusGroup.append "rect"
	.attr(
		"x": "#{margin.left}px"
		"y": "#{margin.top}px"
		"width": "#{width}px"
		"height": "#{height}px"
		"rx": "10px"
	)
	.style(
		"fill": "lightgray"
		"fill-opacity": "0.5"
		"stroke": "lightblue"
		"stroke-width": "5px"
	)
plusGroup.append "text"
	.attr(
		"x": "#{margin.left + width/2}px"
		"y": "#{margin.top + height/2 + 10 }px"
		"font-size": "32"
		"fill": "blue"
	)
	.style(
		"text-anchor": "middle"
	)
	.html "+"

minusGroup = div.append "g"
	.attr(
		"id": "minusGroup"
	)
	.classed(
		"btn": true
	)
minusGroup.append "rect"
	.attr(
		"x": "#{margin.left}px"
		"y": "#{2 * margin.top + height}px"
		"width" : "#{width}px"
		"height": "#{height}px"
		"rx": "10px"
	)
	.style(
		"fill": "lightgray"
		"fill-opacity": "0.5"
		"stroke": "lightblue"
		"stroke-width": "5px"
	)
minusGroup.append "text"
	.attr(
		"x": "50px"
		"y": "#{10 + 2 * margin.top + height + height/2}px"
		"font-size": "32"
		"fill": "blue"
	)
	.style(
		"text-anchor": "middle"
	)
	.html "-"

resetGroup = div.append "g"
	.attr(
		"id": "resetGroup"
	)
	.classed(
		"btn": true
	)

resetGroup.append "rect"
	.attr(
		"x": "#{margin.left}px"
		"y": "#{3 * margin.top + 2*height}px"
		"width" : "#{width}px"
		"height": "#{height}px"
		"rx": "10px"
	)
	.style(
		"fill": "lightgray"
		"fill-opacity": "0.5"
		"stroke": "lightblue"
		"stroke-width": "5px"
	)
resetGroup.append "text"
	.attr(
		"x": "50px"
		"y": "#{5 + 3 * margin.top + 2*height + height/2}px"
		"font-size": "16"
		"fill": "blue"
	)
	.style(
		"text-anchor": "middle"
	)
	.html "Reset"

plusGroup.on("click", zoomIn)
minusGroup.on("click", zoomOut)
resetGroup.on("click", resetZoom)
