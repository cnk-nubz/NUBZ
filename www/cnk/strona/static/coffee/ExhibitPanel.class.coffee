root = exports ? this
root.ExhibitPanel = class ExhibitPanel extends root.View
  constructor: ->
    super
    @mapData = new MapDataHandler()
    @_NO_FLOOR = 2
    @_ENTER_KEY = 13
    @_exhibits = []
    @_init()

  _init: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      url: '/getExhibitPanel/'
      success: (data) =>
        jQuery(data).appendTo(@_containerId)
        @_setExhibitPanelHandlers()
        @_getExhibitElementHTML()
    )

  _setExhibitPanelHandlers: =>
    instance = this
    jQuery("#exhibitPanel > button").click( => @fireEvents("addExhibit"))
    jQuery("#exhibitPanel > div.input-group span").click(@_refreshExhibitsList)
    jQuery("#exhibitPanel > div.input-group input").keypress(@_searchBarKeypressHandler)
    jQuery("#filterButtons button").each((index) ->
      jQuery(this).addClass("active")
      jQuery(this).click( ->
        isActive = jQuery(this).hasClass("active")
        jQuery(this).removeClass("active") if isActive
        jQuery(this).addClass("active") unless isActive
        jQuery(this).blur()
        instance._refreshExhibitsList()
      )
    )

  _refreshExhibitsList: =>
    jQuery("#exhibitList .exhibitListElement").each( -> jQuery(this).remove())
    @_filterExhibits()
    for e in @_exhibits when e.visible is true
      exhibit = e.exhibit.clone(true, true)
      jQuery(exhibit).appendTo("#exhibitList")
      jQuery(".exhibitCaption > div", exhibit).shortenText()
    return

  _filterExhibits: =>
    filterButtons = jQuery("#filterButtons button")
    filterButtonsState = (jQuery(b).hasClass("active") for b in jQuery.makeArray(filterButtons))
    for e in @_exhibits
      # filter by search bar text
      searchedText = jQuery("#exhibitSearchBar").val().toLowerCase()
      exhibitsText = jQuery(e.exhibit).data("caption").text().toLowerCase()
      if exhibitsText.indexOf(searchedText) isnt -1
        e.visible = true
        # filter by floor only if exhibit matches the query
        exhibit = e.exhibit.data("exhibit")
        exhibitsFloor = @_getExhibitsFloor(exhibit)
        e.visible = filterButtonsState[exhibitsFloor]
      else
        e.visible = false
    return

  _getExhibitsFloor: (exhibit) =>
    [id, e] = [k, v] for k, v of exhibit
    if e.frame?.mapLevel? then e.frame.mapLevel else @_NO_FLOOR

  _searchBarKeypressHandler: (e) =>
    jQuery("#exhibitPanel > div.input-group span").click() if e.which is @_ENTER_KEY

  _getExhibitElementHTML: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      url: '/getExhibitListElement/'
      success: (data) ->
        @_exhibitElementHTML = data
        @addExhibits(@mapData.exhibits)
    )
    return

  addExhibits: (exhibits) =>
    for id, e of exhibits
      exhibit = {}
      exhibit[id] = e
      exhibitListElement = jQuery(@_exhibitElementHTML)
      caption = jQuery(".exhibitCaption div", exhibitListElement)
      flyToButton = jQuery(".exhibitFlyToButton div", exhibitListElement)
      exhibitListElement.data("caption", caption)
        .data("button", flyToButton)
        .data("exhibit", exhibit)
      caption.html "#{e.name}"

      exhibitsFloor = @_getExhibitsFloor(exhibit)
      if 0 <= exhibitsFloor <= 1
        flyToButton.html exhibitsFloor
        instance = this
        jQuery(".exhibitFlyToButton", exhibitListElement).click( ->
          obj = jQuery(this)
          instance._flyToExhibitHandler(e.frame, obj, instance)
        )

      jQuery(".exhibitCaption", exhibitListElement).click(@_editExhibitHandler)
      @_exhibits.push { exhibit: exhibitListElement, visible: true }
    @_refreshExhibitsList()
    return

  _flyToExhibitHandler: (frame, obj, instance) ->
    element = obj.parent()
    exhibit = element.data("exhibit")
    [id, e] = [k, v] for k, v of exhibit
    instance.fireEvents("flyToExhibit", e)
    return

  _editExhibitHandler: (e) ->
    element = jQuery(this).parent()
    [id, exhibit] = [k, v] for k, v of element.data("exhibit")
    dialog = new root.ExhibitDialog(exhibit.name, exhibit.frame?.mapLevel, (->))
    dialog.show()
    return
