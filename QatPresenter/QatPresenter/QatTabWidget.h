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

#ifndef QatTABWIDGET_H
#define QatTABWIDGET_H

#include <QTabWidget>

class QTab;
class QatTabBar;

class QatTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_PROPERTY( bool tabReorderingEnabled READ isTabReorderingEnabled WRITE setTabReorderingEnabled )
    Q_PROPERTY( bool tabCloseActivatePrevious READ tabCloseActivatePrevious WRITE setTabCloseActivatePrevious )
    Q_PROPERTY( bool automaticResizeTabs READ automaticResizeTabs WRITE setAutomaticResizeTabs )

  public:

    QatTabBar* getQatTabBar();//Added by TK.

    explicit QatTabWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

    virtual ~QatTabWidget();

    void setTabTextColor( int index, const QColor& color );

    QColor tabTextColor( int index ) const;

    bool isTabReorderingEnabled() const;

    bool tabCloseActivatePrevious() const;

    bool automaticResizeTabs() const;

    void setTabBarHidden( bool hide );

    bool isTabBarHidden() const;

    QString tabText( int ) const; // but it's not virtual...

    void setTabText( int , const QString & );

  public Q_SLOTS:
    virtual void moveTab( int, int );

    virtual QT_MOC_COMPAT void removePage ( QWidget * w );

    virtual void removeTab(int index);

    void setTabReorderingEnabled( bool enable );

    void setTabCloseActivatePrevious( bool previous );

    void setAutomaticResizeTabs( bool enable );

  Q_SIGNALS:
    void testCanDecode(const QDragMoveEvent *e, bool &accept /* result */);

    void receivedDropEvent( QDropEvent * );

    void receivedDropEvent( QWidget *, QDropEvent * );

    void initiateDrag( QWidget * );

    void contextMenu( const QPoint & );

    void contextMenu( QWidget *, const QPoint & );

    void movedTab( int, int );

    void mouseDoubleClick();

    void mouseDoubleClick( QWidget * );

    void mouseMiddleClick();

    void mouseMiddleClick( QWidget * );

    void closeRequest( QWidget * );

  protected:
    virtual void mouseDoubleClickEvent( QMouseEvent* );
    virtual void mousePressEvent( QMouseEvent* );
    virtual void dragEnterEvent( QDragEnterEvent* );
    virtual void dragMoveEvent( QDragMoveEvent* );
    virtual void dropEvent( QDropEvent* );
    int tabBarWidthForMaxChars( int );
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent( QWheelEvent* );
#endif
    virtual void resizeEvent( QResizeEvent* );
    virtual void tabInserted( int );
    virtual void tabRemoved ( int );


  protected Q_SLOTS:
    virtual void receivedDropEvent( int, QDropEvent* );
    virtual void initiateDrag( int );
    virtual void contextMenu( int, const QPoint& );
    virtual void mouseDoubleClick( int );
    virtual void mouseMiddleClick( int );
    virtual void closeRequest( int );
#ifndef QT_NO_WHEELEVENT
    virtual void wheelDelta( int );
#endif

  private:
    class Private;
    Private * const d;
};

#endif
