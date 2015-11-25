root = exports ? this
div = d3.select "body"
		.append "div"

form = div.append "form"
.attr(
	"id": "imageUploadForm"
	"method": "POST"
	"enctype": "multipart/form-data"
	"action": "/uploadImage/"
)
.style(
	"display": "none"
	"padding-top": "5px"
)

.html root.csrf_token

form.append "input"
.attr(
	"id": "uploadImage"
	"type": "file"
	"name": "image"
	"accept": "image/*"
)
.classed(
	"btn": true
	"btn-xs": true
	"btn-default": true
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
