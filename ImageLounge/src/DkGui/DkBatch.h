/*******************************************************************************************************
DkNoMacs.h
Created on:	26.10.2014
 
nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
Copyright (C) 2011-2014 Markus Diem <markus@nomacs.org>
Copyright (C) 2011-2014 Stefan Fiel <stefan@nomacs.org>
Copyright (C) 2011-2014 Florian Kleber <florian@nomacs.org>

This file is part of nomacs.

nomacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

nomacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************************************/
#pragma once

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QWidget>
#include <QUrl>
#include <QDialog>
#include <QTextEdit>
#pragma warning(pop)		// no warnings from includes - end

#include "DkImageContainer.h"
#include "DkImageLoader.h"

// Qt defines
class QListView;
class QVBoxLayout;
class QLabel;
class QFileInfo;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QGridLayout;
class QCheckBox;
class QButtonGroup;
class QRadioButton;
class QDialogButtonBox;
class QProgressBar;
class QTabWidget;
class QListWidget;

namespace nmc {

// TODO: these enums are global - they should be put into the respective classes
enum fileNameTypes {
	fileNameTypes_fileName,
	fileNameTypes_Number,
	fileNameTypes_Text,

	fileNameTypes_end
};

enum fileNameWidget {
	fileNameWidget_type,
	fileNameWidget_input1,
	fileNameWidget_input2,
	fileNameWidget_plus,
	fileNameWidget_minus,

	fileNameWidget_end
};

// nomacs defines
class DkResizeBatch;
class DkPluginBatch;
class DkBatchProcessing;
class DkBatchTransform;
class DkBatchContent;
class DkButton;
class DkThumbScrollWidget;
class DkImageLoader;
class DkExplorer;
class DkDirectoryEdit;
class DkListWidget;

class DkBatchContent {

public:
	virtual bool hasUserInput() const = 0;
	virtual bool requiresUserInput() const = 0;
};

class DkBatchWidget : public QWidget {
	Q_OBJECT

public:
	DkBatchWidget(const QString& titleString, const QString& headerString, QWidget* parent = 0, Qt::WindowFlags f = 0);
	
	void setContentWidget(QWidget* batchContent);
	QWidget* contentWidget() const;

public slots:
	void setTitle(const QString& title);
	void setHeader(const QString& header);
	void showContent(bool show);

protected:
	virtual void createLayout();

private:
	DkBatchContent* mBatchContent = 0;
	QVBoxLayout* mBatchWidgetLayout = 0;

	QLabel* mTitleLabel = 0; 
	QLabel* mHeaderLabel = 0;
	DkButton* mShowButton = 0;

	QString mTitleString;
	QString mHeaderString;
};

class DkInputTextEdit : public QTextEdit {
	Q_OBJECT

public:
	DkInputTextEdit(QWidget* parent = 0);

	QStringList getFileList() const;
	void appendDir(const QString& newDir, bool recursive = false);
	void insertFromMimeData(const QMimeData *src);
	void clear();

signals:
	void fileListChangedSignal() const;

public slots:
	void appendFiles(const QStringList& fileList);

protected:
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void appendFromMime(const QMimeData* mimeData, bool recursive = false);

	QList<int> mResultList;
};

class DkFileSelection : public QWidget, public DkBatchContent  {
	Q_OBJECT

public:

	enum {
		tab_thumbs = 0,
		tab_text_input,
		tab_results,

		tab_end
	};

	DkFileSelection(QWidget* parent = 0, Qt::WindowFlags f = 0);

	QString getDir() const;
	QStringList getSelectedFiles() const;
	QStringList getSelectedFilesBatch();
	DkInputTextEdit* getInputEdit() const;

	virtual bool hasUserInput() const {return mHUserInput;};
	virtual bool requiresUserInput() const {return mRUserInput;};
	void changeTab(int tabIdx) const;
	void startProcessing();
	void stopProcessing();
	void setResults(const QStringList& results);

public slots:
	void setDir(const QString& dirPath);
	void browse();
	void updateDir(QVector<QSharedPointer<DkImageContainerT> >);
	void setVisible(bool visible);
	void emitChangedSignal();
	void selectionChanged();
	void setFileInfo(QFileInfo file);

signals:
	void updateDirSignal(QVector<QSharedPointer<DkImageContainerT> >) const;
	void newHeaderText(const QString&) const;
	void updateInputDir(const QString&) const;
	void changed() const;

protected:
	virtual void createLayout();

	QString mCDirPath;
	QListView* mFileWidget = 0;
	DkThumbScrollWidget* mThumbScrollWidget = 0;
	DkInputTextEdit* mInputTextEdit = 0;
	QTextEdit* mResultTextEdit = 0;
	DkExplorer* mExplorer = 0;
	DkDirectoryEdit* mDirectoryEdit = 0;
	QLabel* mInfoLabel = 0;
	QTabWidget* mInputTabs = 0;
	QSharedPointer<DkImageLoader> mLoader = QSharedPointer<DkImageLoader>(new DkImageLoader());

private:
	bool mHUserInput = false;
	bool mRUserInput = false;

};

class DkFilenameWidget : public QWidget {
Q_OBJECT

public:	
	DkFilenameWidget(QWidget* parent = 0);
	void enableMinusButton(bool enable);
	void enablePlusButton(bool enable);
	bool hasUserInput() const {return hasChanged;};
	QString getTag() const;

signals:
	void plusPressed(DkFilenameWidget*) const;
	void minusPressed(DkFilenameWidget*) const;
	void changed() const;

private slots:
	void typeCBChanged(int index);
	void pbPlusPressed();
	void pbMinusPressed();
	void checkForUserInput();
	void digitCBChanged(int index);

private:
	void createLayout();
	void showOnlyText();
	void showOnlyNumber();
	void showOnlyFilename();

	QComboBox* mCbType = 0;
		
	QLineEdit* mLeText = 0;
	QComboBox* mCbCase = 0;

	QSpinBox* mSbNumber = 0;
	QComboBox* mCbDigits = 0;
		
	QPushButton* mPbPlus = 0;
	QPushButton* mPbMinus = 0;

	QGridLayout* mLayout = 0;

	bool hasChanged = false;
};

class DkBatchOutput : public QWidget, public DkBatchContent {
Q_OBJECT

public:
	DkBatchOutput(QWidget* parent = 0, Qt::WindowFlags f = 0);

	virtual bool hasUserInput() const;
	virtual bool requiresUserInput() const {return mRUserInput;};
	int overwriteMode() const;
	bool useInputDir() const;
	bool deleteOriginal() const;
	QString getOutputDirectory();
	QString getFilePattern();
	void setExampleFilename(const QString& exampleName);
	int getCompression() const;

signals:
	void newHeaderText(const QString&) const;
	void changed() const;

public slots:
	void setInputDir(const QString& dirPath);

protected slots:
	void browse();
	void plusPressed(DkFilenameWidget* widget);
	void minusPressed(DkFilenameWidget* widget);
	void extensionCBChanged(int index);
	void emitChangedSignal();
	void updateFileLabelPreview();
	void useInputFolderChanged(bool checked);
	void setDir(const QString& dirPath, bool updateLineEdit = true);

protected:
	virtual void createLayout();

private:

	bool mHUserInput = false;
	bool mRUserInput = false;
	QString mOutputDirectory;
	QString mInputDirectory;
	QVector<DkFilenameWidget*> mFilenameWidgets;
	DkDirectoryEdit* mOutputlineEdit = 0;
	QVBoxLayout* mFilenameVBLayout = 0;
	QCheckBox* mCbOverwriteExisting = 0;
	QCheckBox* mCbUseInput = 0;
	QCheckBox* mCbDeleteOriginal = 0;
	QPushButton* mOutputBrowseButton = 0;

	QComboBox* mCbExtension = 0;
	QComboBox* mCbNewExtension = 0;
	QSpinBox* mSbCompression = 0;
	QLabel* mOldFileNameLabel = 0;
	QLabel* mNewFileNameLabel = 0;
	QString mExampleName = "";

};

class DkBatchResizeWidget : public QWidget, public DkBatchContent {
	Q_OBJECT

public:
	DkBatchResizeWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

	void transferProperties(QSharedPointer<DkResizeBatch> batchResize) const;
	bool hasUserInput() const;
	bool requiresUserInput() const;

public slots:
	void modeChanged(int idx);
	void percentChanged(double val);
	void pxChanged(int val);

signals:
	void newHeaderText(const QString& txt) const;

protected:
	void createLayout();

	QComboBox* mComboMode;
	QComboBox* mComboProperties;
	QSpinBox* mSbPx;
	QDoubleSpinBox* mSbPercent;
};

#ifdef WITH_PLUGINS
class DkBatchPluginWidget : public QWidget, public DkBatchContent {
	Q_OBJECT

public:
	DkBatchPluginWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

	void transferProperties(QSharedPointer<DkPluginBatch> batchPlugin) const;
	bool hasUserInput() const;
	bool requiresUserInput() const;

signals:
	void newHeaderText(const QString& txt) const;

public slots:
	void updateHeader() const;

protected:
	void createLayout();
	QStringList getPluginActionNames() const;

	DkListWidget* mPluginListWidget;

};
#endif

class DkBatchTransformWidget : public QWidget, public DkBatchContent {
	Q_OBJECT

public:
	DkBatchTransformWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

	void transferProperties(QSharedPointer<DkBatchTransform> batchTransform) const;
	bool hasUserInput() const;
	bool requiresUserInput() const;

public slots:
	void radioButtonClicked(int id);
	void checkBoxClicked();

signals:
	void newHeaderText(const QString& txt) const;

protected:
	void createLayout();
	void updateHeader() const;
	int getAngle() const;

	QButtonGroup* mRotateGroup = 0;
	QRadioButton* mRbRotate0 = 0;
	QRadioButton* mRbRotateLeft = 0;
	QRadioButton* mRbRotateRight = 0;
	QRadioButton* mRbRotate180 = 0;

	QCheckBox* mCbFlipH = 0;
	QCheckBox* mCbFlipV = 0;
};

class DkBatchDialog : public QDialog {
	Q_OBJECT

public:
	DkBatchDialog(const QString& currentDirectory = QString(), QWidget* parent = 0, Qt::WindowFlags f = 0);

	enum batchWidgets {
		batch_input,
		batch_resize,
		batch_transform,
		batch_plugin,
		batch_output,

		batchWidgets_end
	};

public slots:
	virtual void accept() override;
	virtual void reject();
	void widgetChanged();
	void logButtonClicked();
	void processingFinished();
	void updateProgress(int progress);
	void updateLog();
	void setSelectedFiles(const QStringList& selFiles);

protected:
	void createLayout();
		
private:
	QVector<DkBatchWidget*> mWidgets;
		
	QString mCurrentDirectory;
	QDialogButtonBox* mButtons;
	DkFileSelection* mFileSelection;
	DkBatchOutput* mOutputSelection;
	DkBatchResizeWidget* mResizeWidget;

#ifdef WITH_PLUGINS
	DkBatchPluginWidget* mPluginWidget;
#endif

	DkBatchTransformWidget* mTransformWidget;
	DkBatchProcessing* mBatchProcessing;
	QPushButton* mLogButton;
	QProgressBar* mProgressBar;
	QLabel* mSummaryLabel;
	QTimer mLogUpdateTimer;
	bool mLogNeedsUpdate = false;

	void startProcessing();
	void stopProcessing();
};

}
