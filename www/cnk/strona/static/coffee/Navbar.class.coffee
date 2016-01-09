root = exports ? this
root.Navbar = class Navbar extends root.View
  constructor: ->
    super
    @navLinks = [
          {
            id: "#nav-1"
            text: 'Podgląd mapy'
            ref: null
          },
          {
            id: "#nav-2"
            text: 'Edycja mapy'
            ref: null
          }
    ]
    @_init()

  _init: =>
    content = @select(@_containerId).append "div"
      .attr(
        id: 'navbar'
      )
      .append "nav"
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

    map = new JustMapPage("#{@_containerId}-a")
    editMap = new EditMapPage("#{@_containerId}-b")
    @navLinks[0].ref = map
    @navLinks[1].ref = editMap
    @_initCss()

    initialPage = 0
    d3.map(@navLinks, (e, id) =>
      content.append "li"
        .attr(
          id: e.id[1..]
        )
        .classed "active", (id is initialPage)
        .append "a"
         .classed "nav-link", true
         .html e.text
         .on "click", =>
           @setActiveButton e.id
           @setActiveView e
    )
    @setActiveView @navLinks[initialPage]
    @_lastClicked = @navLinks[initialPage].id

  _initCss: =>
    navbarStyle = {
      "position": "absolute"
      "top": "0px"
      "left": "0px"
      "height": "50px"
      "width": "100%"
    }
    mapStyle = {
      "position": "relative"
      "padding-top": "50px"
      "height": "100%"
      "width": "100%"
    }
    @select("#{@_containerId}-a")
      .style(mapStyle)
    @select("#{@_containerId}-b")
      .style(mapStyle)
    @select(@_containerId).select("#navbar")
      .style(navbarStyle)
    @

  setActiveButton: (id) ->
    return if @_lastClicked is id
    d3.select "#{id}"
      .classed "active", true
    d3.select "#{@_lastClicked}"
      .classed "active", false
    @_lastClicked = id
    @

  setActiveView: (link) =>
    return if @_activeView?.id is link.id
    @destroyView("content") if @_activeView?
    @addView("content", link.ref)
    link.ref.refresh()
    @_activeView = link
    @
