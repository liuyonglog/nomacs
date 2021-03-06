/*******************************************************************************************************
 DkDialog.h
 Created on:	21.04.2011
 
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2013 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2013 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2013 Florian Kleber <florian@nomacs.org>

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
#include <QItemDelegate>
#include <QPrintPreviewWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QDialog>
#include <QDir>
#include <QFutureWatcher>
#pragma warning(pop)		// no warnings from includes - end

#include "DkBasicLoader.h"

// Qt defines
class QStandardItemModel;
class QStandardItem;
class QTableView;
class QStringListModel;
class QListView;
class QTextEdit;
class QListWidget;
class QDialogButtonBox;
class QTreeView;
class QActionGroup;
class QPrintDialog;
class QAbstractButton;
class QLabel;
class QComboBox;
class QCheckBox;
class QProgressBar;

namespace nmc {

// nomacs defines
class DkBaseViewPort;
class TreeItem;
class DkSlider;
class DkButton;
class DkThumbNail;
class DkAppManager;

// needed because of http://stackoverflow.com/questions/1891744/pyqt4-qspinbox-selectall-not-working-as-expected 
// and http://qt-project.org/forums/viewthread/8590
class DkSelectAllLineEdit : public QLineEdit {

public:
	DkSelectAllLineEdit(QWidget* parent = 0) : QLineEdit(parent) {selectOnMousePressEvent = false;};

protected:
	void focusInEvent(QFocusEvent *event) {
		QLineEdit::focusInEvent(event);
		selectAll();
		selectOnMousePressEvent = true;
	}

	void mousePressEvent(QMouseEvent *event) {
		QLineEdit::mousePressEvent(event);
		if (selectOnMousePressEvent) {
			selectAll();
			selectOnMousePressEvent = false;
		}
	}
private:
	bool selectOnMousePressEvent; 
};

class DkSelectAllDoubleSpinBox : public QDoubleSpinBox {
	public:
		DkSelectAllDoubleSpinBox(QWidget* parent = 0) : QDoubleSpinBox(parent) {
			DkSelectAllLineEdit* le = new DkSelectAllLineEdit(this); 
			setLineEdit(le);
		};
};

class DkSplashScreen : public QDialog {
	Q_OBJECT

public:
	DkSplashScreen(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~DkSplashScreen() {};

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void showClose();

private:
	QPoint mouseGrab;
	QString text;
	QLabel* textLabel;
	QLabel* imgLabel;
	QTimer* showTimer;
	QPushButton* exitButton;
};

class DkFileValidator : public QValidator {
	Q_OBJECT

public:
	DkFileValidator(const QString& lastFile = QString(), QObject * parent = 0);

	void setLastFile(const QString& lastFile) {
		mLastFile = lastFile;
	};
	virtual void fixup(QString& input) const;
	virtual State validate(QString& input, int& pos) const;

protected:
	QString mLastFile;
};

class DkTrainDialog : public QDialog {
	Q_OBJECT
	
public:
	DkTrainDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	QString getAcceptedFile() {
		return mAcceptedFile;
	};

	void setCurrentFile(const QString& file) {
		mFile = file;
	};

public slots:
	void textChanged(const QString& text);
	void loadFile(const QString& filePath = "");
	void openFile();
	void accept();

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	void createLayout();
	void userFeedback(const QString& msg, bool error = false);

	DkFileValidator mFileValidator;
	QDialogButtonBox* mButtons;
	QLineEdit* mPathEdit;
	QLabel* mFeedbackLabel;
	DkBaseViewPort* mViewport;
	
	QString mAcceptedFile;
	QString mFile;
};

class DkAppManagerDialog : public QDialog {
	Q_OBJECT

public:
	DkAppManagerDialog(DkAppManager* manager = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);

public slots:
	void on_addButton_clicked();
	void on_deleteButton_clicked();
	void on_runButton_clicked();
	virtual void accept();

signals:
	void openWithSignal(QAction* act);

protected:
	DkAppManager* manager;
	QStandardItemModel* model;

	void createLayout();
	QList<QStandardItem* > getItems(QAction* action);
	QTableView* appTableView;
};

class DkSearchDialog : public QDialog {
	Q_OBJECT

public:

	enum Buttons {
		cancel_button = 0,
		find_button,
		filter_button,

		button_end,
	};

	DkSearchDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	void setFiles(const QStringList& fileList);
	void setPath(const QString& dirPath);
	bool filterPressed() const;
	void setDefaultButton(int defaultButton = find_button);

public slots:
	void on_searchBar_textChanged(const QString& text);
	void on_filterButton_pressed();
	void on_resultListView_doubleClicked(const QModelIndex& modelIndex);
	void on_resultListView_clicked(const QModelIndex& modelIndex);
	virtual void accept();

signals:
	void loadFileSignal(const QString& filePath) const;
	void filterSignal(const QStringList&) const;

protected:

	void updateHistory();
	void init();
	QStringList makeViewable(const QStringList& resultList, bool forceAll = false);

	QStringListModel* mStringModel = 0;
	QListView* mResultListView = 0;
	QLineEdit* mSearchBar = 0;
	QDialogButtonBox* mButtons = 0;

	QPushButton* mFilterButton = 0;

	QString mCurrentSearch;

	QString mPath;
	QStringList mFileList;
	QStringList mResultList;

	QString mEndMessage;

	bool mAllDisplayed = true;
	bool mIsFilterPressed = false;
};

class DkResizeDialog : public QDialog {
	Q_OBJECT

public:
	DkResizeDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~DkResizeDialog() {};

	enum{ipl_nearest, ipl_area, ipl_linear, ipl_cubic, ipl_lanczos, ipl_end};
	enum{size_pixel, size_percent, size_end};
	enum{unit_cm, unit_mm, unit_inch, unit_end};
	enum{res_ppi, res_ppc, res_end};  

	void setImage(const QImage& img);
	QImage getResizedImage();
	void setExifDpi(float exifDpi);
	float getExifDpi();
	bool resample();

protected slots:
	void on_lockButtonDim_clicked();
	void on_lockButton_clicked();

	void on_wPixelSpin_valueChanged(double val);
	void on_hPixelSpin_valueChanged(double val);

	void on_widthSpin_valueChanged(double val);
	void on_heightSpin_valueChanged(double val);
	void on_resolutionSpin_valueChanged(double val);

	void on_sizeBox_currentIndexChanged(int idx);
	void on_unitBox_currentIndexChanged(int idx);
	void on_resUnitBox_currentIndexChanged(int idx);
	void on_resampleBox_currentIndexChanged(int idx);

	void on_resampleCheck_clicked();
	void on_gammaCorrection_clicked();

	void drawPreview();

	void setVisible(bool visible) {
		updateSnippets();
		drawPreview();

		QDialog::setVisible(visible);
	}

public slots:
	virtual void accept();

protected:
	QImage mImg;
	QLabel* mPreviewLabel = 0;
	
	DkBaseViewPort* mOrigView = 0;

	// resize gui:
	QDoubleSpinBox* mWPixelSpin = 0;
	QDoubleSpinBox* mHPixelSpin = 0;
	DkButton* mLockButton = 0;

	QDoubleSpinBox* mWidthSpin = 0;
	QDoubleSpinBox* mHeightSpin = 0;
	QComboBox* mUnitBox = 0;
	QComboBox* mSizeBox = 0;
	DkButton* mLockButtonDim = 0;

	QDoubleSpinBox* mResolutionSpin = 0;
	QComboBox* mResUnitBox = 0;
	QCheckBox* mResampleCheck = 0;
	QCheckBox* mGammaCorrection = 0;
	QComboBox* mResampleBox = 0;

	float mExifDpi = 72;
	QVector<float> mUnitFactor;
	QVector<float> mResFactor;

	void init();
	void initBoxes(bool updateSettings = false);
	void createLayout();
	void updateSnippets();
	void updateHeight();
	void updateWidth();
	void updatePixelWidth();
	void updatePixelHeight();
	void updateResolution();
	void loadSettings();
	void saveSettings();
	QImage resizeImg(QImage img, bool silent = true);
};

class DkShortcutDelegate : public QItemDelegate {
	Q_OBJECT

public:
	DkShortcutDelegate(QObject* parent = 0);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

signals:
	void checkDuplicateSignal(const QString& text, void* item);
	void checkDuplicateSignal(const QKeySequence& keySequence, void* item);
	void clearDuplicateSignal() const;

protected slots:
	void textChanged(const QString& text = QString());
	void keySequenceChanged(const QKeySequence& keySequence);

protected:
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void* item;
	//virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;

};

class DkShortcutEditor : public QLineEdit {
	Q_OBJECT
	Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut)

public:
	DkShortcutEditor(QWidget *widget = 0);
	QKeySequence shortcut() const;
	void setShortcut(const QKeySequence shortcut);

protected:
	//void keyPressEvent(QKeyEvent *event);
	//void keyReleaseEvent(QKeyEvent* event);
	bool eventFilter(QObject *obj, QEvent *event);

	QKeySequence ks;

};

class DkShortcutsModel : public QAbstractItemModel {
	Q_OBJECT

public:
	DkShortcutsModel(QObject* parent = 0);
	~DkShortcutsModel();
	//DkShortcutsModel(QVector<QPair<QString, QKeySequence> > actions, QObject *parent = 0);

	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &index) const;

	// return item of the model
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	virtual Qt::ItemFlags flags(const QModelIndex& index) const;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	void addDataActions(QVector<QAction*> actions, const QString& name);

	void resetActions();
	void saveActions() const;

public slots:
	void checkDuplicate(const QString& text, void* item);		// deprecated (Qt4)
	void checkDuplicate(const QKeySequence& ks, void* item);
	void clearDuplicateInfo() const;

signals:
	void duplicateSignal(const QString& info) const;

protected:
	TreeItem* mRootItem;
	QVector<QVector<QAction*> > mActions;

};

class DkShortcutsDialog : public QDialog {
	Q_OBJECT

public:
	DkShortcutsDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	void addActions(const QVector<QAction*>& actions, const QString& name);

public slots:
	void accept() override;

protected slots:
	void contextMenu(const QPoint& cur);
	void defaultButtonClicked();

protected:
	void createLayout();
	
	DkShortcutsModel* mModel;
	QLabel* mNotificationLabel;
	QPushButton* mDefaultButton;

};

class DkTextDialog : public QDialog {
	Q_OBJECT

public:
	DkTextDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	void setText(const QStringList& text);
	QTextEdit* getTextEdit() { return textEdit; };

public slots:
	virtual void save();

protected:
	void createLayout();

	QTextEdit* textEdit;
};

class DkUpdateDialog : public QDialog {
	Q_OBJECT

public:
	DkUpdateDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	QLabel* upperLabel;

signals:
	void startUpdate();

protected slots:
	void okButtonClicked();

protected:
	void init();
	void createLayout();

		
	QPushButton* okButton;
	QPushButton* cancelButton;
};


class DkPrintPreviewWidget : public QPrintPreviewWidget {
	Q_OBJECT
public:
	DkPrintPreviewWidget(QPrinter* printer, QWidget* parent = 0, Qt::WindowFlags flags = 0);

signals:
	void zoomChanged() const;

protected:
	virtual void wheelEvent(QWheelEvent *event);		


};

class DkPrintPreviewValidator : public QDoubleValidator {
	
public:
	DkPrintPreviewValidator(const QString& suffix, qreal bottom, qreal top, int decimals, QObject* parent) : QDoubleValidator(bottom, top, decimals, parent) { this->suffix = suffix;};
	State validate(QString &input, int &pos) const {
		bool replaceSuffix = false;
		if (input.endsWith(suffix)) {
			input = input.left(input.length() - suffix.length());
			replaceSuffix = true;
		}
		State state = QDoubleValidator::validate(input, pos);
		if (replaceSuffix)
			input += suffix;
		const int num_size = 4+suffix.length();
		if (state == Intermediate) {
			int i = input.indexOf(QLocale::system().decimalPoint());
			if ((i == -1 && input.size() > num_size)
				|| (i != -1 && i > num_size))
				return Invalid;
		}

		return state;			
	}
private:
	QString suffix;
};



class DkPrintPreviewDialog : public QMainWindow {
	Q_OBJECT

public:
	enum icons{
		print_fit_width,
		print_fit_page,
		print_zoom_out,
		print_zoom_in,
		print_reset_dpi,
		print_portrait,
		print_landscape,
		print_setup,
		print_printer,
		print_end,
	};

	DkPrintPreviewDialog(const QImage& img, float dpi, QPrinter* printer = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);
	void setImage(const QImage& img, float dpi);
	void init();

public slots:
	void updateZoomFactor();

protected:
	void setupActions();
	void createLayout();
	void setIcon(QAction* action, const QLatin1String &name);
	void createIcons();

private slots:
	void paintRequested(QPrinter* printer);
	void fitImage(QAction* action);
	void zoomIn();
	void zoomOut();
	void zoomFactorChanged();
	void dpiFactorChanged();
	void updateDpiFactor(qreal dpi);
	void resetDpi();
	void pageSetup();
	void print();
	void centerImage();

private:
	void setFitting(bool on);
	void scaleImage();
	bool isFitting();
		
	QImage mImg;

	QActionGroup* mFitGroup = 0;
	QAction* mFitWidthAction = 0;
	QAction* mFitPageAction = 0;
	
	QActionGroup* mZoomGroup = 0;
	QAction* mZoomInAction = 0;
	QAction* mZoomOutAction = 0;

	QActionGroup* mOrientationGroup = 0;
	QAction* mPortraitAction = 0;
	QAction* mLandscapeAction = 0;

	QActionGroup* mPrinterGroup = 0;
	QAction* mPrintAction = 0;
	QAction* mPageSetupAction = 0;

	QActionGroup* mDpiGroup = 0;
	QAction* mResetDpiAction = 0;

	QComboBox* mZoomFactor = 0;
	QComboBox* mDpiFactor = 0;
	QString mDpiEditorSuffix;


	DkPrintPreviewWidget* mPreview = 0;
	QPrinter* mPrinter = 0;
	QPrintDialog* mPrintDialog = 0;

	QVector<QIcon> mIcons;

	QTransform mImgTransform;

	float mDpi = 150;
	float mOrigDpi = 150;
};

class DkOpacityDialog : public QDialog {
	Q_OBJECT

public:
	DkOpacityDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
	int value() const;

protected:
	void createLayout();

	DkSlider* slider;
};

class DkExportTiffDialog : public QDialog {
	Q_OBJECT

public:
	DkExportTiffDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

public slots:
	void on_openButton_pressed();
	void on_saveButton_pressed();
	void on_fileEdit_textChanged(const QString& filename);
	void setFile(const QString& filePath);
	void accept();
	void reject();
	int exportImages(const QString& saveFilePath, int from, int to, bool overwrite);
	void processingFinished();

signals:
	void updateImage(const QImage& img) const;
	void updateProgress(int) const;
	void infoMessage(const QString& msg) const;

protected:
	void createLayout();
	void enableTIFFSave(bool enable);
	void enableAll(bool enable);
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);

	DkBaseViewPort* mViewport;
	QLabel* mTiffLabel;
	QLabel* mFolderLabel;
	QLineEdit* mFileEdit;
	QComboBox* mSuffixBox;
	QSpinBox* mFromPage;
	QSpinBox* mToPage;
	QDialogButtonBox* mButtons;
	QProgressBar* mProgress;
	QLabel* mMsgLabel;
	QWidget* mControlWidget;
	QCheckBox* mOverwrite;

	QString mFilePath;
	QString mSaveDirPath;
	DkBasicLoader mLoader;
	QFutureWatcher<int> mWatcher;

	bool mProcessing = false;

	enum {
		finished,
		question_save,
		error,

	};
};

#ifdef WITH_OPENCV
class DkUnsharpDialog : public QDialog {
	Q_OBJECT

public:
	DkUnsharpDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
	QImage getImage();

public slots:
	void on_sigmaSlider_valueChanged(int i);
	void on_amountSlider_valueChanged(int i);
	void setFile(const QString& filePath);
	void setImage(const QImage& img);
	void computePreview();
	void reject();
	QImage computeUnsharp(const QImage& img, int sigma, int amount);
	void unsharpFinished();

signals:
	void updateImage(const QImage& img) const;

protected:
	void createLayout();
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);

	DkBaseViewPort* mViewport;
	QLabel* mPreview;
	QDialogButtonBox* mButtons;
	QFutureWatcher<QImage> mUnsharpWatcher;

	DkSlider* mSigmaSlider;
	DkSlider* mAmountSlider;

	bool mProcessing = false;
	QImage mImg;
};

class DkTinyPlanetDialog : public QDialog {
	Q_OBJECT

public:
	DkTinyPlanetDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
	QImage getImage();

public slots:
	void on_scaleLogSlider_valueChanged(int i);
	void on_angleSlider_valueChanged(int i);
	void on_invertBox_toggled(bool t);
	void setFile(const QString& filePath);
	void setImage(const QImage& img);
	void computePreview();
	void updateImageSlot(const QImage&);
	void reject();
	QImage computeTinyPlanet(const QImage& img, float scaleLog, double angle, QSize s);
	void tinyPlanetFinished();

signals:
	void updateImage(const QImage& img);

protected:
	void createLayout();
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void resizeEvent(QResizeEvent *event);

	QLabel* mImgPreview = 0;
	QLabel* mPreviewLabel = 0;
	QDialogButtonBox* mButtons = 0;
	QFutureWatcher<QImage> mTinyPlanetWatcher;

	DkSlider* mScaleLogSlider = 0;
	DkSlider* mAngleSlider = 0;
	QCheckBox* mInvertBox = 0;

	bool mProcessing = false;
	QImage mImg;
};

class DkMosaicDialog : public QDialog {
	Q_OBJECT

public:
	DkMosaicDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
	QImage getImage();

public slots:
	void on_openButton_pressed();
	void on_dbButton_pressed();
	void on_fileEdit_textChanged(const QString& filename);
	void on_newWidthBox_valueChanged(int i);
	void on_newHeightBox_valueChanged(int i);
	void on_numPatchesV_valueChanged(int i);
	void on_numPatchesH_valueChanged(int i);
	void on_darkenSlider_valueChanged(int i);
	void on_lightenSlider_valueChanged(int i);
	void on_saturationSlider_valueChanged(int i);
	
	void setFile(const QString& file);
	void compute();
	void reject();
	int computeMosaic(const QString& filter, const QString& suffix, int from, int to);		// TODO: make const!
	void mosaicFinished();
	void postProcessFinished();
	void buttonClicked(QAbstractButton* button);
	void updatePatchRes();

signals:
	void updateImage(const QImage& img) const;
	void updateProgress(int) const;
	void infoMessage(const QString& msg) const;

protected:
	void updatePostProcess();
	bool postProcessMosaic(float multiply = 0.3f, float screen = 0.5f, float saturation = 0.5f, bool computePreview = true);
	void createLayout();
	void enableMosaicSave(bool enable);
	void enableAll(bool enable);
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	QString getRandomImagePath(const QString& cPath, const QString& ignore, const QString& suffix);
	void matchPatch(const cv::Mat& img, const cv::Mat& thumb, int patchRes, cv::Mat& cc);
	cv::Mat createPatch(const DkThumbNail& thumb, int patchRes);
	
	DkBaseViewPort* mViewport = 0;
	DkBaseViewPort* mPreview = 0;
	QLabel* mFileLabel = 0;
	QLabel* mFolderLabel = 0;
	QLineEdit* mFilterEdit = 0;
	QComboBox* mSuffixBox = 0;
	QSpinBox* mNewWidthBox = 0;
	QSpinBox* mNewHeightBox = 0;
	QSpinBox* mNumPatchesV = 0;
	QSpinBox* mNumPatchesH = 0;
	QDialogButtonBox* mButtons = 0;
	QProgressBar* mProgress = 0;
	QLabel* mMsgLabel = 0;
	QWidget* mControlWidget = 0;
	QCheckBox* mOverwrite = 0;
	QLabel* mRealResLabel = 0;
	QLabel* mPatchResLabel = 0;
	
	QWidget* mSliderWidget = 0;
	QSlider* mDarkenSlider = 0;
	QSlider* mLightenSlider = 0;
	QSlider* mSaturationSlider = 0;

	QString mFilePath;
	QString mSavePath;
	DkBasicLoader mLoader;
	QFutureWatcher<int> mMosaicWatcher;
	QFutureWatcher<bool> mPostProcessWatcher;

	bool mUpdatePostProcessing = false;
	bool mPostProcessing = false;
	bool mProcessing = false;
	cv::Mat mOrigImg;
	cv::Mat mMosaicMat;
	cv::Mat mMosaicMatSmall;
	QImage mMosaic;
	QVector<QFileInfo> mFilesUsed;

	enum {
		finished,
		question_save,
		error,

	};
};
#endif

class DkForceThumbDialog : public QDialog {
	Q_OBJECT

public:
	DkForceThumbDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

	bool forceSave() const;
	void setDir(const QDir& fileInfo);

protected:
	void createLayout();

	QLabel* infoLabel;
	QCheckBox* cbForceSave;
};

class DkWelcomeDialog : public QDialog {
	Q_OBJECT

public:
	DkWelcomeDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

	bool isLanguageChanged();

public slots:
	virtual void accept();

protected:
	void createLayout();
	
	QComboBox* mLanguageCombo = 0;
	QCheckBox* mRegisterFilesCheckBox = 0;
	QCheckBox* mSetAsDefaultCheckBox = 0;
	QStringList mLanguages;
	bool mLanguageChanged = false;
};

#ifdef WITH_QUAZIP
class DkArchiveExtractionDialog : public QDialog {
	Q_OBJECT
	
public:
	DkArchiveExtractionDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	void setCurrentFile(const QString& filePath, bool isZip);

public slots:
	void textChanged(const QString& text);
	void checkbocChecked(int state);
	void dirTextChanged(const QString& text);
	void loadArchive(const QString& filePath = "");
	void openArchive();
	void openDir();
	void accept();

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	void createLayout();
	void userFeedback(const QString& msg, bool error = false);
	QStringList extractFilesWithProgress(const QString& fileCompressed, const QStringList& files, const QString& dir, bool removeSubfolders);

	DkFileValidator mFileValidator;
	QDialogButtonBox* mButtons = 0;
	QLineEdit* mArchivePathEdit = 0;
	QLineEdit* mDirPathEdit = 0;
	QListWidget* mFileListDisplay = 0;
	QLabel* mFeedbackLabel = 0;
	QCheckBox* mRemoveSubfolders = 0;

	QStringList mFileList;
	QString mFilePath;
};
#endif

class DkDialogManager : public QObject {
	Q_OBJECT

public:
	DkDialogManager(QObject* parent = 0);

	public slots:
	void openShortcutsDialog() const;
	void openAppManager() const;
};

}
