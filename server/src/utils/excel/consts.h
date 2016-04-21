#ifndef UTILS_EXCEL__CONSTS__H
#define UTILS_EXCEL__CONSTS__H

namespace utils {
namespace excel {
namespace consts {

namespace header {
namespace experiment_info {
static const char *experimentName = "Nazwa badania";
static const char *startDate = "Data rozpoczęcia badania";
static const char *finishDate = "Data zakończenia badania";
}  // experiment info

namespace report_info {
static const char *reportID = "ID raportu";
static const char *receiveDate = "Data odebrania raportu";
static const char *beginTime = "Czas rozpoczęcia raportu";
static const char *finishTime = "Czas zakończenia raportu";
}  // report info

namespace survey {
static const char *before = "przed";
static const char *after = "po";

static const char *simplePrefix = "otwarte";
static const char *multiChoicePrefix = "zamknięte";
static const char *sortPrefix = "sortowanie";

static const char *questionInfix = "_";

static const char *optionPrefix = "opcja nr ";
}  // survey

namespace event {
static const char *exhibitName = "Nazwa eksponatu";
static const char *beginTime = "Czas rozpoczęcia";
static const char *endTime = "Czas zakończenia";
static const char *duration = "Czas trwania (sekundy)";
static const char *breakActionPrefix = "przerwa";
static const char *isBreak = "Typ (czy przerwa)";
}  // event
}  // header

namespace legend {
static const char *textAns = "odp. tekstowa";
static const char *numAns = "odp. liczbowa";
}  // legend

static const char *emptyField = " ";
static const char *prefixSuffix = "__";
static const char *delimiter = ";";
}  // consts
}
}

#endif