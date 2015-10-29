/*******************************************************************************************************
 DkNoMacs.h
 Created on:	21.04.2011
 
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2013 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2013 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2013 Florian Kleber <florian@nomacs.org>

 This file is part of nomacs.

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

// Qt
#pragma warning(push, 0)	// no warnings from includes - begin
#include <QMainWindow>
#include <QProcess>
#pragma warning(pop)		// no warnings from includes - end

#include "DkImageContainer.h"

#ifdef WIN32
	#pragma warning(disable: 4996)
	#pragma warning(disable: 4251)	// TODO: remove
#endif

#ifndef DllExport
#ifdef DK_DLL_EXPORT
#define DllExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllExport Q_DECL_IMPORT
#else
#define DllExport
#endif
#endif

// Qt defines
class QGestureEvent;
class QFileDialog;
class QProgressDialog;
class QDesktopWidget;
class QLabel;
class QShortcut;

namespace nmc {

class DkTcpMenu;
class DkCompressDialog;
class DkSettingsDialog;
class DkOpacityDialog;
class DkResizeDialog;
class DkUpdateDialog;
class DkForceThumbDialog;
class DkTrainDialog;
#ifdef WITH_QUAZIP
class DkArchiveExtractionDialog;
#endif
class DkExplorer;
class DkMetaDataDock;
class DkExportTiffDialog;
class DkImageManipulationDialog;
class DkUpdater;
class DkTranslationUpdater;
class DkLocalManagerThread;
class DkLanManagerThread;
class DkRCManagerThread;
class DkTransferToolBar;
class DkPluginManagerDialog;
class DkAppManager;
class DkThumbsSaver;
class DkPrintPreviewDialog;
class DkBatchDialog;
class DkViewPort;
class DkCentralWidget;
class DkMainToolBar;
class DkDockWidget;
class DkQuickAccess;
class DkImageLoader;

#ifdef WITH_UPNP
class DkUpnpControlPoint;
class DkUpnpDeviceHost;
class DkUpnpRendererDeviceHost;
#endif // WITH_UPNP

enum statusbarLabels {
	status_pixel_info,
	status_filesize_info,
	status_time_info,

	status_end,

};

/*! A custom event filter for OSX.
Its main purpose is to provide Mac OS X "open file from finder"
functionality for nomacs. It *has* to be catched on the
QApplication level in the event().
*/
class DkNomacsOSXEventFilter : public QObject {
	Q_OBJECT

public:
	DkNomacsOSXEventFilter(QObject *parent = 0);

signals:
	void loadFile(const QString& fi) const;

protected:
	/*! Handle QFileOpenEvent for mac here */
	bool eventFilter(QObject *obj, QEvent *event);

};

class DkMenuBar;

enum PluginMenuActions {
	menu_plugin_manager,

	menu_plugins_end,	// nothing beyond this point
};


class DllExport DkNoMacs : public QMainWindow {
	Q_OBJECT

public:

	virtual ~DkNoMacs();

	void release();
	
	//static int infoDialog(const QString& msg, QWidget* parent = 0, QString title = "Question");
	//static int dialog(const QString& msg, QWidget* parent = 0, QString title = "Error");
	//static QWidget* getDialogParent();

	virtual DkViewPort* viewport() const;
	virtual DkCentralWidget* getTabWidget() const;
	
	void loadFile(const QString& filePath);

	static void updateAll();

	bool mSaveSettings = true;

signals:
	void sendTitleSignal(const QString& newTitle) const;
	void sendPositionSignal(QRect newRect, bool overlaid) const;
	void sendArrangeSignal(bool overlaid) const;
	void synchronizeWithSignal(quint16) const;
	void stopSynchronizeWithSignal() const;
	void synchronizeWithServerPortSignal(quint16) const;
	void synchronizeRemoteControl(quint16) const;
	void closeSignal() const;
	void sendQuitLocalClientsSignal() const;

public slots:
	void restart();
	void toggleFullScreen();
	void enterFullScreen();
	void exitFullScreen();
	void openKeyboardShortcuts();
	void openSettings();
	void showExplorer(bool show, bool saveSettings = true);
	void showMetaDataDock(bool show, bool saveSettings = true);
	void showThumbsDock(bool show);
	void thumbsDockAreaChanged();
	void showRecentFiles(bool show = true);
	void openDir();
	void openFile();
	void openQuickLaunch();
	void renameFile();
	void changeSorting(bool change);
	void goTo();
	void find(bool filterAction = true);
	void saveFile();
	void saveFileAs(bool silent = false);
	void saveFileWeb();
	void extractImagesFromArchive();
	void trainFormat();
	void resizeImage();
	void openImgManipulationDialog();
	void exportTiff();
	void computeMosaic();
	void deleteFile();
	void openAppManager();
	void setWallpaper();
	void printDialog();
	void cleanSettings();
	void newInstance(const QString& filePath = QString());
	void showStatusBar(bool show, bool permanent = true);
	void showMenuBar(bool show);
	void showToolbarsTemporarily(bool show);
	void showToolbar(bool show);
	void showToolbar(QToolBar* toolbar, bool show);
	void showGpsCoordinates();
	void openFileWith(QAction* action);
	void aboutDialog();
	void openDocumentation();
	void bugReport();
	void featureRequest();
	//void errorDialog(QString msg, QString title = "Error");
	//void errorDialog(const QString& msg);
	void loadRecursion();
	void setWindowTitle(QSharedPointer<DkImageContainerT> imgC);
	void setWindowTitle(const QString& filePath, const QSize& size = QSize(), bool edited = false, const QString& attr = QString());
	void showOpacityDialog();
	void opacityUp();
	void opacityDown();
	void changeOpacity(float change);
	void animateChangeOpacity();
	void animateOpacityUp();
	void animateOpacityDown();
	void lockWindow(bool lock);
	void tcpSetWindowRect(QRect newRect, bool opacity, bool overlaid);
	void tcpSendWindowRect();
	void tcpSendArrange();
	virtual void newClientConnected(bool connected, bool local);
	void showStatusMessage(QString msg, int which = status_pixel_info);
	void flipImageHorizontal();
	void flipImageVertical();
	void normalizeImage();
	void autoAdjustImage();
	void unsharpMask();
	void tinyPlanet();
	void invertImage();
	void convert2gray();
	virtual void settingsChanged();
	void showUpdaterMessage(QString msg, QString title);
	void showUpdateDialog(QString msg, QString title);
	void performUpdate();
	void updateProgress(qint64 received, qint64 total);
	void updateProgressTranslations(qint64 received, qint64 total);
	void startSetup(QString);
	void updateTranslations();
	virtual void enableNoImageActions(bool enable = true);
	void checkForUpdate(bool silent = false);
	void setFrameless(bool frameless);
	void startPong() const;
	void fitFrame();
	void setRecursiveScan(bool recursive);
	void setContrast(bool contrast);
	void enableMovieActions(bool enable);
	void runLoadedPlugin();
	void openPluginManager();
	void runPluginFromShortcut();
	void clearFileHistory();
	void clearFolderHistory();
	//void shareFacebook();
	void restartWithTranslationUpdate();

	// batch actions
	void computeThumbsBatch();
	void onWindowLoaded();

	void createPluginsMenu();

protected:
	
	// mouse events
	void mouseDoubleClickEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	// mouse events
	void moveEvent(QMoveEvent *event);

	// window events
	void contextMenuEvent(QContextMenuEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	void resizeEvent(QResizeEvent *event);
	void closeEvent(QCloseEvent *event);

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent* event);

	bool gestureEvent(QGestureEvent *event);

	void assignCustomShortcuts(QVector<QAction*> actions);
	void assignCustomPluginShortcuts();
	void savePluginActions(QVector<QAction *> actions);

	// needed to hide menu
	bool mOtherKeyPressed = true;
	QPoint mPosGrabKey;
	bool mOverlaid = false;

	// vars
	QVector<QAction *> mPluginsActions;
	QVector<QAction *> mPluginsDummyActions;
	
	// menu
	DkMenuBar* mMenu = 0;
	QMenu* mPluginsMenu = 0;
	QMenu* mSyncMenu = 0;
	QVector<QMenu*> mPluginSubMenus;

	QPoint mMousePos;
	
	// toolbar
	DkMainToolBar* mToolbar = 0;
	QToolBar* mMovieToolbar = 0;
	QStatusBar* mStatusbar = 0;
	QVector<QLabel*> mStatusbarLabels;
	DkQuickAccess* mQuickAccess = 0;

	// file dialog
	QFileDialog* mOpenDialog = 0;
	QFileDialog* mSaveDialog = 0;
	DkOpacityDialog* mOpacityDialog = 0;
	DkResizeDialog* mResizeDialog = 0;
	DkUpdateDialog* mUpdateDialog = 0;
	QProgressDialog* mProgressDialog = 0;
	QProgressDialog* mProgressDialogTranslations = 0;
	DkForceThumbDialog* mForceDialog = 0;
	DkTrainDialog* mTrainDialog = 0;
#ifdef WITH_QUAZIP
	DkArchiveExtractionDialog* mArchiveExtractionDialog = 0;
#endif
	DkExplorer* mExplorer = 0;
	DkMetaDataDock* mMetaDataDock = 0;
	DkDockWidget* mThumbsDock = 0;
	DkExportTiffDialog* mExportTiffDialog = 0;
	DkSettingsDialog* mSettingsDialog = 0;
	DkThumbsSaver* mThumbSaver = 0;
	DkImageManipulationDialog* mImgManipulationDialog = 0;

	DkPrintPreviewDialog* mPrintPreviewDialog = 0;

	DkUpdater* mUpdater = 0;
	DkTranslationUpdater* mTranslationUpdater = 0;	

	QRect mOldGeometry;
	QList<QToolBar *> mHiddenToolbars;

	QProcess mProcess;

	// functions
	DkNoMacs(QWidget *parent = 0, Qt::WindowFlags flags = 0);

	virtual void init();
	
	void loadStyleSheet();
	void registerFileVersion();
	virtual void createToolbar();
	virtual void createActions();
	virtual void createMenu();
	virtual void createContextMenu();
	virtual void createStatusbar();

	virtual void readSettings();

	// plugin functions
	void addPluginsToMenu();
	bool mPluginMenuCreated = false;
};

class DllExport DkNoMacsSync : public DkNoMacs {
	Q_OBJECT

public:
	DkNoMacsSync(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~DkNoMacsSync();
	qint16 getServerPort();
	void syncWith(qint16 port);
	
signals:
	void clientInitializedSignal();
	void startRCServerSignal(bool start);
	void startTCPServerSignal(bool start);

public slots:
	void tcpConnectAll();
	void tcpChangeSyncMode(int syncMode, bool connectWithWhiteList = false);
	void tcpRemoteControl(bool start);
	void tcpRemoteDisplay(bool start);
	void tcpAutoConnect(bool connect);
	void startUpnpRenderer(bool start);
	void settingsChanged();
	void clientInitialized();
	void newClientConnected(bool connected, bool local);
	void startTCPServer(bool start);
	virtual void enableNoImageActions(bool enable = true);

protected:

	// mouse events
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	// functions
	void initLanClient();
	bool connectWhiteList(int mode, bool connect = true);

	// gui
	virtual void createActions();
	virtual void createMenu();

	// network layer
	DkLocalManagerThread* mLocalClient = 0;
	DkLanManagerThread* mLanClient = 0;
	DkRCManagerThread* mRcClient = 0;
#ifdef WITH_UPNP
	QSharedPointer<DkUpnpControlPoint> upnpControlPoint;
	QSharedPointer<DkUpnpDeviceHost> upnpDeviceHost;
	QSharedPointer<DkUpnpRendererDeviceHost> upnpRendererDeviceHost;
#endif // WITH_UPNP

};

class DllExport DkNoMacsIpl : public DkNoMacsSync {
	Q_OBJECT

public:
	DkNoMacsIpl(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};



class DllExport DkNoMacsFrameless : public DkNoMacs {
	Q_OBJECT

public:
	DkNoMacsFrameless(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	
	virtual ~DkNoMacsFrameless();

	void release();
	
public slots:
	virtual void enableNoImageActions(bool enable = true);
	void exitFullScreen();
	void updateScreenSize(int screen = 0);

protected:
	void closeEvent(QCloseEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	virtual void createContextMenu();

	QDesktopWidget* mDesktop = 0;
};

class DllExport DkNoMacsContrast : public DkNoMacsSync {
	Q_OBJECT

public:
	DkNoMacsContrast(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	
	virtual ~DkNoMacsContrast();
	void release();	

protected:
	void createTransferToolbar();

	DkTransferToolBar* mTransferToolBar = 0;

};
};
