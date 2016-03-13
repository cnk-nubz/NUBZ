root = exports ? this
root.ListView = class ListView extends root.View
  constructor: (@_containerId, @_listType, DOMElements = document.createDocumentFragment()) ->
    # DOMElements are initially in detached state
    # need to implement in subclasses:
    # generateId(id)
    super()
    @_elementsOnList = {}
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
    [].forEach.call(@_listElementsDOM.querySelectorAll("tr"), (element) ->
      jQuery("td:first-child > div", element).shortenText()
    )
    @

  hide: =>
    document.querySelector(@_containerId).removeChild(@_listElementsDOM)
    @

  removeElement: (DOMElement) =>
    TRElement = jQuery(DOMElement).parents("tr")
    viewId = TRElement[0].data
    TRElement.remove()
    @_elementsOnList[viewId] = false

  isElementOnList: (viewId) =>
    @_elementsOnList[viewId] is true

  getAllElements: =>
    @_getOriginalId(el) for el, status of @_elementsOnList when status is true
