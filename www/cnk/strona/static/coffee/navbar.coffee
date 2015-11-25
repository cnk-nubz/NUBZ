getJustMap = ->
	d3.select "#imageUploadForm"
		.style(
			"display": "none"
		)
	return
getEditMap = ->
	d3.select "#imageUploadForm"
		.style(
			"display": "initial"
		)
	return

navigationBar = d3.select "body"
	.append "nav"
	.classed(
		"navbar": true
		"navbar-inverse": true
		"navbar-fixed-top": true
	)
	.append "div"
	.classed(
		"container": true
	)
	.append "div"
	.attr("id", "navbar")
	.classed(
		"collapse": true
		"navbar-collapse": true
	)
	.append "ul"
	.classed(
		"nav": true
		"navbar-nav": true
	)

navbarData = [
	{
		"caption": "Podgląd mapy"
		"clickHandler": getJustMap
	},
	{
		"caption": "Edycja mapy"
		"clickHandler": getEditMap
	}
]

navigationBar.selectAll("li")
	.data(navbarData)
	.enter()
	.append("li")
	.on("click", (d) -> d.clickHandler())
	.append "a"
	.style(
		"cursor": "pointer"
	)
	.html (d) -> d.caption
