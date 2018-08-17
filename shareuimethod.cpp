// copyshareuimethod.cpp

/*
 * copy-share-ui-plugin
 * Copyright (C) 2013 Roopesh Chander <roop@forwardbias.in>
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "shareuimethod.h"
#include <ShareUI/ItemContainer>
#include <ShareUI/FileItem>
#include <ShareUI/DataUriItem>
#include <QApplication>
#include <QProcess>

ShellShareUIMethod::ShellShareUIMethod(QObject *parent)
    : ShareUI::MethodBase(parent) {
}

ShellShareUIMethod::~ShellShareUIMethod()
{
}

QString ShellShareUIMethod::id()
{
    return "ch.stanson.shellshare";
}

QString ShellShareUIMethod::title()
{
    return "User script";
}

QString ShellShareUIMethod::icon()
{
    return "icon-m-shellshare";
}

void ShellShareUIMethod::currentItems(const ShareUI::ItemContainer * items)
{
    Q_UNUSED (items);

    emit visible( true ); // always valid
}

void ShellShareUIMethod::selected( const ShareUI::ItemContainer * items )
{
    QString path = "";
    QString script = "/bin/sh /home/user/.local/share/scripts/share.sh";

    ShareUI::ItemIterator itemsIter = items->itemIterator();

    while ( itemsIter.hasNext() )
    {
        ShareUI::SharedItem  item = itemsIter.next();
        ShareUI::FileItem    *fileItem = ShareUI::FileItem::toFileItem( item );
        ShareUI::DataUriItem *dataUriItem = ShareUI::DataUriItem::toDataUriItem( item );
        if ( fileItem )
        { // it's a file
            path = fileItem->filePath();
        }
        else if ( dataUriItem )
        { // it's a URL
            path = dataUriItem->dataUri().textData();
        }
        else continue; // Something weird.

        // run command with path as argument
        QString cmd = script + " \"" + path + "\"";

        if ( !QProcess::startDetached( cmd ) )
        { // Failed to launch the command
            QString err = "Failed to launch: " + cmd;
            qCritical( err.toLatin1() );
            emit( selectedFailed( err ) );
            return;
        }
    }
    emit( done() );
}
