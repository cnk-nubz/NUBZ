root = exports ? this
margin = {"top": 5, "left": 25}
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

buttonGroup = d3.select("#divImage").append "div"
	.style(
		"position": "absolute"
		"top": "0px"
	)

buttonData = [
	{"caption": "+", "id": "plusButton", "f": zoomIn}
	{"caption": "0", "id": "resetButton", "f": resetZoom}
	{"caption": "-", "id": "minusButton", "f": zoomOut}
]

buttonGroup.selectAll "button"
	.data buttonData
	.enter()
	.append "div"
	.style(
		"padding-top": "5px"
	)
	.append "button"
	.attr(
		"id": (d) -> d.id
	)
	.style(
		"width": "34px"
		"height": "34px"
	)
	.classed(
		"btn": true
		"btn-primary": true
	)
	.html (d) -> d.caption
	.on("click", (d) -> d.f())
