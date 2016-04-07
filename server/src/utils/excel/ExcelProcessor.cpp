#include <functional>

#include "ExcelProcessor.h"
#include "Experiment.h"
#include "History.h"
#include "Report.h"
#include "WrapperFactory.h"

namespace utils {
namespace excel {

using namespace std::placeholders;

ExcelProcessor ExcelProcessor::SingleReportProc(db::DatabaseSession &session,
                                                std::int32_t reportID) {
    auto report = repository::Reports{session}.getF(reportID);
    auto experiment = repository::Experiments{session}.get(report.experimentID).value();
    return ExcelProcessor{session, experiment, {report}};
}

ExcelProcessor ExcelProcessor::AllReportsProc(db::DatabaseSession &session,
                                              std::int32_t experimentID) {
    auto experiment = repository::Experiments{session}.getF(experimentID);
    auto reports = repository::Reports{session}.getAllForExperiment(experimentID);
    return ExcelProcessor{session, experiment, reports};
}

ExcelProcessor::ExcelProcessor(db::DatabaseSession &session,
                               const repository::Experiment &experiment,
                               const std::vector<repository::Report> &reports)
    : experiment(experiment), reports(reports), exhibits(repository::Exhibits{session}.getAll()) {
}

void ExcelProcessor::saveReportToFile(const boost::filesystem::path &output) const {
    safeSaving(output, std::bind(&ExcelProcessor::saveReport, this, _1));
}

void ExcelProcessor::saveReport(std::ofstream &output) const {
    auto experimentInfo = ExperimentInfoSection{};
    auto reportInfo = ReportInfoSection(experiment);
    auto historyInfo = HistorySection(experiment, exhibits);

    // header
    {
        auto headers = std::vector<wrapper_ptr>{};
        headers.push_back(experimentInfo.getHeader());
        headers.push_back(reportInfo.getHeader());
        headers.push_back(historyInfo.getHeader());
        auto header = ContainerWrapperFactory::lineWrapper(std::move(headers));
        header->print(output);
    }

    auto sections = std::vector<Wrapper *>{};
    auto newline = WrapperFactory::newlineWrapper();

    auto experimentData = experimentInfo.getEntry(experiment);
    sections.push_back(experimentData.get());

    for (const auto &report : reports) {
        auto reportData = reportInfo.getEntry(report);
        sections.push_back(reportData.get());

        for (const auto &event : report.history) {
            auto eventData = historyInfo.getEntry(event);

            sections.push_back(eventData.get());
            sections.push_back(newline.get());
            auto row = ContainerWrapperFactory::rowWrapper(sections);
            row->print(output);
            sections.pop_back();
            sections.pop_back();
        }

        sections.pop_back();
    }
}

void ExcelProcessor::saveLegendToFile(const boost::filesystem::path &output) const {
    safeSaving(output, std::bind(&ExcelProcessor::saveLegend, this, _1));
}

template <class Q>
std::vector<Q> mergeQuestions(const std::vector<Q> &v1, const std::vector<Q> &v2);

template <class Q>
void addQuestionWithOptions(std::vector<wrapper_ptr> *rows, LegendWrapperFactory::QType type,
                            const std::vector<Q> &questions);

void addRow(std::vector<wrapper_ptr> *rows, wrapper_ptr &&key, wrapper_ptr &&val);

void ExcelProcessor::saveLegend(std::ofstream &output) const {
    auto rows = std::vector<wrapper_ptr>{};

    // simple questions
    {
        auto questions = mergeQuestions(experiment.surveyBefore.simpleQuestions,
                                        experiment.surveyAfter.simpleQuestions);
        for (auto &q : questions) {
            auto key = LegendWrapperFactory::simpleQuestionWrapper(
                q.numberAnswer ? LegendWrapperFactory::SimpleQType::Num
                               : LegendWrapperFactory::SimpleQType::Text,
                q.name);
            auto val = DataWrapperFactory::stringWrapper(q.question);
            addRow(&rows, std::move(key), std::move(val));
        }
    }

    // multiple choice questions
    {
        auto questions = mergeQuestions(experiment.surveyBefore.multipleChoiceQuestions,
                                        experiment.surveyAfter.multipleChoiceQuestions);
        addQuestionWithOptions(&rows, LegendWrapperFactory::QType::Multi, questions);
    }

    // sort questions
    {
        auto questions = mergeQuestions(experiment.surveyBefore.sortQuestions,
                                        experiment.surveyAfter.sortQuestions);
        addQuestionWithOptions(&rows, LegendWrapperFactory::QType::Sort, questions);
    }

    auto legend = ContainerWrapperFactory::colWrapper(std::move(rows));
    legend->print(output);
}

template <class Q>
std::vector<Q> mergeQuestions(const std::vector<Q> &v1, const std::vector<Q> &v2) {
    auto questions = v1;
    for (const auto &newQ : v2) {
        bool exist = false;
        for (const auto &q : questions) {
            if (q.ID == newQ.ID) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            questions.push_back(newQ);
        }
    }
    return questions;
}

template <class Q>
void addQuestionWithOptions(std::vector<wrapper_ptr> *rows, LegendWrapperFactory::QType type,
                            const std::vector<Q> &questions) {
    for (auto &q : questions) {
        auto key = LegendWrapperFactory::questionWrapper(type, q.name);
        auto val = DataWrapperFactory::stringWrapper(q.question);
        addRow(rows, std::move(key), std::move(val));

        for (std::size_t i = 0; i < q.options.size(); i++) {
            auto key = LegendWrapperFactory::questionOptionWrapper(type, q.name, i + 1);
            auto val = DataWrapperFactory::stringWrapper(q.options[i].text);
            addRow(rows, std::move(key), std::move(val));
        }
    }
}

void addRow(std::vector<wrapper_ptr> *rows, wrapper_ptr &&key, wrapper_ptr &&val) {
    auto row = std::vector<wrapper_ptr>{};
    row.push_back(std::move(key));
    row.push_back(std::move(val));
    rows->push_back(ContainerWrapperFactory::rowWrapper(std::move(row)));
};
}
}