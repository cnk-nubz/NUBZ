root = exports ? this
root.Navbar = class Navbar extends root.ControllerEmitter
  constructor: ->
    super

  spawn: (selection) =>
    d3.select "body"
      .append -> selection.node()
    nav = selection
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

    #HERE ADD NEXT NAV-LINKS

    navLink = [
      {
        id: 'nav-1'
        text: 'Podgląd mapy'
        onclick: =>
          @setActiveView "justMap"
          @destroyView "editMap"
          @materializeView "justMap"
          @refreshView "justMap"
          return
      },
      {
        id: "nav-2"
        text: 'Edycja mapy'
        onclick: =>
          @setActiveView "editMap"
          @destroyView "justMap"
          @materializeView "editMap"
          @refreshView "editMap"
          return
      }
    ]

    d3.map(navLink, (e) =>
      nav.append "li"
        .attr(
          id: e.id
        )
        .append "a"
         .classed "nav-link", true
         .html e.text
         .on "click", =>
           @setActiveButton e.id
           e.onclick()
    )
    #initial state
    d3.select "#nav-1"
      .classed "active", true
    @refreshView "justMap"
    @_lastClicked = "nav-1"
    @setActiveView "justMap"

  setActiveButton: (id) ->
    d3.select "##{id}"
      .classed "active", true
    d3.select "##{@_lastClicked}"
      .classed "active", false
    @_lastClicked = id
    @
