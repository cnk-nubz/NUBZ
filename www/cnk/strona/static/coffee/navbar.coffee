root = exports ? this
formVisibility = (visible) ->
  d3.select "#map"
    .classed(
      'without-form': if not visible then true else false
      'with-form': if visible then true else false
    )

removeAlert = ->
  d3.select "#uploadAlert"
    .remove()
  return

removeActiveClass = ->
  d3.selectAll "#navJustMap, #navEditMap"
    .classed(
      "active": false
    )
  removeAlert()
  return

getJustMap = ->
  d3.selectAll "#imageUploadForm"
    .style(
      "display": "none"
    )

  removeActiveClass()
  d3.select "#navJustMap"
    .classed(
      "active": true
    )
  removeAlert()
  formVisibility false
  return

getEditMap = ->
  d3.select "#imageUploadForm"
    .style(
      "display": "initial"
    )

  removeActiveClass()

  d3.select "#navEditMap"
    .classed(
      "active": true
    )

  formVisibility true
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
	.attr(
		"id": "navbar"
	)
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
		"id": "navJustMap"
		"caption": "Podgląd mapy"
		"clickHandler": getJustMap
	},
	{
		"id": "navEditMap"
		"caption": "Edycja mapy"
		"clickHandler": getEditMap
	}
]

navigationBar.selectAll "li"
	.data navbarData
	.enter()
	 .append "li"
	.attr(
		"id": (d) -> d.id
	)
	.on("click", (d) -> d.clickHandler())
	 .append "a"
	.style(
		"cursor": "pointer"
	)
	.html (d) -> d.caption

d3.select("#navJustMap")
	.classed(
		"active": true
	)
