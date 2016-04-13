# -*- coding: UTF-8 -*-
# ================
# THOSE ARE CONSTANTS FOR NORMAL SIZE OF BOOTSTRAP DIALOG
# DEFAULT LABEL SIZE: 3
# DEFAULT INPUT SIZE: 9
# ALL ROWS MUST SUM UP TO 12 OR LESS
# ================

INTERNAL_ERROR = "Wystąpił nieoczekiwany błąd. Spróbuj ponownie za chwilę."
EXHIBIT_LIST_ERROR = "Nie udało sie pobrać listy eksponatów. Spróbuj ponownie za chwilę."
NO_SUCH_REPORT_ERROR = "Nie istnieje raport o numerze"
NO_SUCH_EXPERIMENT_ERROR = "Nie istnieje badanie o numerze"

DEFAULT_CONSTANTS = {
    "utils": {
        "text": {
            "emptyInputError": "Napis nie może być pusty",
            "nameDuplicatedError": "Taka nazwa już istnieje.",
            "saveButton": "Zapisz",
            "cancelButton": "Anuluj",
            "deleteButton": "Usuń",
        }
    }
}

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
            "titleNew": "Tworzenie pytania otwartego",
            "title": "Pytanie otwarte"
        },
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
            "titleNew": "Tworzenie pytania zamkniętego",
            "title": "Pytanie zamknięte",
            "needMultipleAnswerError": "Potrzebne są co najmniej 2 odpowiedzi.",
            "optionDuplicatedError": "Ta opcja już się pojawiła na liście."
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
            "titleNew": "Tworzenie pytania z sortowaniem",
            "title": "Pytanie z sortowaniem",
            "needMultipleAnswerError": "Potrzebne są co najmniej 2 odpowiedzi.",
            "optionDuplicatedError": "Ta opcja już się pojawiła na liście."
        },
        "default": {
            "labelSize": "3"
        }
    }
}

NEW_ACTION_DIALOG = {
    "data": [
        [
            "dialog/input.html", {
                "placeholder": "Akcja",
                "labelText": "Akcja",
            }
        ]
    ],
    "utils": {
        "text": {
            "titleNew": "Tworzenie nowej akcji",
            "title": "Akcja"
        }
    }
}

EXHIBIT_DIALOG = {
    "data": [
        [
            "dialog/input.html", {
                "placeholder": "Nazwa eksponatu",
                "labelText": "Nazwa",
            }
        ],
        [
            "dialog/radios.html", {
                "radioGroup": "floorNum",
                "labelText": "Piętro",
                "radioSize": "2",
                "textList": ["0", "1", "brak"]
            }
        ],
        [
            "dialog/popoverButton.html", {
                "labelText": "Kolor",
                "popoverTitle": "Wybierz kolor",
                "popoverPlacement": "right",
                "popoverText": "",
            }
        ]
    ],
    "utils": {
        "text": {
            "title": "Tworzenie eksponatu",
            "editTitle": "Edycja eksponatu",
            "deleteButtonHtml": '<button class="btn btn-danger delete-button">Usuń</button>',
        },
    }
}

CHANGE_EXPERIMENT_NAME_DIALOG = {
    "data": [
        [
            "dialog/input.html", {
                "labelText": "Nowa nazwa"
            }
        ]
    ],
    "utils": {
        "text": {
            "title": "Klonowanie badania",
        }
    }
}

CONFIRM_EXHIBIT_DEL_DIALOG = {
    "data": [],
    "utils": {
        "text": {
            "title": "Potwierdzenie usuwania eksponatu",
            "question": "Czy na pewno chcesz usunąć eksponat? Tej akcji nie będzie można cofnąć.",
            "confirmButton": "Tak",
        }
    }
}

EXPERIMENT_CONFIRMATION_MESSAGES = {
    'finishExperiment': 'Czy chcesz zakończyć dane badanie? Na żadnym tablecie nie będzie można rozpocząć tego badania. Tej operacji nie da się cofnąć.',
    'activateExperiment': 'Czy chcesz aktywować dane badanie? Nie będzie ono już edytowalne. Tej operacji nie da się cofnąć.',
    'cancelButton': 'Anuluj',
    'confirmButton': 'OK',
    'title': 'Potwierdzenie'
}

POPOVER_COLORS = ['#64B3E0', '#9DE35A', '#FEE161', '#FEC172', '#FD605E', '#9E45B9',
'#499CCA', '#6FC238', '#F2D130', '#FEAA3A', '#FE2D21', '#6C2185',
'#357DA3', '#7BAF3E', '#E3B902', '#EEA02E', '#CF232C', '#55146C',
'#175879', '#578826', '#C79403', '#D27F15', '#AE1A15', '#3C0B49']

to_merge_with_defaults = [
    SIMPLE_QUESTION_DIALOG,
    MULTIPLE_CHOICE_QUESTION_DIALOG,
    SORT_QUESTION_DIALOG,
    NEW_ACTION_DIALOG,
    EXHIBIT_DIALOG,
    CHANGE_EXPERIMENT_NAME_DIALOG,
    CONFIRM_EXHIBIT_DEL_DIALOG,
]

def merge_dicts(a, b, path=None):
    if path is None: path = []
    for key in b:
        if key in a:
            if isinstance(a[key], dict) and isinstance(b[key], dict):
                merge_dicts(a[key], b[key], path + [str(key)])
            elif a[key] != b[key]:
                b[key] = a[key]
        else:
            a[key] = b[key]
    return a

for k in to_merge_with_defaults:
    k = merge_dicts(dict(k), dict(DEFAULT_CONSTANTS))
