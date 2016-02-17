# ======================
# jQuery(obj).shortenText()
# ======================

jQuery.fn.textWidth = ->
	text = jQuery(this).html()
	jQuery(this).html('<span>'+text+'</span>')
	width = jQuery(this).find('span:first').width()
	jQuery(this).html(text)
	width

jQuery.fn.shortenText = ->
  replaceLastWord = (text) ->
    pos = text.lastIndexOf(' ')
    if pos is -1
      '...'
    else
      "#{text.substring(0, pos)}..."

  shorten = (obj) ->
    text = obj[0].innerText
    textWidth = obj.textWidth()
    thisWidth = obj.parent().width()
    while thisWidth < textWidth
      text = replaceLastWord(text)
      obj.html text
      textWidth = obj.textWidth()
    return

  this.each( ->
    shorten(jQuery(this))
  )
