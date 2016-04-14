root = exports ? this
root.ExhibitPanel = class ExhibitPanel extends root.View
  # ========== ATTACHED EVENTS ==========
  # addExhibit          :: (Exhibit, BootstrapDialog) -> _
  # flyToExhibitWithId  :: Int -> _
  # modifyExhibitWithId :: Int -> _
  # =====================================
  constructor: (@_containerId) ->
    # _containerId :: String
    super
    @mapData = new MapDataHandler()
    @_NO_FLOOR = 1 + @mapData.numberOfFloors
    @_ENTER_KEY = 13
    @_lastSearchedText = ''
    @_exhibits = []
    @_exhibitDialog = new root.ExhibitDialog('getHTML?name=exhibitDialog', @addExhibitHandler)
    @_init()


  # _init :: () -> undefined
  _init: =>
    jQuery.getJSON('getHTML?name=exhibitPanel', null, (data) =>
      jQuery(data.html).appendTo(@_containerId)
      @_setExhibitPanelHandlers()
      @_getExhibitElementHTML()
      @filterForCurrentFloor()
    )
    return


  # addExhibitHandler :: (Exhibit, BootstrapDialog) -> undefined
  addExhibitHandler: (data, dialog) =>
    @fireEvents("addExhibit", data, dialog)
    return


  # _setExhibitPanelHandlers :: () -> undefined
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
    return


  # _getExhibitElementHTML :: () -> undefined
  _getExhibitElementHTML: =>
    jQuery.getJSON('getHTML?name=exhibitListElement', null, (data) =>
      @_exhibitElementHTML = data.html
      if @mapData.exhibitsList?
        @replaceExhibits((e.id for e in @mapData.exhibitsList))
    )
    return


  # filterForCurrentFloor :: () -> Context
  filterForCurrentFloor: =>
    jQuery("#filterButtons button").not(":last").removeClass("active")
    jQuery("#filterButtons button:eq(#{@mapData.activeFloor})").addClass("active")
    @refreshExhibitsList()
    @


  # addExhibits :: [Int] -> Context
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
    @


  # replaceExhibits :: [Int] -> Context
  replaceExhibits: (exhibitIdList) =>
    @_exhibits = []
    @addExhibits(exhibitIdList)
    @


  # refreshDialogInstance :: () -> Context
  refreshDialogInstance: =>
    @_exhibitDialog = new root.ExhibitDialog('getHTML?name=exhibitDialog', @addExhibitHandler)
    @


  # refreshExhibitsList :: () -> Context
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
    @


  # _attachHandlersToListElement :: (DOMNode, Int) -> undefined
  _attachHandlersToListElement: (listElement, exhibitId) =>
    listElement
      .querySelector(".exhibitCaption")
      .addEventListener("click", => @fireEvents("modifyExhibitWithId", exhibitId))

    if @_getExhibitFloor(exhibitId) isnt @_NO_FLOOR
      listElement
        .querySelector(".exhibitFlyToButton")
        .addEventListener("click", => @fireEvents("flyToExhibitWithId", exhibitId))
    return


  # _getExhibitFloor :: Int -> Int
  _getExhibitFloor: (exhibitId) =>
    exhibit = @mapData.exhibits[exhibitId]
    if exhibit.frame?.mapLevel?
      exhibit.frame.mapLevel
    else
      @_NO_FLOOR


  # _filterExhibits :: () -> undefined
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


  # _searchBarKeypressHandler :: Event -> undefined
  _searchBarKeypressHandler: (e) =>
    if e.which is @_ENTER_KEY
      jQuery("#exhibitPanel > div.input-group span").click()
    return
