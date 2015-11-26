root = exports ? this
div = d3.select "body"
		.append "div"

form = div.append "div"
	.style(
		"display": "inline"
		"height": "39px"
	)
	.attr(
		"id": "formDiv"
	)
	.append "form"
	.attr(
		"id": "imageUploadForm"
		"method": "POST"
		"enctype": "multipart/form-data"
		"action": "/uploadImage/"
	)
	.classed(
		"form-inline": true
	)
	.style(
		"display": "none"
		"padding-top": "5px"
	)
	.html root.csrf_token
	.append "div" #spacer
	.style(
		"padding-bottom": "5px"
	)

form.append "div"
	.classed(
		"form-group": true
	)
	.append "input"
	.attr(
		"id": "uploadImage"
		"type": "file"
		"name": "image"
		"required": "true"
		"accept": "image/*"
	)
	.classed(
		"form-control": true
	)

form.append "input"
	.attr(
		"type": "submit",
		"value": "Prześlij",
	)
	.classed(
		"btn": true
		"btn-default": true
		"btn-xs": true
	)
.on("click", ->
	d3.select "#hiddenFloorLevel"
		.attr("value", root.activeFloor)
)

form.append "input"
	.attr(
		"id": "hiddenFloorLevel"
		"type": "hidden"
		"name": "floor"
	)
