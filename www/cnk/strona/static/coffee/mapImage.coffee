root = exports ? this
zoomed = ->
	d3.select("#mapZoom").attr("transform",
								"translate(#{d3.event.translate}) " +
								"scale(#{d3.event.scale})")
	return

zoom = d3.behavior.zoom()
	.scaleExtent [1, 10]
	.on("zoom", zoomed)

svg = d3.select "body"
	.append "div"
	.style(
		"width": "700px"
		"height": "auto"
	)
	.append "svg"
	.attr(
		"viewBox": "0 0 1 1"
		"preserveAspectRatio": "xMinYMin"
		"height": "100%"
		"width": "100%"
	)
	.append "g"
	.attr("id", "zoomGroup")
	.call zoom

svg.append "defs"
	.append "pattern"
	.attr(
		"id": "floor0"
		"patternUnits": "userSpaceOnUse"
		"width": "100%"
		"height": "100%"
		)
	.append "image"
	.attr(
		"width": "100%"
		"height": "100%"
		"x": "0"
		"y": "0"
		"xlink:href": root.url_floor0
	)

svg.append "defs"
		.append "pattern"
		.attr(
			"id": "floor1"
			"patternUnits": "userSpaceOnUse"
			"width": "100%"
			"height": "100%"
			)
		.append "image"
		.attr(
			"width": "100%"
			"height": "100%"
			"x": "0"
			"y": "0"
			"xlink:href": root.url_floor1
		)

container = svg.append "g"
			.attr("id", "mapZoom")

container.append "rect"
	.attr(
		"id": "floorImage"
		"width": "100%"
		"height": "100%"
		)
	.style("fill", "url(#floor0)")
