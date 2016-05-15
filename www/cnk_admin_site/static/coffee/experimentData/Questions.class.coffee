root = exports ? this
root.Questions = class Questions extends root.ExperimentData
  ###
  # data Question =
  #   MultipleChoiceQuestion
  # | SimpleQuestion
  # | SortQuestion
  ###
  ###
  # type MultipleChoiceQuestion = {
  #   isNew      :: Boolean,
  #   name       :: String,
  #   options    :: [String],
  #   question   :: String,
  #   questionId :: Int,
  #   type       :: Int
  # }
  ###
  ###
  # type SimpleQuestion = {
  #   answerType :: Int,
  #   isNew      :: Boolean,
  #   name       :: String,
  #   question   :: String,
  #   questionId :: Int,
  #   type       :: Int
  # }
  ###
  ###
  # type SortQuestion = {
  #   isNew        :: Boolean,
  #   name         :: String,
  #   options      :: [String],
  #   question     :: String,
  #   questionId   :: Int,
  #   singleAnswer :: Boolean,
  #   type         :: Int
  # }
  ###
  # constructor :: [Question] -> Context
  constructor: (_list = []) ->
    super _list
    @_simpleQuestionDialog = new root.SimpleQuestionDialog()
    @_multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog()
    @_sortQuestionDialog = new root.SortQuestionDialog()
    ###
    # _questionType :: (JsObject :: (Int, {name :: String, dialog :: BootstrapDialog}))
    ###
    @_questionType =
      0:
        name: 'otwarte'
        dialog: @_simpleQuestionDialog
      1:
        name: 'zamknięte'
        dialog: @_multipleChoiceQuestionDialog
      2:
        name: 'sortowanie'
        dialog: @_sortQuestionDialog

  ###
  # type QuestionListElement = {
  #   name     :: String,
  #   typeName :: String
  #   id       :: Int
  #   isNew    :: Boolean
  #   type     :: Int
  # }
  ###
  # _elementListFormat :: String -> QuestionListElement
  _elementListFormat: (viewId) =>
    name: @escapeText(@_elementsDict[viewId].name)
    typeName: @_questionType[@_elementsDict[viewId].type].name
    id: @_elementsDict[viewId].questionId
    isNew: @_elementsDict[viewId].isNew
    type: @_elementsDict[viewId].type


  # getFilledDialog :: (String, jsObject) -> BootstrapDialog
  getFilledDialog: (viewId, options = {}) =>
    dialog = @_questionType[@_elementsDict[viewId].type].dialog
    jQuery.extend(dialog.options, options)
    dialog.bindData(@_elementsDict[viewId])
    dialog


  # _getViewId :: Int -> String
  _getViewId: (index) =>
    JSON.stringify(
      type: @_orderedList[index].type
      id: @_orderedList[index].questionId
    )
