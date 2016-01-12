root = exports ? this
root.ExhibitDialog = class ExhibitDialog extends root.View
	@nameEditable: false
	constructor: (@_name, @_floor, @_handler) ->

	show: =>
		_message =
			d3.select(document.createElement("div"))
			.append "div"
				.classed "container-fluid", true

		line1 = _message
			.append "form"
				.classed "form-group row", true

		line1_title = line1
			.append "div"
				.classed "col-sm-2", true

		line1_title
			.append "label"
				.text("Nazwa:")
				.attr(
					for: "name"
				)
				.style(
					"padding-top" : "0.5em"
				)

		line1_input = line1
			.append "div"
				.classed "col-sm-10", true

		line1_input_field = line1_input
			.append "input"
				.attr(
					id: "name"
					type: "text"
					value: @_name
					disabled: "true" if not @nameEditable
				)
				.classed "form-control", true

		line1_input
			.append "div"
				.attr(
					id: "error"
				)

		line2 = _message
			.append "div"
				.style(
					"margin-top" : "0.5em"
				)
				.classed "row", true

		line2_label = line2
			.append "div"
				.classed "col-sm-2", true

		line2_label
			.append "label"
				.text("Piętro:")
				.style(
					"padding-top" : "0.5em"
				)

		line2_radios = line2
			.append "div"
				.attr(
					role: "group"
					'data-toggle': "buttons"
				)
				.classed "btn-group col-sm-10", true

		line2_radios_label1 = line2_radios
			.append "label"
				.attr(
					id: 'radio1'
				)
				.text "0"
				.classed "btn btn-default" + (if @_floor is 0 then " active" else ""), true

		line2_radios_label1
			.append "input"
				.attr(
					type: "radio"
					name: "rfloor"
				)

		line2_radios_label2 = line2_radios
			.append "label"
				.attr(
					id: 'radio2'
				)
				.text "1"
				.classed "btn btn-default" + (if @_floor is 1 then " active" else ""), true

		line2_radios_label2
			.append "input"
				.attr(
					type: "radio"
					name: "rfloor"
				)

		line2_radios_label3 = line2_radios
			.append "label"
				.attr(
					id: 'radio3'
				)
				.text "break"
				.classed "btn btn-default" + (if @_floor is null then " active" else ""), true

		line2_radios_label3
			.append "input"
				.attr(
					type: "radio"
					name: "rfloor"
				)

		_message = _message[0][0].outerHTML

		_dialog_buttons = [
											 @_dialogCancel()
											 @_dialogSave()
											 ]

		@_dialog = new BootstrapDialog(
																	title: "Edycja eksponatu"
																	message: _message
																	closable: false
																	buttons: _dialog_buttons
																	)
		@_dialog.realize()
		@_dialog.open()

	_verifyName: (editedName) =>
		namePattern = /^[a-zA-Z\ ]+$/
		if editedName.match(namePattern)
			return true
		else
			return false

	_sendDataToHandler: (wasCanceled) =>
		editedName = jQuery('#name').val();
		floorVal = null
		if jQuery('#radio1').hasClass("active")
			floorVal = 0
		else if jQuery('#radio2').hasClass("active")
			floorVal = 1

		changedData =
			name: editedName
			floor: floorVal

		@_handler(changedData, wasCanceled)

	_dialogCancel: =>
		label: "Anuluj"
		action: (dialog) =>
			@_sendDataToHandler(true)
			dialog.close()

	_dialogSave: =>
		label: "Zapisz"
		action: (dialog) =>
			editedName = jQuery('#name').val();
			if @_verifyName(editedName) is true
				@_sendDataToHandler(false)
				dialog.close()
			else
				jQuery('#error').html('<span style="color: #D8000C">Nazwa może składać się tylko z liter alfabetu angielskiego i spacji.</span>')
