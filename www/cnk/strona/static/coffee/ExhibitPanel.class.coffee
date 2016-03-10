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
      exhibitListElement = jQuery(@_exhibitElementHTML)[0]
      exhibitCaption = exhibitListElement.querySelector(".exhibitCaption div")
      exhibitCaption.innerHTML = e.name
      exhibitListElement.data =
        caption: e.name
        exhibitId: id

      exhibitFloor = @_getExhibitFloor(id)
      if exhibitFloor isnt @_NO_FLOOR
        flyToButton = exhibitListElement.querySelector(".exhibitFlyToButton div")
        flyToButton.innerHTML = exhibitFloor
        flyToButton.className = "clickable"
      @_exhibits.push { listElement: exhibitListElement, visible: true }
    @_refreshExhibitsList()
    return

  _refreshExhibitsList: =>
    jQuery("#exhibitList .exhibitListElement").each( -> jQuery(this).remove())
    @_filterExhibits()
    fragment = document.createDocumentFragment()
    for e in @_exhibits when e.visible is true
      listElement = e.listElement.cloneNode(true)
      @_attachHandlersToListElement(listElement, e.listElement.data.exhibitId)
      fragment.appendChild(listElement)

    jQuery("#exhibitList").append(fragment)
    jQuery("#exhibitList .exhibitCaption > div").each( ->
      jQuery(this).shortenText()
    )
    return

  _attachHandlersToListElement: (listElement, exhibitId) =>
    listElement
      .querySelector(".exhibitCaption")
      .addEventListener("click", => @fireEvents("modifyExhibitWithId", exhibitId))
      
    if @_getExhibitFloor(exhibitId) isnt @_NO_FLOOR
      listElement
        .querySelector(".exhibitFlyToButton")
        .addEventListener("click", => @fireEvents("flyToExhibitWithId", exhibitId))
    return

  _filterExhibits: =>
    filterButtons = jQuery("#filterButtons button")
    filterButtonsState = (jQuery(b).hasClass("active") for b in jQuery.makeArray(filterButtons))
    searchedText = @_lastSearchedText.toLowerCase()
    for e in @_exhibits
      # filter by search bar text
      exhibitText = e.listElement.data.caption.toLowerCase()
      if exhibitsText.indexOf(searchedText) isnt -1
        # filter by floor only if exhibit matches the query
        exhibitId = e.listElement.data.exhibitId
        exhibitFloor = @_getExhibitFloor(exhibitsId)
        e.visible = filterButtonsState[exhibitsFloor]
      else
        e.visible = false
    return

  _getExhibitFloor: (exhibitId) =>
    exhibit = @mapData.exhibits[exhibitId]
    if exhibit.frame?.mapLevel? then exhibit.frame.mapLevel else @_NO_FLOOR

  _filterExhibits: =>
    filterButtons = jQuery("#filterButtons button")
    filterButtonsState = (jQuery(b).hasClass("active") for b in jQuery.makeArray(filterButtons))
    searchedText = @_lastSearchedText.toLowerCase()
    for e in @_exhibits
      # filter by search bar text
      exhibitText = e.listElement.data.caption.toLowerCase()
      if exhibitText.indexOf(searchedText) isnt -1
        # filter by floor only if exhibit matches the query
        exhibitId = e.listElement.data.exhibitId
        exhibitFloor = @_getExhibitFloor(exhibitId)
        e.visible = filterButtonsState[exhibitFloor]
      else
        e.visible = false
    return

  _searchBarKeypressHandler: (e) =>
    jQuery("#exhibitPanel > div.input-group span").click() if e.which is @_ENTER_KEY
