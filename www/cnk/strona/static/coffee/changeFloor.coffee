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

activeFloor = root.activeFloor

div = d3.select "#divImage"
	.append "div"
	.style(
		"position": "absolute"
		"top": "0"
		"right": "0"
		"width": "50px"
		"padding-top": "5px"
	)

div.append "button"
.attr("id", "floorButton0")
.style(
	"padding-top": "5px"
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

div.append "button"
.attr("id", "floorButton1")
.style(
	"padding-top": "5px"
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
