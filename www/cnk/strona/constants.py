SIMPLE_QUESTION_DIALOG = [
    [
        "input", {
            "placeholder": "Nazwa pytania",
            "labelText": "Nazwa pytania",
        }
    ],
    [
        "input", {
            "placeholder": "Pytanie dla uzytkownika",
            "labelText": "Pytanie",
        }
    ],
    [
        "radios", {
            "radioGroup": "test",
            "labelText": "Typ odpowiedzi",
            "radioSize": "3",
            "idList": [["t1", "Liczba"], ["t2", "Zdanie"]]
        }
    ]
]

EXHIBIT_DIALOG = [
    [
        "input", {
            "labelText": "Nazwa",
            "withError": True
        }
    ],
    [
        "radios", {
            "radioGroup": "rFloor",
            "labelText": "Pietro",
            "textList": ["0", "1", "brak"]
        }
    ]
]
