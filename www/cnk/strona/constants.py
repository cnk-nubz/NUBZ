SIMPLE_QUESTION_DIALOG = {
    "data": [
        [
            "dialog/input.html", {
                "placeholder": "Nazwa pytania",
                "labelText": "Nazwa pytania",
            }
        ],
        [
            "dialog/input.html", {
                "placeholder": "Pytanie dla uzytkownika",
                "labelText": "Pytanie",
            }
        ],
        [
            "dialog/radios.html", {
                "radioGroup": "ansType",
                "labelText": "Typ odpowiedzi",
                "radioSize": "3",
                "textList": ["Liczba", "Zdanie"]
            }
        ]
    ],
    "utils": {
        "text": {
            "title": "Tworzenie nowego pytania",
            "saveButton": "Zapisz",
            "cancelButton": "Anuluj"
        }
    }
}
