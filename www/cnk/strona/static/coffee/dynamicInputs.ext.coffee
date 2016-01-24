jQuery.fn.dynamicInputs = (offset, keyUpFun, instance) ->

  renumerateInput = (group, index) ->
    jQuery("span", group).text("#{index}.")
    return

  eraseNumber = (group) ->
    jQuery("span", group).text("") 

  createInput = (node, index, addOffset) ->
    el = jQuery(node).data("input#{index}").clone().appendTo(node)
    err = jQuery(node).data("error#{index}").clone().appendTo(node)
    el.addClass("col-sm-offset-#{addOffset}") if addOffset?

    jQuery("input", el).keyup( (e) ->
      keyUpFun(jQuery(this), e, true)
      if jQuery(this).val().length and jQuery(this).parent().next().is(':last-child')
        createInput(node, index, offset)
        jQuery(".input-group:not(:last)", node).each( (index) ->
          renumerateInput(this, index + 1)
        )
        eraseNumber(jQuery(".input-group:last", node))
    )
    jQuery("input", el).blur( ->
      if not jQuery(this).val().length and not jQuery(this).parent().next().is(':last-child')
        #remove error -> remove input -> alter first's element offset
        jQuery(this).parent().next().remove()
        jQuery(this).parent().remove()
        jQuery("div", node).filter(":first").removeClass("col-sm-offset-#{offset}")
        jQuery(".input-group:not(:last)", node).each( (index) ->
          renumerateInput(this, index + 1)
        )
      return
    )

  this.each( (index) ->
    ref = jQuery(this).parent().parent()
    ref.data("error#{index}", jQuery(this).parent().next().detach())
    ref.data("input#{index}", jQuery(this).parent().detach())
    createInput(ref, index, null)
  )
