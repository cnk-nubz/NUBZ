root = exports ? this
margin = {"top": 5, "left": 25}
width = 50
height = 50

div = d3.select "#mapImage"

zoomed = ->
	d3.select "#mapZoom"
		.attr("transform","translate(#{d3.event.translate}) " +
								"scale(#{d3.event.scale})")
	return

zoom = root.zoom

svgWidth = root.svgWidth
svgHeight = root.svgHeight
zoomByFactor = (factor) ->
	scale = zoom.scale()
	if scale is 1 and factor > 1
		d3.select "#minusButton"
			.property(
				"disabled": false
			)
	else if scale is 5 and factor < 1
		d3.select "#plusButton"
			.property(
				"disabled": false
			)
	extent = zoom.scaleExtent()
	newScale = scale * factor
	if extent[0] > newScale
		newScale = 1
		d3.select "#minusButton"
			.property(
				"disabled": true
			)
	else if extent[1] < newScale
		newScale = 5
		d3.select "#plusButton"
			.property(
				"disabled": true
			)

	t = zoom.translate()
	boundingRect = d3.select("#divImage").node().getBoundingClientRect()
	c = [
		boundingRect.width / 2
		boundingRect.height / 2
	]
	zoom.scale newScale
		.translate([
			(c[0] + (t[0] - c[0]) / scale * newScale),
			(c[1] + (t[1] - c[1]) / scale * newScale)
		]).event(d3.select "#mapZoom")
	return

zoomIn = ->
	zoomByFactor 1.2
	return

zoomOut = ->
	zoomByFactor 0.8
	return

resetZoom = ->
	zoom.scale 1
		.translate [0, 0]
	d3.select "#mapZoom"
		.attr("transform", "translate(0, 0) scale(1)")
	d3.select "#minusButton"
		.property(
			"disabled": true
		)
	d3.select "#plusButton"
		.property(
			"disabled": false
		)
	return

buttonGroup = d3.select "#divImage"
	.append "div"
	.style(
		"position": "absolute"
		"top": "0px"
		"left": "10px"
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

resetZoom()
