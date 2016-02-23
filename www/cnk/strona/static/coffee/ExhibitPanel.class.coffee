root = exports ? this
root.ExhibitPanel = class ExhibitPanel extends root.View
  # ========== ATTACHED EVENTS ==========
  # addExhibit
  # flyToExhibitWithId: takes exhibit id as second argument
  # modifyExhibitWithId: takes exhibit id as second argument
  # =====================================
  constructor: (@_containerId) ->
    super
    @mapData = new MapDataHandler()
    @_NO_FLOOR = 2
    @_ENTER_KEY = 13
    @_lastSearchedText = ''
    @_exhibits = []
    @_exhibitDialog = new root.ExhibitDialog("getExhibitDialog/", @exhibitAddHandler)
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

  exhibitAddHandler: (data) =>
    @fireEvents("addExhibit", data)

  _setExhibitPanelHandlers: =>
    instance = this
    jQuery("#exhibitPanel #addExhibit").click( =>
      @_exhibitDialog.show()
    )
    jQuery("#exhibitPanel > div.input-group span").click( =>
      @_lastSearchedText = jQuery("#exhibitPanel > div.input-group input").val()
      @_refreshExhibitsList()
    )
    jQuery("#exhibitPanel > div.input-group input").keypress(@_searchBarKeypressHandler)
    jQuery("#filterButtons button").each((index) ->
      jQuery(this).addClass("active")
      jQuery(this).click( ->
        isActive = jQuery(this).hasClass("active")
        if isActive
          jQuery(this).removeClass("active")
        else
          jQuery(this).addClass("active")
        jQuery(this).blur()
        instance._refreshExhibitsList()
      )
    )

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
        @addExhibits((id for id, _ of @mapData.exhibits))
    )
    return

  addExhibits: (exhibitIdList) =>
    for id in exhibitIdList
      e = @mapData.exhibits[id]
      exhibitListElement = jQuery(@_exhibitElementHTML)
      caption = jQuery(".exhibitCaption div", exhibitListElement)
      flyToButton = jQuery(".exhibitFlyToButton div", exhibitListElement)
      exhibitListElement.data("caption", caption)
        .data("button", flyToButton)
        .data("exhibitId", id)
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
      jQuery(".exhibitCaption", exhibitListElement).click( do (id) => ( => @fireEvents("modifyExhibitWithId", id)))
      @_exhibits.push { exhibit: exhibitListElement, visible: true }
    @_refreshExhibitsList()
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
      exhibitText = jQuery(e.exhibit).data("caption").text().toLowerCase()
      if exhibitsText.indexOf(searchedText) isnt -1
        # filter by floor only if exhibit matches the query
        exhibitId = e.exhibit.data("exhibitsId")
        exhibitFloor = @_getExhibitFloor(exhibitsId)
        e.visible = filterButtonsState[exhibitsFloor]
      else
        e.visible = false
    return

  _getExhibitFloor: (exhibitId) =>
    exhibit = @mapData.exhibits[exhibitId]
    if exhibit.frame?.mapLevel? then exhibit.frame.mapLevel else @_NO_FLOOR

  _flyToExhibitHandler: (obj, instance) ->
    element = obj.parent()
    exhibitId = element.data("exhibitId")
    instance.fireEvents("flyToExhibitWithId", exhibitId)
    return

  _filterExhibits: =>
    filterButtons = jQuery("#filterButtons button")
    filterButtonsState = (jQuery(b).hasClass("active") for b in jQuery.makeArray(filterButtons))
    searchedText = @_lastSearchedText.toLowerCase()
    for e in @_exhibits
      # filter by search bar text
      exhibitText = jQuery(e.exhibit).data("caption").text().toLowerCase()
      if exhibitText.indexOf(searchedText) isnt -1
        # filter by floor only if exhibit matches the query
        exhibitId = e.exhibit.data("exhibitId")
        exhibitFloor = @_getExhibitFloor(exhibitId)
        e.visible = filterButtonsState[exhibitFloor]
      else
        e.visible = false
    return

  _searchBarKeypressHandler: (e) =>
    jQuery("#exhibitPanel > div.input-group span").click() if e.which is @_ENTER_KEY
