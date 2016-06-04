/*******************************************************************************************************
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2016 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2016 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2016 Florian Kleber <florian@nomacs.org>

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

 related links:
 [1] http://www.nomacs.org/
 [2] https://github.com/nomacs/
 [3] http://download.nomacs.org
 *******************************************************************************************************/

#include "DkBatchInfo.h"

#pragma warning(push, 0)	// no warnings from includes
#include <QFileInfo>
#include <QSharedPointer>
#include <QVector>
#include <QDebug>
#include <QDataStream>
#pragma warning(pop)

namespace nmc {

/// <summary>
/// Initializes a new instance of the <see cref="DkBatchInfo" /> class.
/// This class is intendet to be used together with the DkBatchProcess.
/// You can derive this class and store all (e.g. plugin) information
/// needed in the postLoad(). Please keep the memory usage per instance low since
/// there might be *a lot* of images to be processed.
/// </summary>
/// <param name="id">The processing module ID (e.g. runID of plugin).</param>
/// <param name="filePath">The image file path.</param>
/// DkBatchInfo --------------------------------------------------------------------
DkBatchInfo::DkBatchInfo(const QString& id, const QString& filePath) {
	mId = id;
	mFilePath = filePath;
}

QDataStream& operator<<(QDataStream& s, const DkBatchInfo& b) {

	// this makes the operator<< virtual (stroustrup)
	s << b.toString();	// for now show children too
	return s;
}

QDebug operator<<(QDebug d, const DkBatchInfo& b) {

	d << qPrintable(b.toString());
	return d;
}


/// <summary>
/// Determines whether this instance is empty.
/// </summary>
/// <returns>true if the file path is empty</returns>
bool DkBatchInfo::isEmpty() const {
	return mFilePath.isEmpty();
}

/// <summary>
/// File path of the corresponding image.
/// </summary>
/// <returns></returns>
QString DkBatchInfo::filePath() const {
	return mFilePath;
}

/// <summary>
/// QFileInfo with the current file path.
/// </summary>
/// <returns></returns>
QFileInfo DkBatchInfo::fileInfo() const {
	return QFileInfo(mFilePath);
}

/// <summary>
/// Sets the file path.
/// </summary>
/// <param name="filePath">The path to the image file which is being processed.</param>
void DkBatchInfo::setFilePath(const QString & filePath) {
	mFilePath = filePath;
}

/// <summary>
/// A unique identifier of the processing module (e.g. runID of plugins).
/// </summary>
/// <param name="id">The identifier.</param>
void DkBatchInfo::setId(const QString & id) {
	mId = id;
}

/// <summary>
/// ID of the processing module.
/// </summary>
/// <returns></returns>
QString DkBatchInfo::id() const {
	return mId;
}

QString DkBatchInfo::toString() const {
	
	QString msg;
	msg += "[DkBatchInfo] ";
	msg += "id: " + id() + " ";
	msg += "path: " + filePath() + " ";
	
	return msg;
}

/// <summary>
/// Filters the specified infos with respect to a given ID.
/// This function returns all DkBatchInfos that were generated by
/// a specific module (or plugin).
/// </summary>
/// <param name="infos">The infos.</param>
/// <param name="id">The ID.</param>
/// <returns>Filtered Batch Infos for a specified module which has runID id.</returns>
QVector<QSharedPointer<DkBatchInfo> > DkBatchInfo::filter(const QVector<QSharedPointer<DkBatchInfo>>& infos, const QString & id) {
	
	QVector<QSharedPointer<DkBatchInfo> > fInfos;

	for (auto cInfo : infos) {

		if (cInfo && cInfo->id() == id)
			fInfos << cInfo;
	}
	
	return fInfos;
}

}