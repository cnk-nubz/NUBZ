root = exports ? this
root.ExhibitPanel = class ExhibitPanel extends root.View
  constructor: ->
    super
    @mapData = new MapDataHandler()
    @_NO_FLOOR = 2
    @unmatchedExhibits = [[], [], []] #0, 1, and no floor
    @filteredExhibits = [[], [], []]
    @newExhibitIndex = 0
    @_init()

  _init: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      url: '/getExhibitPanel/'
      success: (data) ->
        instance = this
        jQuery(data).appendTo(@_containerId)
        jQuery("#exhibitPanel > button").click( => @fireEvents("addExhibit"))
        jQuery("#exhibitPanel > div.input-group span").click(@_searchBarClickHandler)
        jQuery("#exhibitPanel > div.input-group input").keypress(@_searchBarKeypressHandler)
        jQuery("#filterButtons button").each((index) ->
          jQuery(this).addClass("active")
          jQuery(this).click(instance._filterButtonHandler(index, instance))
        )
        @_getExhibitElementHTML()
    )

  _searchBarClickHandler: (e) =>
    val = jQuery("#exhibitSearchBar").val()
    instance = this
    #add elements back that match the query
    jQuery("#filterButtons button").each( (floor) ->
      if jQuery(this).hasClass("active")
        instance._matchAndAddExhibitsToDOM(instance.unmatchedExhibits[floor], val)
      else
        instance.filteredExhibits[floor] = instance._mergeArraysById(
          instance.filteredExhibits[floor],
          instance.unmatchedExhibits[floor]
        )
        instance.unmatchedExhibits[floor] = []
    )
    jQuery("#exhibitList .exhibitListElement")
      .each( ->
        obj = jQuery(this)
        floor = obj.data("button").text()
        name = obj.data("caption").text()
        if not (name.indexOf(val) > -1)
          if floor is ''
            instance.unmatchedExhibits[instance._NO_FLOOR].push obj.detach()
          else
            instance.unmatchedExhibits[+floor].push obj.detach()
      )
    return

  _mergeArraysById: (first, second) =>
    res = []
    index0 = 0
    index1 = 0
    while index0 < first.length and index1 < second.length
      exhibitIndex0 = first[index0].data("index")
      exhibitIndex1 = second[index1].data("index")
      if +exhibitIndex0 < +exhibitIndex1
        res.push first[index0++]
      else
        res.push second[index1++]

    while index0 < first.length
      res.push first[index0++]
    while index1 < second.length
      res.push second[index1++]
    res

  _matchAndAddExhibitsToDOM: (exhibits, inputVal) =>
    matchedExhibits = []
    unmatchedExhibits = []
    for e in exhibits
      if e.data("caption").text().indexOf(inputVal) > -1
        matchedExhibits.push e
      else
        unmatchedExhibits.push e
    exhibits = unmatchedExhibits
    @_addExhibitsToDOM matchedExhibits
    return

  _addExhibitsToDOM: (exhibits) =>
    idx = 0
    jQuery("#exhibitList .exhibitListElement")
      .each( ->
        return if idx is exhibits.length
        obj = jQuery(this)
        thisIndex = obj.data "index"
        indexToAdd = exhibits[idx].data "index"
        while +indexToAdd < +thisIndex
          added = exhibits[idx++].insertBefore obj
          jQuery(".exhibitCaption > div", added).shortenText()
          break if idx is exhibits.length
          indexToAdd = exhibits[idx].data "index"
      )
    #add rest of elements to the end of list
    for i in [idx...exhibits.length]
      added = exhibits[i].appendTo jQuery("#exhibitList")
      jQuery(".exhibitCaption > div", added).shortenText()
    return

  _searchBarKeypressHandler: (e) ->
    jQuery("#exhibitPanel > div.input-group span").click() if e.which is 13 #13 = enter

  _filterButtonHandler: (floor, context) ->
    (e) ->
      isActive = jQuery(this).hasClass("active")
      jQuery(this).addClass("active") unless isActive
      jQuery(this).removeClass("active") if isActive
      jQuery(this).blur()
      isActive = not isActive
      instance = context
      if not isActive
        jQuery("#exhibitList .exhibitListElement")
          .each( ->
            exhibitsFloor = jQuery(this).data("button").text()
            exhibitsFloor = instance._NO_FLOOR if exhibitsFloor is ""
            return unless floor is +exhibitsFloor
            instance.filteredExhibits[floor].push jQuery(this).detach()
        )
      else
        inputVal = jQuery("#exhibitSearchBar").val()
        instance._matchAndAddExhibitsToDOM instance.filteredExhibits[floor], inputVal
        instance.unmatchedExhibits[floor] = instance._mergeArraysById(
          instance.unmatchedExhibits[floor],
          instance.filteredExhibits[floor]
        )
        instance.filteredExhibits[floor] = []

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
    exhibitsToAdd = []
    for id, e of exhibits
      tmp = {}
      tmp[id] = e
      el = jQuery(@_exhibitElementHTML)
      caption = jQuery(".exhibitCaption div", el)
      button = jQuery(".exhibitFlyToButton div", el)
      el.data("caption", caption)
        .data("button", button)
        .data("exhibit", tmp)
        .data("index", @newExhibitIndex++)
      caption.html "#{e.name} (#{@newExhibitIndex - 1})"

      exhibitFloor = e.frame?.mapLevel
      if exhibitFloor?
        button.html e.frame.mapLevel
      else
        exhibitFloor = @_NO_FLOOR
        button.html ""

      jQuery(".exhibitCaption", el).click(@_editExhibitHandler)
      instance = this
      jQuery(".exhibitFlyToButton", el).click( ->
        obj = jQuery(this)
        instance._flyToExhibitHandler(e.frame, obj, instance)
      )
      if jQuery("#filterButtons button:eq(#{exhibitFloor})").hasClass("active")
        exhibitsToAdd.push el
      else
        @filteredExhibits[exhibitFloor].push el

    @_addExhibitsToDOM exhibitsToAdd
    return

  _editExhibitHandler: (e) ->
    element = jQuery(this).parent()
    [id, exhibit] = [k, v] for k, v of element.data("exhibit")
    dialog = new root.ExhibitDialog(exhibit.name, exhibit.frame?.mapLevel, (->))
    dialog.show()
    return

  _flyToExhibitHandler: (frame, obj, instance) ->
    element = obj.parent()
    exhibit = element.data("exhibit")
    [id, exhibit] = [k, v] for k, v of exhibit
    instance.fireEvents("flyToExhibit", exhibit.frame)
    return

  _eraseExhibits: =>
    @unmatchedExhibits = [[], [], []]
    @filteredExhibits = [[], [], []]
    jQuery("#exhibitList .exhibitListElement")
      .each( ->
        jQuery(this).remove()
      )
    return
