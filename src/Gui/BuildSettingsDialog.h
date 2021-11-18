#ifndef BUILDSETTINGSDIALOG_H
#define BUILDSETTINGSDIALOG_H

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QComboBox;


class BuildSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    BuildSettingsDialog(QWidget *parent = nullptr);

    QString computeMode() const;
    void setComputeMode(const QString &newComputeMode);

    int depth() const;
    void setDepth(int newDepth);

    int memorySize() const;
    void setMemorySize(int newMemorySize);

    QString rootDir() const;
    void setRootDir(const QString &newRootDir);

    QString fileName() const;
    void setFileName(const QString &newFileName);


private slots:
    void BrowseFolder();


private:
    QComboBox* _computeMode;
    QSpinBox* _depth;
    QSpinBox* _memorySize;

    QLineEdit* _dirView;
    QLineEdit* _fileName;
};

#endif // BUILDSETTINGS_H
