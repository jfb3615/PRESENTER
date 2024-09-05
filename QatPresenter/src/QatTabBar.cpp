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

#include <QTimer>
#include <QApplication>
#include <QMouseEvent>

#include "QatPresenter/QatTabBar.h"
#define QT_NO_WHEELEVENT 1
class QatTabBar::Private
{
  public:
    Private()
      : mReorderStartTab( -1 ),
        mReorderPreviousTab( -1 ),
        mDragSwitchTab( -1 ),
        mTabReorderingEnabled( false ),
        mTabCloseActivatePrevious( false )
    {
    }

    QPoint mDragStart;
    int mReorderStartTab;
    int mReorderPreviousTab;
    int mDragSwitchTab;
    QTimer *mActivateDragSwitchTabTimer;

    bool mTabReorderingEnabled;
    bool mTabCloseActivatePrevious;

};

QatTabBar::QatTabBar( QWidget *parent )
    : QTabBar( parent ),
      d( new Private )
{
  setAcceptDrops( true );
  setMouseTracking( true );

  //d->mEnableCloseButtonTimer = new QTimer( this );
  //connect( d->mEnableCloseButtonTimer, SIGNAL( timeout() ), SLOT( enableCloseButton() ) );

  d->mActivateDragSwitchTabTimer = new QTimer( this );
  d->mActivateDragSwitchTabTimer->setSingleShot( true );
  connect( d->mActivateDragSwitchTabTimer, SIGNAL( timeout() ), SLOT( activateDragSwitchTab() ) );

  //connect( this, SIGNAL( layoutChanged() ), SLOT( onLayoutChange() ) );
}

QatTabBar::~QatTabBar()
{
  delete d;
}

void QatTabBar::mouseDoubleClickEvent( QMouseEvent *event )
{
  if ( event->button() != Qt::LeftButton )
    return;

  int tab = selectTab( event->pos() );
  if ( tab != -1 ) {
    emit mouseDoubleClick( tab );
    return;
  }

  QTabBar::mouseDoubleClickEvent( event );
}

void QatTabBar::mousePressEvent( QMouseEvent *event )
{
  if ( event->button() == Qt::LeftButton ) {
    //d->mEnableCloseButtonTimer->stop();
    d->mDragStart = event->pos();
  } else if( event->button() == Qt::RightButton ) {
    int tab = selectTab( event->pos() );
    if ( tab != -1 ) {
      emit contextMenu( tab, mapToGlobal( event->pos() ) );
      return;
    }
  }

  QTabBar::mousePressEvent( event );
}

void QatTabBar::mouseMoveEvent( QMouseEvent *event )
{
  if ( event->buttons() == Qt::LeftButton ) {
    int tab = selectTab( event->pos() );
    if ( d->mDragSwitchTab && tab != d->mDragSwitchTab ) {
      d->mActivateDragSwitchTabTimer->stop();
      d->mDragSwitchTab = 0;
    }

    int delay = 5;//TK fixme KGlobalSettings::dndEventDelay();
    QPoint newPos = event->pos();
    if ( newPos.x() > d->mDragStart.x() + delay || newPos.x() < d->mDragStart.x() - delay ||
         newPos.y() > d->mDragStart.y() + delay || newPos.y() < d->mDragStart.y() - delay ) {
      if ( tab != -1 ) {
        emit initiateDrag( tab );
        return;
      }
    }
  } else if ( event->buttons() == Qt::MiddleButton ) {
    if ( d->mReorderStartTab == -1 ) {
      int delay = 5;//TK fixme KGlobalSettings::dndEventDelay();
      QPoint newPos = event->pos();

      if ( newPos.x() > d->mDragStart.x() + delay || newPos.x() < d->mDragStart.x() - delay ||
           newPos.y() > d->mDragStart.y() + delay || newPos.y() < d->mDragStart.y() - delay ) {
        int tab = selectTab( event->pos() );
        if ( tab != -1 && d->mTabReorderingEnabled ) {
          d->mReorderStartTab = tab;
          grabMouse( Qt::SizeAllCursor );
          return;
        }
      }
    } else {
      int tab = selectTab( event->pos() );
      if ( tab != -1 ) {
        int reorderStopTab = tab;
        if ( d->mReorderStartTab != reorderStopTab && d->mReorderPreviousTab != reorderStopTab ) {
          emit moveTab( d->mReorderStartTab, reorderStopTab );

          d->mReorderPreviousTab = d->mReorderStartTab;
          d->mReorderStartTab = reorderStopTab;

          return;
        }
      }
    }
  }

  QTabBar::mouseMoveEvent( event );
}


void QatTabBar::activateDragSwitchTab()
{
  int tab = selectTab( mapFromGlobal( QCursor::pos() ) );
  if ( tab != -1 && d->mDragSwitchTab == tab )
    setCurrentIndex( d->mDragSwitchTab );

  d->mDragSwitchTab = 0;
}

void QatTabBar::mouseReleaseEvent( QMouseEvent *event )
{
  if ( event->button() == Qt::MiddleButton ) {
    if ( d->mReorderStartTab == -1 ) {
      int tab = selectTab( event->pos() );
      if ( tab != -1 ) {
        emit mouseMiddleClick( tab );
        return;
      }
    } else {
      releaseMouse();
      setCursor( Qt::ArrowCursor );
      d->mReorderStartTab = -1;
      d->mReorderPreviousTab = -1;
    }
  }

  QTabBar::mouseReleaseEvent( event );
}

void QatTabBar::dragEnterEvent( QDragEnterEvent *event )
{
  event->setAccepted( true );
  QTabBar::dragEnterEvent( event );
}

void QatTabBar::dragMoveEvent( QDragMoveEvent *event )
{
  int tab = selectTab( event->position().toPoint() );
  if ( tab != -1 ) {
    bool accept = false;
    // The receivers of the testCanDecode() signal has to adjust
    // 'accept' accordingly.
    emit testCanDecode( event, accept );
    if ( accept && tab != currentIndex() ) {
      d->mDragSwitchTab = tab;
      d->mActivateDragSwitchTabTimer->start( QApplication::doubleClickInterval() * 2 );
    }

    event->setAccepted( accept );
    return;
  }

  event->setAccepted( false );
  QTabBar::dragMoveEvent( event );
}

void QatTabBar::dropEvent( QDropEvent *event )
{
  int tab = selectTab( event->position().toPoint() );
  if ( tab != -1 ) {
    d->mActivateDragSwitchTabTimer->stop();
    d->mDragSwitchTab = 0;
    emit receivedDropEvent( tab , event );
    return;
  }

  QTabBar::dropEvent( event );
}

#ifndef QT_NO_WHEELEVENT
void QatTabBar::wheelEvent( QWheelEvent *event )
{
  if ( event->orientation() == Qt::Horizontal )
    return;

  emit( wheelDelta( event->delta() ) );
}
#endif

bool QatTabBar::isTabReorderingEnabled() const
{
  return d->mTabReorderingEnabled;
}

void QatTabBar::setTabReorderingEnabled( bool on )
{
  d->mTabReorderingEnabled = on;
}

bool QatTabBar::tabCloseActivatePrevious() const
{
  return d->mTabCloseActivatePrevious;
}

void QatTabBar::setTabCloseActivatePrevious( bool on )
{
  d->mTabCloseActivatePrevious = on;
}


void QatTabBar::tabLayoutChange()
{
  d->mActivateDragSwitchTabTimer->stop();
  d->mDragSwitchTab = 0;
}

int QatTabBar::selectTab( const QPoint &pos ) const
{
  for ( int i = 0; i < count(); ++i )
    if ( tabRect( i ).contains( pos ) )
      return i;

  return -1;
}

