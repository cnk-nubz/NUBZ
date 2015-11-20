from django import forms

class MapUploadForm(forms.Form):
    image = forms.ImageField()
