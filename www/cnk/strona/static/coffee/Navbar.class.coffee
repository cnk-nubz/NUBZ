root = exports ? this
root.Navbar = class Navbar extends root.View
  constructor: ->
    @appearance = new Appearance()
    super
    #here add next navbar links
    @navLinks = [
          {
            id: "#nav-1"
            text: 'Podgląd mapy'
            ref: root.JustMapPage
            refDiv: "#{@_containerId}-a"
          },
          {
            id: "#nav-2"
            text: 'Edycja mapy'
            ref: root.EditMapPage
            refDiv: "#{@_containerId}-b"
          },
    ]
    @jumpLinks = [
      {
        text: 'Badania'
        href: '/badania'
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

    d3.map(@jumpLinks, (e, id) =>
      content.append "li"
        .append "a"
          .html e.text
          .on "click", =>
            location.href = e.href
    )

    @setActiveView @navLinks[initialPage]
    @_lastClicked = @navLinks[initialPage].id

  _initCss: =>
    navbarStyle = {
      "position": "absolute"
      "top": "0px"
      "left": "0px"
      "height": @appearance.navbar.height
      "width": "100%"
    }
    mapStyle = {
      "position": "relative"
      "padding-top": @appearance.navbar.height
      "height": "100%"
      "width": "100%"
      "-moz-box-sizing": "border-box"
      "box-sizing": "border-box"
    }
    @select(@navLinks[0].refDiv)
      ?.style(mapStyle)
    @select(@navLinks[1].refDiv)
      ?.style(mapStyle)
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
    view = new link.ref(link.refDiv)
    @addView("content", view)
    @_initCss()
    view.refresh()
    @_activeView = link
    @
