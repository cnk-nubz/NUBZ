from django import template
import sys
register = template.Library()

@register.simple_tag(takes_context=True)
def updateContext(context, myDict):
	for k, v in myDict:
		context[k] = v
	return ''
