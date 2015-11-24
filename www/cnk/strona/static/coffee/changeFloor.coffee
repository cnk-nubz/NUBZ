getThFloor = (n) ->
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

div = d3.select "body"
		.append "div"
div.append "button"
.attr("id", "floorButton0")
.style(
	"min-width": "50px"
	"width": "auto"
	"height": "20px"
)
.classed(
	"btn": true
	"btn-primary": true
	"btn-xs": true
	"active": true
)
.html "Piętro 0."
.on("click", ->getThFloor(0))

div.append "button"
.attr("id", "floorButton1")
.style(
	"min-width": "50px"
	"width": "auto"
	"height": "20px"
)
.classed(
	"btn": true
	"btn-primary": true
	"btn-xs": true
)
.html "Piętro 1."
.on("click", ->getThFloor(1))
