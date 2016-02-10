root = exports ? this
root.ExhibitPanel = class ExhibitPanel extends root.View
  constructor: ->
    super
    @mapData = new MapDataHandler()
    @_NO_FLOOR = 2
    @_ENTER_KEY = 13
    @_lastSearchedText = ''
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

  addExhibits: (exhibits) =>
    for id, e of exhibits
      exhibitListElement = jQuery(@_exhibitElementHTML)
      caption = jQuery(".exhibitCaption div", exhibitListElement)
      flyToButton = jQuery(".exhibitFlyToButton div", exhibitListElement)
      exhibitListElement.data("caption", caption)
        .data("button", flyToButton)
        .data("exhibitsId", id)
      caption.html "#{e.name}"

      exhibitFloor = @_getExhibitFloor(id)
      if exhibitFloor isnt @_NO_FLOOR
        flyToButton.html exhibitFloor
        flyToButton.addClass "clickable"
        instance = this
        jQuery(".exhibitFlyToButton", exhibitListElement).click( ->
          obj = jQuery(this)
          instance._flyToExhibitHandler(obj, instance)
        )
      jQuery(".exhibitCaption", exhibitListElement).click( do (id) => (=> @fireEvents("modifyExhibit", id)))
      @_exhibits.push { exhibit: exhibitListElement, visible: true }
    @_refreshExhibitsList()
    return

  _getExhibitFloor: (exhibitsId) =>
    exhibit = @mapData.exhibits[exhibitsId]
    if exhibit.frame?.mapLevel? then exhibit.frame.mapLevel else @_NO_FLOOR

  _flyToExhibitHandler: (obj, instance) ->
    element = obj.parent()
    exhibitsId = element.data("exhibitsId")
    instance.fireEvents("flyToExhibit", exhibitsId)
    return

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
    searchedText = @_lastSearchedText.toLowerCase()
    for e in @_exhibits
      # filter by search bar text
      exhibitsText = jQuery(e.exhibit).data("caption").text().toLowerCase()
      if exhibitsText.indexOf(searchedText) isnt -1
        # filter by floor only if exhibit matches the query
        exhibitsId = e.exhibit.data("exhibitsId")
        exhibitsFloor = @_getExhibitFloor(exhibitsId)
        e.visible = filterButtonsState[exhibitsFloor]
      else
        e.visible = false
    return

  _searchBarKeypressHandler: (e) =>
    if e.which is @_ENTER_KEY
      @_lastSearchedText = jQuery("#exhibitPanel > div.input-group input").text()
      jQuery("#exhibitPanel > div.input-group span").click()

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
