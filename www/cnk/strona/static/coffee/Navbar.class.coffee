root = exports ? this
root.Navbar = class Navbar extends root.Controller
  constructor: ->
    super
    @_init()

  _init: =>
    content = @select(@_containerId).append "nav"
       .classed "navbar navbar-inverse navbar-fixed-top", true
      .append "div"
       .classed "container", true
      .append "div"
       .classed "collapse navbar-collapse", true
      .append "ul"
       .classed "nav navbar-nav", true
       .attr(
         id: "navbar-list"
       )


    map = new Canvas("#map", "#{@_containerId}-a")
    editMap = new EditableCanvas("#editMap", "#{@_containerId}-b")
    submitHandler = (maps...) =>
      jQuery("#dialogForm").submit((e) =>
        jQuery.ajaxSetup(
          headers: { "X-CSRFToken": getCookie("csrftoken") }
        )
        jQuery.ajax(
          type: "POST"
          url: "/uploadImage/"
          data: new FormData(jQuery("#dialogForm")[0])
          processData: false
          contentType: false

          success: (data) ->
            errorData = [
              {"message": "Mapa piętra została pomyślnie zmieniona", "type": BootstrapDialog.TYPE_SUCCESS, "title": "Sukces"}
              {"message": "Niepoprawny format. Obsługiwane rozszerzenia: .png .jpg .gif .bmp", "type": BootstrapDialog.TYPE_INFO, "title": "Zły format"}
              {"message": "Wystąpił wewnętrzny błąd serwera - spróbuj ponownie za chwilę. Sprawdź czy serwer jest włączony", "type": BootstrapDialog.TYPE_DANGER, "title": "Błąd serwera"}
              {"message": "form error - not POST method", "type": BootstrapDialog.TYPE_DANGER, "title": "not POST method"}
            ]
            if data.err == 1
              data.floorTilesInfo[data.floor] = jQuery.map(data.floorTilesInfo[data.floor], (val) -> [val])
              mapData = new MapDataHandler()
              mapData.floorTilesInfo[data.floor] = data.floorTilesInfo[data.floor]
              map.refreshTiles() for map in maps

            err = data.err - 1
            #close existing dialog
            jQuery.each(BootstrapDialog.dialogs, (id, dialog) ->
              dialog.close()
            )
            BootstrapDialog.show(
              message: errorData[err].message
              title: errorData[err].title
              type: errorData[err].type
            )
          )
        e.preventDefault()
        return
      )

    editMap.on("submit", submitHandler.bind(this, map, editMap)) #just add here new maps if needed
    #HERE ADD NEXT NAV-LINKS
    navLink = [
      {
        id: "#nav-1"
        text: 'Podgląd mapy'
        ref: map
        clicked: true
      },
      {
        id: "#nav-2"
        text: 'Edycja mapy'
        ref: editMap
        clicked: false
      }
    ]

    d3.map(navLink, (e) =>
      content.append "li"
        .attr(
          id: e.id[1..]
        )
        .classed "active", e.clicked
        .append "a"
         .classed "nav-link", true
         .html e.text
         .on "click", =>
           @setActiveButton e.id
           @setActiveView e
    )
    @addView(navLink[0].ref._containerId, navLink[0].ref)
    #initial state
    @_activeView = navLink[0]
    @_lastClicked = navLink[0].id

  setActiveButton: (id) ->
    return if @_lastClicked is id
    d3.select "#{id}"
      .classed "active", true
    d3.select "#{@_lastClicked}"
      .classed "active", false
    @_lastClicked = id
    @

  setActiveView: (link) =>
    return if @_activeView.id is link.id
    @destroyView(@_activeView.ref._containerId)
    @addView(link.ref._containerId, link.ref)
    link.ref.refresh()
    @_activeView = link
    @
