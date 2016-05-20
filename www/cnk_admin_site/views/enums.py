from enum import Enum

class QuestionType(Enum):
    SIMPLE = 0
    MULTIPLE = 1
    SORT = 2


class ActiveLink(Enum):
    JUST_MAP = '0'
    EDIT_MAP = '1'
    QUESTIONS_ACTIONS_PAGE = '2'
    EXPERIMENTS_PAGE = '3'
