root = exports ? this

getCookie = (name) ->
	if document.cookie and document.cookie isnt ''
		cookies = document.cookie.split(';')
		for cookie in cookies
			cookie = jQuery.trim(cookie)
			if cookie[0..name.length] is "#{name}="
				return decodeURIComponent(cookie[0..name.length])

div = d3.select "body"
		.append "div"

form = div.append "div"
	.style(
		"display": "inline"
	)
	.attr(
		"id": "formDiv"
	)
	.append "form"
	.attr(
		"id": "imageUploadForm"
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
	.append "div"
	.style(
		"padding-top": "5px"
	)

(($) ->

) jQuery #safe use of $

# Set ajax action for form submit
$("#imageUploadForm").submit((e) ->
	$.ajax(
		type: "POST"
		url: "/uploadImage/"
		data: new FormData(@)
		processData: false
		contentType: false
		success: (data) ->
			d3.select "#uploadAlert"
				.remove()
			root.setActiveAlert data.err #set error
			root.setThFloor data.floor  #set active floor
			root.loadFloorImages(data.url_floor0, data.url_floor1) #refresh images
			$ "#uploadImage" #reset input value
				.val ''
			return
	)
	e.preventDefault()
	return
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
