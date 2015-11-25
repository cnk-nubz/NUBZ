root = exports ? this

#uploading error:
# 0 - no error
# 1 - sucessful upload
# 2 - sent file is not a image
# 3 - problem with server
# 4 - not POST method
errorData = [
	{"info": "Mapa piętra #{root.activeFloor}. została pomyślnie zmieniona", "type": "success"}
	{"info": "Wybrany plik nie jest obrazkiem", "type": "info"}
	{"info": "Wystąpił wewnętrzny błąd serwera - spróbuj ponownie za chwilę", "type": "danger"}
	{"info": "form error - not POST method", "type": "warning"}
]
uploadError = root.uploadError
activeFloor = root.activeFloor
if uploadError isnt 0
	alert =	d3.select "body"
		.append "div"
		.classed(
			"alert": true
			"alert-#{errorData[uploadError - 1].type}": true
			"alert-dismissible": true
		)
		.attr(
			"role": "alert"
		)

	alert.append "button"
		.attr(
			"type": "button"
			"class": "close"
			"data-dismiss": "alert"
			"aria-label": "Close"
		)
		.append "span"
		.attr(
			"aria-hidden": "true"
		)
		.html "&times;"

	alert.append "a"
		.classed(
			"alert-link": true
		)
		.html "#{errorData[uploadError - 1].info}"
