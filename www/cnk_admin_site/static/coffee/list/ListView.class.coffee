root = exports ? this
root.ListView = class ListView extends root.View
  # ======== ABSTRACT FUNCTIONS ========
  # getAllElements :: () -> [TODO]
  # ====================================
  # constructor :: (String, String, DocumentFragment)
  constructor: (@_containerId, @_listType, DOMElements = document.createDocumentFragment()) ->
    # DOMElements are initially in detached state
    super()
    # _elementsOnList :: [String]
    @_elementsOnList = []
    @_listElementsDOM = @_wrapElements(DOMElements)


  # addElement :: DocumentFragment -> Context
  addElement: (element) =>
    @_elementsOnList.push(element.querySelector("tr").data)
    @_listElementsDOM.querySelector("tbody").appendChild(element)
    @


  # replaceElements :: DocumentFragment -> Context
  replaceElements: (DOMElements) =>
    @hide()
    @_listElementsDOM = @_wrapElements(DOMElements)
    @show()
    @


  # _wrapElement :: DocumentFragment -> DocumentFragment
  _wrapElements: (DOMElements) =>
    [].forEach.call(DOMElements.querySelectorAll("tr"), (element) =>
      @_elementsOnList.push(element.data)
    )
    container = jQuery(root.structures.HTML.tableList).addClass(@_listType)
    container = container[0]
    container.querySelector("tbody").appendChild(DOMElements)
    container


  # show :: () -> Context
  show: =>
    document.querySelector(@_containerId).appendChild(@_listElementsDOM)
    [].forEach.call(@_listElementsDOM.querySelectorAll("tr"), (element) ->
      jQuery("td:first-child > div", element).shortenText()
    )
    @


  # hide :: () -> Context
  hide: =>
    document.querySelector(@_containerId).removeChild(@_listElementsDOM)
    @


  # removeElement :: String -> Context
  removeElement: (viewId) =>
    @_elementsOnList.splice(index, 1) for el, index in @_elementsOnList when el is viewId
    @


  # isElementOnList :: String -> Boolean
  isElementOnList: (viewId) =>
    viewId in @_elementsOnList
