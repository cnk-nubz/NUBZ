root = exports ? this
root.ListView = class ListView extends root.View
  constructor: (@_containerId, DOMElements = document.createDocumentFragment()) ->
    # DOMElements are initially in detached state
    # need to implement in subclasses:
    # generateId(element)
    super()
    @_elementsOnList = {}
    @_newElementsId = {}
    @_listElementsDOM = @_wrapElements(DOMElements)

  addElement: (element) =>
    @_elementsOnList[element.querySelector("tr").data] = true
    @_listElementsDOM.querySelector("tbody").appendChild(element)
    @

  replaceElements: (DOMElements) =>
    @hide()
    @_listElementsDOM = @_wrapElements(DOMElements)
    @show()
    @

  _wrapElements: (DOMElements) =>
    [].forEach.call(DOMElements.querySelectorAll("tr"), (element) =>
      @_elementsOnList[element.data] = true
    )
    container = jQuery(root.HTML.tableList).addClass(@_listType)
    container = container[0]
    container.querySelector("tbody").appendChild(DOMElements)
    container

  show: =>
    document.querySelector(@_containerId).appendChild(@_listElementsDOM)
    @

  hide: =>
    document.querySelector(@_containerId).removeChild(@_listElementsDOM)
    @

  isElementOnList: (id) =>
    @_elementsOnList[id] is true
