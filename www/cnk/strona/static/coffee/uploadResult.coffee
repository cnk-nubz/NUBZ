root = exports ? this

#uploading error:
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
root.setActiveAlert = (err) ->

	alert = d3.select "body"
		.insert("div", ":first-child")
		.classed(
			"alert": true
			"alert-#{errorData[err - 1].type}": true
			"alert-dismissible": true
		)
		.attr(
			"id": "uploadAlert"
			"role": "alert"
		)

	alert.append "button"
		.attr(
			"type": "button"
			"data-dismiss": "alert"
			"aria-label": "Close"
		)
		.classed(
			"close": true
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
		.html "#{errorData[err - 1].info}"
