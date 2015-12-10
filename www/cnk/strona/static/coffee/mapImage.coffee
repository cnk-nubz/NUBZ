root = exports ? this
zoomed = ->
	d3.select "#mapZoom"
		.attr(
			"transform": "translate(#{d3.event.translate}) scale(#{d3.event.scale})"
		)
	return

zoomedEnd = ->
	d3.select "#minusButton"
		.property(
			"disabled": if root.zoom.scale() is root.minZoomScale then true else false
		)

	d3.select "#plusButton"
		.property(
			"disabled": if root.zoom.scale() is root.maxZoomScale then true else false
		)
	return

calcMapImageSize = (floor) ->
	heightFloor1 = d3.select("#mapContainer").node().getBoundingClientRect().height
	widthFloor1 = d3.select("#mapContainer").node().getBoundingClientRect().width
	root.svgHeight[floor] = heightFloor1
	root.svgWidth[floor] = widthFloor1
	return

calcNewMapCoords = (floor) ->
	svgMultRatio = root.svgHeight[floor] * root.floorRealImageWidth[floor]
	imageMultRatio = root.svgWidth[floor] * root.floorRealImageHeight[floor]
	if svgMultRatio == imageMultRatio
		root.floorImageX[floor] = 0
		root.floorImageY[floor] = 0
	else if svgMultRatio < imageMultRatio #glued to top line
		scaledWidth = root.svgHeight[floor] * root.floorRealImageWidth[floor] / root.floorRealImageHeight[floor]
		root.floorScaledImageWidth[floor] = scaledWidth
		root.floorImageX[floor] = 0
		root.floorImageY[floor] = root.svgWidth[floor] / 2 - scaledWidth / 2
	else
		scaledHeight = root.svgWidth[floor] * root.floorRealImageHeight[floor] / root.floorRealImageWidth[floor]
		root.floorScaledImageHeight[floor] = scaledHeight
		root.floorImageX[floor] = root.svgHeight[floor] / 2 - scaledHeight / 2
		root.floorImageY[floor] = 0
	return

strokeWidthScale = d3.scale.linear().domain([2, 1024]).range([0.1, 6])
getStrokeWidth = (d) ->
  if d > 1024 then 6 else strokeWidthScale(d)

roundCornerScale = d3.scale.linear().domain([2, 1024]).range([0.1, 16])
getRoundCorner = (d) ->
  if d > 1024 then 16 else roundCornerScale(d)


updateFloorExhibits = (floor) ->
  d3.selectAll(".exhibitFloor#{floor}")
    .each((d, i) ->
        if root.floorImageX[floor] is 0 #glued to top line
          ratio = root.floorScaledImageWidth[floor] / root.floorRealImageWidth[floor]
        else
          ratio = root.floorScaledImageHeight[floor] / root.floorRealImageHeight[floor]

        roundCornerSize = getRoundCorner(d.width * ratio + d.height * ratio)
        d3.select this
          .selectAll "rect, foreignObject"
          .attr(
            "x": d.x * ratio + root.floorImageY[floor]
            "y": d.y * ratio + root.floorImageX[floor]
            "width": d.width * ratio
            "height": d.height * ratio
          )

        d3.select this
          .select "rect"
          .attr(
            "rx": "#{roundCornerSize}px"
            "ry": "#{roundCornerSize}px"
            "stroke-width": "#{getStrokeWidth(d.width * ratio + d.height * ratio)}px"
          )

        d3.select this
          .select "foreignObject div"
          .style(
            "cursor": "default"
            "width": "#{d.width * ratio}px"
            "height": "#{d.height * ratio}px"
          )
          .html d.exhibitName
          .style(
            "font-family": "Open Sans"
          )

        jQuery("foreignObject div", this).boxfit {'multiline': true, 'minimum_font_size': 1}
    )
  return

loadFloorImage = (floor, filename) ->
  if filename?
    tmpimg = new Image()
    tmpimg.src = filename
    tmpimg.onload = ->
      d3.select "#patternImage#{floor}"
      .attr(
          "width": tmpimg.naturalWidth
          "height": tmpimg.naturalHeight
          "xlink:href": filename
      )
      d3.select "#floor#{floor}"
      .attr(
        "viewBox": "0 0 #{tmpimg.naturalWidth} #{tmpimg.naturalHeight}"
      )

      root.floorRealImageHeight[floor] = tmpimg.naturalHeight
      root.floorRealImageWidth[floor] = tmpimg.naturalWidth
      calcMapImageSize floor
      calcNewMapCoords floor
      jQuery(".exhibitFloor#{floor}").remove()
      root.spawnExhibits floor
      updateFloorExhibits floor
  return
root.loadFloorImage = loadFloorImage

zoom = d3.behavior.zoom()
	.scaleExtent [root.minZoomScale, root.maxZoomScale]
	.on("zoom", zoomed)
	.on("zoomend", zoomedEnd)
root.zoom = zoom #make it global for zooming buttons
d3.select "body"
	.style(
		"overflow": "hidden"
		"margin": "none"
	)

svg = d3.select "body"
	 .append "div"
	.attr(
		"id": "mapContainer"
	)
	.style(
		"position": "relative"
		"width": "100vw"
		"height": "calc(100vh - 50px)"
	)
	 .append "svg"
	.attr(
    "width": "100%"
    "height": "100%"
    "id": "mapImage"
    "xmlns": "http://www.w3.org/2000/svg"
    "version": "1.1"
    "xmlns:xlink": "http://www.w3.org/1999/xlink"
	)
	.style(
		"width": "100%"
		"height": "100%"
	)
	.append "g"
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
		"id": "patternImage0"
		"preserveAspectRatio": "xMinYMin meet"
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
		"id": "patternImage1"
		"preserveAspectRatio": "xMinYMin meet"
	)

container = svg.append "g"
	.attr(
		"id": "mapZoom"
	)

container.append "rect"
    .attr(
      "id": "floorImage"
      "width": "100%"
      "height": "100%"
  	)
