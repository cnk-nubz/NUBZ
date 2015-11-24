root = exports ? this
zoomed = ->
	d3.select("#mapZoom").attr("transform",
								"translate(#{d3.event.translate}) " +
								"scale(#{d3.event.scale})")
	return



zoom = d3.behavior.zoom()
	.scaleExtent [1, 5]
	.on("zoom", zoomed)
root.zoom = zoom #make it global for zooming buttons
root.svgWidth = "1280"
root.svgHeight = "960"
svg = d3.select "body"
	.append "div"
	.style(
		"width": "100%"
		"height": "auto"
	)
	.append "svg"
	.attr(
		"id": "mapImage"
		"viewBox": "0 0 #{root.svgWidth} #{root.svgHeight}"
		"preserveAspectRatio": "xMinYMin"
		"height": "600"
		"width": "800"
	)
	.append "g"
	.attr("id", "zoomGroup")
	.call zoom
	.append "g"


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
