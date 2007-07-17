/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _FONT_SETTINGS_DIALOG_H_
#define _FONT_SETTINGS_DIALOG_H_

#include <QtCore/QObject>
#include <QtGui/QDialog>

class FontPanel;
struct FontSettings;
class QDialogButtonBox;

class FontSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    FontSettingsDialog(QWidget *parent = 0);
    ~FontSettingsDialog();

    bool showDialog(FontSettings *settings);

private:
    void updateFontSettings(FontSettings *settings);
    void setupFontSettingsDialog(const FontSettings *settings);
    
private:
    FontPanel *m_windowFontPanel;
    FontPanel *m_browserFontPanel;
    QDialogButtonBox *m_dialogButtonBox;
};

#endif  // _FONT_SETTINGS_DIALOG_H_

