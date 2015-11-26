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
svgWidth = root.svgWidth
svgHeight = root.svgHeight
d3.select "body"
	.style(
		"overflow": "hidden"
		"margin": "0"
	)
svg = d3.select "body"
	.append "div"
	.attr(
		"id": "divImage"
	)
	.style(
		"position": "relative"
		"width": "100vw"
		"height": "calc(100vh - 50px)"
	)
	.append "svg"
	.attr(
		"id": "mapImage"
	)
	.style(
		"width": "100%"
		"height": "100%"
	)
	.append "g"
	.attr("id", "zoomGroup")
	.call zoom

svg.append "defs"
	.append "pattern"
	.attr(
		"id": "floor0"
		"width": "100%"
		"height": "100%"
		"patternContentUnits": "objectBoundingBox"
		"preserveAspectRatio": "xMidYMid meet"
	)
	.append "image"
	.attr(
		"id": "patternimage0"
		"preserveAspectRatio": "xMinYMin meet"
		"xlink:href": root.url_floor0
	)

svg.append "defs"
	.append "pattern"
	.attr(
		"id": "floor1"
		"width": "100%"
		"height": "100%"
		"patternContentUnits": "objectBoundingBox"
		"preserveAspectRatio": "xMidYMid meet"
	)
	.append "image"
	.attr(
		"id": "patternimage1"
		"preserveAspectRatio": "xMinYMin meet"
		"xlink:href": root.url_floor1
	)


tmpimg1 = new Image()
tmpimg1.src = root.url_floor1
tmpimg1.onload = () ->
	d3.select "#patternimage1"
		.attr(
			"width": tmpimg1.naturalWidth
			"height": tmpimg1.naturalHeight
		)
	d3.select "#floor1"
		.attr(
			"viewBox": "0 0 #{tmpimg1.naturalWidth} #{tmpimg1.naturalHeight}"
		)
	return

tmpimg0 = new Image()
tmpimg0.src = root.url_floor0
tmpimg0.onload = () ->
	d3.select "#patternimage0"
		.attr(
			"width": tmpimg0.naturalWidth
			"height": tmpimg0.naturalHeight
		)
	d3.select "#floor0"
		.attr(
			"viewBox": "0 0 #{tmpimg0.naturalWidth} #{tmpimg0.naturalHeight}"
		)
	return

container = svg.append "g"
			.attr("id", "mapZoom")

container.append "rect"
	.attr(
		"id": "floorImage"
	)
	.style(
		"fill": "url(#floor#{root.activeFloor})"
		"width": "100%"
		"height": "100%"
	)
