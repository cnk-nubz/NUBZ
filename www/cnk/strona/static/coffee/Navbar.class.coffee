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
    editMap = new EditMapPage("#{@_containerId}-b")
    #HERE ADD NEXT NAV-LINKS
    initialPage = 0
    navLink = [
      {
        id: "#nav-1"
        text: 'Podgląd mapy'
        ref: map
      },
      {
        id: "#nav-2"
        text: 'Edycja mapy'
        ref: editMap
      }
    ]

    d3.map(navLink, (e, id) =>
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
    @setActiveView navLink[initialPage]
    @_lastClicked = navLink[initialPage].id

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
    @destroyView(@_activeView.ref._containerId) if @_activeView?
    @addView(link.ref._containerId, link.ref)
    link.ref.refresh()
    @_activeView = link
    @
