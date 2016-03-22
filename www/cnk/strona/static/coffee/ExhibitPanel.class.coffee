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
    @_exhibitDialog = new root.ExhibitDialog('getHTML?name=exhibitDialog', @addExhibitHandler)
    @_init()

  _init: =>
    jQuery.getJSON('getHTML?name=exhibitPanel', null, (data) =>
      jQuery(data.html).appendTo(@_containerId)
      @_setExhibitPanelHandlers()
      @_getExhibitElementHTML()
      @filterForCurrentFloor()
    )
    return

  addExhibitHandler: (data) =>
    @fireEvents("addExhibit", data)

  _setExhibitPanelHandlers: =>
    instance = this
    jQuery("#exhibitPanel #addExhibit").click( =>
      @_exhibitDialog.show()
      @refreshDialogInstance()
    )
    jQuery("#exhibitPanel > div.input-group span").click( =>
      @_lastSearchedText = jQuery("#exhibitPanel > div.input-group input").val()
      @refreshExhibitsList()
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
        instance.refreshExhibitsList()
      )
    )

  _getExhibitElementHTML: =>
    jQuery.getJSON('getHTML?name=exhibitListElement', null, (data) =>
      @_exhibitElementHTML = data.html
      @addExhibits((id for id, _ of @mapData.exhibits))
    )
    return

  filterForCurrentFloor: () =>
    jQuery("#filterButtons button:eq(#{1 - @mapData.activeFloor})").removeClass("active")
    jQuery("#filterButtons button:eq(#{@mapData.activeFloor})").addClass("active")
    @refreshExhibitsList()

  addExhibits: (exhibitIdList) =>
    for id in exhibitIdList
      e = @mapData.exhibits[id]
      exhibitListElement = jQuery(@_exhibitElementHTML)[0]
      exhibitCaption = exhibitListElement.querySelector(".exhibitCaption div")
      exhibitCaption.innerHTML = e.name
      exhibitListElement.data =
        caption: e.name
        exhibitId: id

      @_exhibits.push { listElement: exhibitListElement, visible: true }
    @refreshExhibitsList()
    return

  replaceExhibits: (exhibitIdList) =>
    @_exhibits = []
    @addExhibits(exhibitIdList)
    @

  refreshDialogInstance: =>
    @_exhibitDialog = new root.ExhibitDialog('getHTML?name=exhibitDialog', @addExhibitHandler)

  refreshExhibitsList: =>
    jQuery("#exhibitList .exhibitListElement").each( -> jQuery(this).remove())
    @_filterExhibits()
    fragment = document.createDocumentFragment()
    for e in @_exhibits when e.visible is true
      listElement = e.listElement.cloneNode(true)
      flyToButton = listElement.querySelector(".exhibitFlyToButton div")
      exhibitFloor = @_getExhibitFloor(e.listElement.data.exhibitId)
      if exhibitFloor isnt @_NO_FLOOR
        flyToButton.innerHTML = exhibitFloor
        flyToButton.className = "clickable"
      else
        flyToButton.innerHTML = ""
        flyToButton.className = ""
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
