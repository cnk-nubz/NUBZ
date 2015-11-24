getActiveFloorLevel = ->
	id = d3.select "#floorImage"
			.style "fill"
	return +id.charAt(id.length - 2)

clickChangeFloorButton = ->
	id = d3.select "#floorImage"
			.style "fill"
	last_dig = getActiveFloorLevel()
	d3.select "#changeFloorButton"
		.html "Zmień piętro na #{last_dig}."
	d3.select "#floorImage"
		.style("fill", if last_dig is 0 then "url(#floor1)" else "url(#floor0)")
	return

div = d3.select "body"
		.append "div"

div.append "button"
.attr("id", "changeFloorButton")
.style(
	"width": "auto"
	"height": "20px"
)
.classed(
	"btn": true
	"btn-primary": true
	"btn-xs": true
)
.html "Zmień piętro na 1."
.on("click", clickChangeFloorButton)
