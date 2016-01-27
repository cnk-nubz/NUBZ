# -*- coding: UTF-8 -*-
# ================
# THOSE ARE CONSTANTS FOR NORMAL SIZE OF BOOTSTRAP DIALOG
# DEFAULT LABEL SIZE: 3
# DEFAULT INPUT SIZE: 9
# ALL ROWS MUST SUM UP TO 12 OR LESS
# ================
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
                "placeholder": "Pytanie dla użytkownika",
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
            "cancelButton": "Anuluj",
            "inputError": u"Napis może zawierać tylko angielski alfabet oraz spacje.",
            "emptyInputError": "Napis nie może być pusty"
        },
        "style": {
            "inputErrorColor": "#D8000C"
        },
        "regex": {
            "input": "^[a-zA-Z\ ]+$"
        }
    }
}

MULTIPLE_CHOICE_QUESTION_DIALOG = {
    "data": [
        [
            "dialog/input.html", {
                "placeholder": "Nazwa pytania",
                "labelText": "Nazwa pytania",
            }
        ],
        [
            "dialog/input.html", {
                "placeholder": "Pytanie dla użytkownika",
                "labelText": "Pytanie",
            }
        ],
        [
            "dialog/radios.html", {
                "radioGroup": "ansType",
                "labelText": "Wybór",
                "radioSize": "3",
                "textList": ["jednokrotny", "wielokrotny"]
            }
        ],
        [
            "dialog/enumeratedInput.html", {
                "labelText": "Odpowiedzi",
                "placeholder": "Dodaj odpowiedź"
            }
        ]
    ],
    "utils": {
        "text": {
            "title": "Tworzenie pytania zamkniętego",
            "saveButton": "Zapisz",
            "cancelButton": "Anuluj",
            "inputError": "Napis może zawierać tylko angielski alfabet oraz spacje.",
            "emptyInputError": "Napis nie może być pusty",
            "needMultipleAnswerError": "Potrzebne są co najmniej 2 odpowiedzi."
        },
        "style": {
            "inputErrorColor": "#D8000C"
        },
        "regex": {
            "input": "^[a-zA-Z\ ]+$",
            "dynamicInput": "^[a-zA-Z\ ]*$"
        },
        "default": {
            "radioGroup": "ansType",
            "labelSize": "3"
        }
    }
}

SORT_QUESTION_DIALOG = {
    "data": [
        [
            "dialog/input.html", {
                "placeholder": "Nazwa pytania",
                "labelText": "Nazwa pytania",
            }
        ],
        [
            "dialog/input.html", {
                "placeholder": "Pytanie dla użytkownika",
                "labelText": "Pytanie",
            }
        ],
        [
            "dialog/enumeratedInput.html", {
                "labelText": "Odpowiedzi",
                "placeholder": "Dodaj odpowiedź"
            }
        ]
    ],
    "utils": {
        "text": {
            "title": "Tworzenie pytania z sortowaniem",
            "saveButton": "Zapisz",
            "cancelButton": "Anuluj",
            "inputError": "Napis może zawierać tylko angielski alfabet oraz spacje.",
            "emptyInputError": "Napis nie może być pusty.",
            "needMultipleAnswerError": "Potrzebne są co najmniej 2 odpowiedzi."
        },
        "style": {
            "inputErrorColor": "#D8000C"
        },
        "regex": {
            "input": "^[a-zA-Z\ ]+$",
            "dynamicInput": "^[a-zA-Z\ ]*$"
        },
        "default": {
            "labelSize": "3"
        }
    }
}
