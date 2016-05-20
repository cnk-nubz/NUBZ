# -*- coding: UTF-8 -*-
# ================
# THOSE ARE CONSTANTS FOR NORMAL SIZE OF BOOTSTRAP DIALOG
# DEFAULT LABEL SIZE: 3
# DEFAULT INPUT SIZE: 9
# ALL ROWS MUST SUM UP TO 12 OR LESS
# ================

INTERNAL_ERROR = "Wystąpił nieoczekiwany błąd. Spróbuj ponownie za chwilę."
NO_SUCH_FLOOR_ERROR = "Nie istnieje piętro o danym numerze"
ELEMENT_IN_USE_ERROR = "Usuwany element jest przypisany przynajmniej do jednego badania."
DUPLICATE_NAME_ERROR = "Taka nazwa już istnieje."
INVALID_DATA_ERROR = "Krytyczny błąd twórców strony. Prosimy o kontakt."
NEW_FLOOR_IMAGE_SUCCESS = {
    "message": "Pomyślnie zmieniono mapę piętra nr",
     "title": "Sukces",
     "type": "success"}
NEW_FLOOR_IMAGE_WRONG_FORMAT = "Niepoprawny format. Obsługiwane rozszerzenia: .png .jpg .gif .bmp"

DEFAULT_CONSTANTS = {
    "utils": {
        "text": {
            "emptyInputError": "Napis nie może być pusty",
            "nameDuplicatedError": DUPLICATE_NAME_ERROR,
            "saveButton": "Zapisz",
            "cancelButton": "Anuluj",
            "deleteButton": "Usuń",
            "confirmationButton": "OK",
            "confirmationTitle": "Potwierdzenie"
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
            "title": "Pytanie otwarte",
            "confirmationMessage": "Czy chcesz usunąć dane pytanie? Tej operacji nie można cofnąć"
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
            "optionDuplicatedError": "Ta opcja już się pojawiła na liście.",
            "confirmationMessage": "Czy chcesz usunąć dane pytanie? Tej operacji nie można cofnąć"
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
            "optionDuplicatedError": "Ta opcja już się pojawiła na liście.",
            "confirmationMessage": "Czy chcesz usunąć dane pytanie? Tej operacji nie można cofnąć"
        },
        "default": {
            "labelSize": "3"
        }
    }
}

ACTION_DIALOG = {
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
            "title": "Akcja",
            "confirmationMessage": "Czy chcesz usunąć daną akcję? Tej operacji nie można cofnąć."
        }
    }
}


POPOVER_COLORS = ['#64B3E0', '#9DE35A', '#FEE161', '#FEC172', '#FD605E', '#9E45B9',
'#499CCA', '#6FC238', '#F2D130', '#FEAA3A', '#FE2D21', '#6C2185',
'#357DA3', '#7BAF3E', '#E3B902', '#EEA02E', '#CF232C', '#55146C',
'#175879', '#578826', '#C79403', '#D27F15', '#AE1A15', '#3C0B49']


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
                "textList": ["brak"]
            }
        ],
        [
            "dialog/popoverButton.html", {
                "labelText": "Kolor",
                "popoverTitle": "Wybierz kolor",
                "popoverPlacement": "right",
                "colorsList": POPOVER_COLORS
            }
        ]
    ],
    "utils": {
        "text": {
            "title": "Tworzenie eksponatu",
            "editTitle": "Edycja eksponatu",
            "confirmationMessage": "Czy chcesz usunąć dany eksponat? Tej operacji nie da się cofnąć."
        },
        "css": {
            "defaultColor": "#9DE35A"
        }
    }
}

CLONE_EXPERIMENT_DIALOG = {
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


START_EXPERIMENT_CONFIRMATION = {
    "utils": {
        "text": {
            'confirmationMessage': 'Czy chcesz aktywować dane badanie? Nie będzie ono już edytowalne. Tej operacji nie da się cofnąć.'
        }
    }
}


FINISH_EXPERIMENT_CONFIRMATION = {
    "utils": {
        "text": {
            'confirmationMessage': 'Czy chcesz zakończyć dane badanie? Na żadnym tablecie nie będzie można rozpocząć tego badania. Tej operacji nie da się cofnąć.'
        }
    }
}


REMOVE_EXPERIMENT_CONFIRMATION = {
    "utils": {
        "text": {
            'confirmationMessage': 'Czy chcesz usunąć dane badanie? Wszystkie raporty dotyczące tego badania zostaną usunięte. Pytania oraz akcje z danego badania nie zostaną usunięte. Tej operacji nie można cofnąć'
        }
    }
}


to_merge_with_defaults = [
    SIMPLE_QUESTION_DIALOG,
    MULTIPLE_CHOICE_QUESTION_DIALOG,
    SORT_QUESTION_DIALOG,
    ACTION_DIALOG,
    EXHIBIT_DIALOG,
    CLONE_EXPERIMENT_DIALOG,
    START_EXPERIMENT_CONFIRMATION,
    FINISH_EXPERIMENT_CONFIRMATION,
    REMOVE_EXPERIMENT_CONFIRMATION
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
