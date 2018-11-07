#ifndef TOOLDIVISIONDUPLICATION_H
#define TOOLDIVISIONDUPLICATION_H

#include "abstracttooliterating.h"

class ToolDivisionDuplication: public AbstractToolIterating
{
public:
    ToolDivisionDuplication();

    /// Icon, label and category displayed to the user to describe the tool
    QString getIconName() const override
    {
        return ":/tool/duplicate.svg";
    }

    QString getLabel() const override
    {
        return trUtf8("Dupliquer les divisions...");
    }

    QString getCategory() const override
    {
        return trUtf8("Transformation");
    }

    /// Internal identifier
    QString getIdentifier() const override
    {
        return _isInst ? "inst:divisionDuplication" : "prst:divisionDuplication";
    }

    /// Method executed before the iterating process
    void beforeProcess(IdList ids) override;

    /// Process an element
    void process(SoundfontManager * sm, EltID id, AbstractToolParameters * parameters) override;

protected:
    /// Get the warning to display after the tool is run
    QString getWarning() override;

private:
    bool _isInst;
    QStringList _elementsInError;
    QMutex _mutex;
};

#endif // TOOLDIVISIONDUPLICATION_H