/////////////////////////////////////////////////////////////////////////
//                                                                     //
// File adopted from KDE4 libraries by                                 //
// T. Kittelmann <Thomas.Kittelmann@cern.ch>, March 2007.              //
//                                                                     //
// Main thing is to remove dependence on KDE for length of title text  //
// settings, delay on drag settings and title eliding. Also,           //
// hoverbuttons were removed (since these had not been properly        //
// implemented in KDE4 at the time the code was copied).               //
//                                                                     //
// Notice about Copyrights and GPL license from the orignal file is    //
// left untouched below.                                               //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

/* This file is part of the KDE libraries
    Copyright (C) 2003 Stephan Binner <binner@kde.org>
    Copyright (C) 2003 Zack Rusin <zack@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef QatTABBAR_H
#define QatTABBAR_H

#include <QTabBar>

class QatTabBar: public QTabBar
{
    Q_OBJECT

  public:
    explicit QatTabBar( QWidget* parent = 0 );

    virtual ~QatTabBar();

    void setTabReorderingEnabled( bool enable );

    bool isTabReorderingEnabled() const;

    void setTabCloseActivatePrevious( bool );

    bool tabCloseActivatePrevious() const;

    int selectTab( const QPoint &position ) const;

  Q_SIGNALS:
    void contextMenu( int, const QPoint& );
    void mouseDoubleClick( int );
    void mouseMiddleClick( int );
    void initiateDrag( int );
    void testCanDecode( const QDragMoveEvent*, bool& );
    void receivedDropEvent( int, QDropEvent* );
    void moveTab( int, int );
    void closeRequest( int );
#ifndef QT_NO_WHEELEVENT
    void wheelDelta( int );
#endif

  protected:
    virtual void mouseDoubleClickEvent( QMouseEvent *event );
    virtual void mousePressEvent( QMouseEvent *event );
    virtual void mouseMoveEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent( QWheelEvent *event );
#endif

    virtual void dragEnterEvent( QDragEnterEvent *event );
    virtual void dragMoveEvent( QDragMoveEvent *event );
    virtual void dropEvent( QDropEvent *event );

  protected Q_SLOTS:
    virtual void activateDragSwitchTab();

  protected:
    virtual void tabLayoutChange();

  private:
    class Private;
    Private* const d;
};

#endif
