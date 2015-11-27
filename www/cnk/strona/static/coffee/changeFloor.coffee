root = exports ? this
margin = {"top": 5, "right": 25}
setThFloor = (n) ->
	root.activeFloor = n
	d3.select "#floorButton#{n}"
		.classed(
			"active": true
		)
	d3.select "#floorButton#{1-n}"
		.classed(
			"active": false
		)
	d3.select "#floorImage"
		.style("fill", "url(#floor#{n})")
	return

root.setThFloor = setThFloor
activeFloor = root.activeFloor

div = d3.select "#divImage"
	.append "div"
	.style(
		"position": "absolute"
		"top": "0px"
		"right": "10px"
		"width": "50px"
	)

div.append "div"
	.style(
		"padding-top": "5px"
	)
	.append "button"
	.attr("id", "floorButton0")
	.style(
		"min-width": "50px"
		"width": "auto"
		"height": "auto"
	)
	.classed(
		"btn": true
		"btn-primary": true
		"btn-xs": true
		"active": true
	)
	.html "Piętro 0"
	.on("click", ->setThFloor(0))

div.append "div"
	.style(
		"padding-top": "5px"
	)
	.append "button"
	.attr("id", "floorButton1")
	.style(
		"min-width": "50px"
		"width": "auto"
		"height": "auto"
	)
	.classed(
		"btn": true
		"btn-primary": true
		"btn-xs": true
	)
	.html "Piętro 1"
	.on("click", ->setThFloor(1))

#set default active floor
setThFloor(activeFloor)
