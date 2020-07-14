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

#include <QApplication>
#include <QDragMoveEvent>
#include <QStyle>
#include <QTextDocument>

#include "QatPresenter/QatTabBar.h"

#include "QatPresenter/QatTabWidget.h"

class QatTabWidget::Private
{
  public:
    Private( QatTabWidget *parent )
      : m_parent( parent ),
        m_automaticResizeTabs( false )
    {
      m_maxLength=30;m_minLength=3;//TK-fixme
      m_currentMaxLength = m_minLength;
    }

    QatTabWidget *m_parent;
    bool m_automaticResizeTabs;
    int m_maxLength;
    int m_minLength;
    int m_currentMaxLength;

    //holds the full names of the tab, otherwise all we
    //know about is the shortened name
    QStringList m_tabNames;

    bool isEmptyTabbarSpace( const QPoint & )  const;
    void resizeTabs( int changedTabIndex = -1 );
    void updateTab( int index );
};

//Added by TK:
QatTabBar* QatTabWidget::getQatTabBar() {
  return static_cast<QatTabBar*>(tabBar());
}

bool QatTabWidget::Private::isEmptyTabbarSpace( const QPoint &point ) const
{
  QSize size( m_parent->tabBar()->sizeHint() );
  if ( ( m_parent->tabPosition() == QTabWidget::North && point.y() < size.height() ) ||
       ( m_parent->tabPosition() == QTabWidget::South && point.y() > (m_parent->height() - size.height() ) ) ) {

    QWidget *rightcorner = m_parent->cornerWidget( Qt::TopRightCorner );
    if ( rightcorner ) {
      if ( point.x() >= m_parent->width()-rightcorner->width() )
        return false;
    }

    QWidget *leftcorner = m_parent->cornerWidget( Qt::TopLeftCorner );
    if ( leftcorner ) {
      if ( point.x() <= leftcorner->width() )
        return false;
    }

    for ( int i = 0; i < m_parent->count(); ++i )
      if ( m_parent->tabBar()->tabRect( i ).contains( m_parent->tabBar()->mapFromParent( point ) ) )
        return false;

    return true;
  }

  return false;
}

void QatTabWidget::Private::resizeTabs( int changeTabIndex )
{
  int newMaxLength;
  if ( m_automaticResizeTabs ) {
    // Calculate new max length
    newMaxLength = m_maxLength;
    int lcw = 0, rcw = 0;

    int tabBarHeight = m_parent->tabBar()->sizeHint().height();
    if ( m_parent->cornerWidget( Qt::TopLeftCorner ) && m_parent->cornerWidget( Qt::TopLeftCorner )->isVisible() )
      lcw = qMax( m_parent->cornerWidget( Qt::TopLeftCorner )->width(), tabBarHeight );

    if ( m_parent->cornerWidget( Qt::TopRightCorner ) && m_parent->cornerWidget( Qt::TopRightCorner )->isVisible() )
      rcw = qMax( m_parent->cornerWidget( Qt::TopRightCorner )->width(), tabBarHeight );

    int maxTabBarWidth = m_parent->width() - lcw - rcw;

    for ( ; newMaxLength > m_minLength; newMaxLength-- ) {
      if ( m_parent->tabBarWidthForMaxChars( newMaxLength ) < maxTabBarWidth )
        break;
    }
  } else
    newMaxLength = 4711;

  // Update hinted or all tabs
  if ( m_currentMaxLength != newMaxLength ) {
    m_currentMaxLength = newMaxLength;
    for ( int i = 0; i < m_parent->count(); ++i )
      updateTab( i );
  } else if ( changeTabIndex != -1 )
    updateTab( changeTabIndex );
}

void QatTabWidget::Private::updateTab( int index )
{
  QString title = m_automaticResizeTabs ? m_tabNames[ index ] : m_parent->QTabWidget::tabText( index );
  m_parent->setTabToolTip( index, QString() );

  if ( title.length() > m_currentMaxLength ) {
    if ( Qt::mightBeRichText( title ) )
      m_parent->setTabToolTip( index,title );
    else
      m_parent->setTabToolTip( index, title );
  }

  //TK-fixme. Just use elide here?
  //TK-fixme  title = KStringHandler::rsqueeze( title, m_currentMaxLength ).leftJustified( m_minLength, ' ' );
  title.replace( '&', "&&" );

  if ( m_parent->QTabWidget::tabText( index ) != title )
    m_parent->QTabWidget::setTabText( index, title );
}

QatTabWidget::QatTabWidget( QWidget *parent, Qt::WindowFlags flags )
  : QTabWidget( parent ),
    d( new Private( this ) )
{
  setWindowFlags( flags );
  setTabBar( new QatTabBar( this ) );
  setObjectName( "tabbar" );
  setAcceptDrops( true );

  connect(tabBar(), SIGNAL(contextMenu( int, const QPoint & )), SLOT(contextMenu( int, const QPoint & )));
  connect(tabBar(), SIGNAL(mouseDoubleClick( int )), SLOT(mouseDoubleClick( int )));
  connect(tabBar(), SIGNAL(mouseMiddleClick( int )), SLOT(mouseMiddleClick( int )));
  connect(tabBar(), SIGNAL(initiateDrag( int )), SLOT(initiateDrag( int )));
  connect(tabBar(), SIGNAL(testCanDecode(const QDragMoveEvent *, bool & )), SIGNAL(testCanDecode(const QDragMoveEvent *, bool & )));
  connect(tabBar(), SIGNAL(receivedDropEvent( int, QDropEvent * )), SLOT(receivedDropEvent( int, QDropEvent * )));
  connect(tabBar(), SIGNAL(moveTab( int, int )), SLOT(moveTab( int, int )));
  connect(tabBar(), SIGNAL(closeRequest( int )), SLOT(closeRequest( int )));
#ifndef QT_NO_WHEELEVENT
  connect(tabBar(), SIGNAL(wheelDelta( int )), SLOT(wheelDelta( int )));
#endif
}

QatTabWidget::~QatTabWidget()
{
  delete d;
}

/*void QatTabWidget::insertTab( QWidget *child, const QString &label, int index )
{
  QTabWidget::insertTab( child, label, index );
}

void QatTabWidget::insertTab( QWidget *child, const QIcon& iconset, const QString &label, int index )
{
  QTabWidget::insertTab( child, iconset, label, index );
}

void QatTabWidget::insertTab( QWidget *child, QTab *tab, int index )
{
  QTabWidget::insertTab( child, tab, index);
  if ( d->m_automaticResizeTabs ) {
    if ( index < 0 || index >= count() ) {
      d->m_tabNames.append( tab->text() );
      d->resizeTabs( d->m_tabNames.count()-1 );
    }
    else {
      d->m_tabNames.insert( d->m_tabNames.at( index ), tab->text() );
      d->resizeTabs( index );
    }
  }
}*/

void QatTabWidget::setTabBarHidden( bool hide )
{
  QWidget *rightcorner = cornerWidget( Qt::TopRightCorner );
  QWidget *leftcorner = cornerWidget( Qt::TopLeftCorner );

  if ( hide ) {
    if ( leftcorner ) leftcorner->hide();
    if ( rightcorner ) rightcorner->hide();
    tabBar()->hide();
  } else {
    tabBar()->show();
    if ( leftcorner ) leftcorner->show();
    if ( rightcorner ) rightcorner->show();
  }
}

bool QatTabWidget::isTabBarHidden() const
{
  return !( tabBar()->isVisible() );
}

void QatTabWidget::setTabTextColor( int index, const QColor& color )
{
  tabBar()->setTabTextColor( index, color );
}

QColor QatTabWidget::tabTextColor( int index ) const
{
  return tabBar()->tabTextColor( index );
}

void QatTabWidget::setTabReorderingEnabled( bool on)
{
  static_cast<QatTabBar*>(tabBar())->setTabReorderingEnabled( on );
}

bool QatTabWidget::isTabReorderingEnabled() const
{
  return static_cast<QatTabBar*>(tabBar())->isTabReorderingEnabled();
}

void QatTabWidget::setTabCloseActivatePrevious( bool previous)
{
  static_cast<QatTabBar*>(tabBar())->setTabCloseActivatePrevious( previous );
}

bool QatTabWidget::tabCloseActivatePrevious() const
{
  return static_cast<QatTabBar*>(tabBar())->tabCloseActivatePrevious();
}

int QatTabWidget::tabBarWidthForMaxChars( int /*maxLength*/ )
{
  int hframe;
  hframe  = tabBar()->style()->pixelMetric( QStyle::PM_TabBarTabHSpace, 0L, tabBar() );

  QFontMetrics fm = tabBar()->fontMetrics();
  int x = 0;
  for ( int i = 0; i < count(); ++i ) {
    QString newTitle = d->m_tabNames[ i ];
  //TK-fixme. Just use elide here?
    //TK-fixme    newTitle = KStringHandler::rsqueeze( newTitle, maxLength ).leftJustified( d->m_minLength, ' ' );

    int lw = fm.width( newTitle );
    int iw = 0;
    if ( !tabBar()->tabIcon( i ).isNull() ) {
      iw = tabBar()->tabIcon( i ).pixmap( style()->pixelMetric( QStyle::PM_SmallIconSize ), QIcon::Normal ).width() + 4;
    }
    x += ( tabBar()->style()->sizeFromContents( QStyle::CT_TabBarTab, 0L,
						QSize( qMax( lw + hframe + iw, QApplication::globalStrut().width() ), 0 ),
						this ) ).width();
  }
  
  return x;
}

QString QatTabWidget::tabText( int index ) const
{
  if ( d->m_automaticResizeTabs ) {
    if ( index >= 0 && index < count() )
      return d->m_tabNames[ index ];
    else
      return QString();
  }
  else
    return QTabWidget::tabText( index );
}

void QatTabWidget::setTabText( int index, const QString &text )
{
  QTabWidget::setTabText( index, text );
  if ( d->m_automaticResizeTabs ) {
    if ( index != -1 ) {
      d->m_tabNames[ index ] = text;
      d->resizeTabs( index );
    }
  }
}


void QatTabWidget::dragEnterEvent( QDragEnterEvent *event )
{
  event->setAccepted( true );
  QTabWidget::dragEnterEvent( event );
}

void QatTabWidget::dragMoveEvent( QDragMoveEvent *event )
{
  if ( d->isEmptyTabbarSpace( event->pos() ) ) {
    bool accept = false;
    // The receivers of the testCanDecode() signal has to adjust
    // 'accept' accordingly.
    emit testCanDecode( event, accept);

    event->setAccepted( accept );
    return;
  }

  event->setAccepted( false );
  QTabWidget::dragMoveEvent( event );
}

void QatTabWidget::dropEvent( QDropEvent *event )
{
  if ( d->isEmptyTabbarSpace( event->pos() ) ) {
    emit ( receivedDropEvent( event ) );
    return;
  }

  QTabWidget::dropEvent( event );
}

#ifndef QT_NO_WHEELEVENT
void QatTabWidget::wheelEvent( QWheelEvent *event )
{
  if ( event->orientation() == Qt::Horizontal )
    return;

  if ( d->isEmptyTabbarSpace( event->pos() ) )
    wheelDelta( event->delta() );
  else
    event->ignore();
}

void QatTabWidget::wheelDelta( int delta )
{
  if ( count() < 2 )
    return;

  int page = currentIndex();
  if ( delta < 0 )
     page = (page + 1) % count();
  else {
    page--;
    if ( page < 0 )
      page = count() - 1;
  }
  setCurrentIndex( page );
}
#endif

void QatTabWidget::mouseDoubleClickEvent( QMouseEvent *event )
{
  if ( event->button() != Qt::LeftButton )
    return;

  if ( d->isEmptyTabbarSpace( event->pos() ) ) {
    emit( mouseDoubleClick() );
    return;
  }

  QTabWidget::mouseDoubleClickEvent( event );
}

void QatTabWidget::mousePressEvent( QMouseEvent *event )
{
  if ( event->button() == Qt::RightButton ) {
    if ( d->isEmptyTabbarSpace( event->pos() ) ) {
      emit( contextMenu( mapToGlobal( event->pos() ) ) );
      return;
    }
  } else if ( event->button() == Qt::MidButton ) {
    if ( d->isEmptyTabbarSpace( event->pos() ) ) {
      emit( mouseMiddleClick() );
      return;
    }
  }

  QTabWidget::mousePressEvent( event );
}

void QatTabWidget::receivedDropEvent( int index, QDropEvent *event )
{
  emit( receivedDropEvent( widget( index ), event ) );
}

void QatTabWidget::initiateDrag( int index )
{
  emit( initiateDrag( widget( index ) ) );
}

void QatTabWidget::contextMenu( int index, const QPoint &point )
{
  emit( contextMenu( widget( index ), point ) );
}

void QatTabWidget::mouseDoubleClick( int index )
{
  emit( mouseDoubleClick( widget( index ) ) );
}

void QatTabWidget::mouseMiddleClick( int index )
{
  emit( mouseMiddleClick( widget( index ) ) );
}

void QatTabWidget::moveTab( int from, int to )
{
  QString tablabel = tabText( from );
  QWidget *w = widget( from );
  QColor color = tabTextColor( from );
  QIcon tabiconset = tabIcon( from );
  QString tabtooltip = tabToolTip( from );
  bool current = ( from == currentIndex() );
  bool enabled = isTabEnabled( from );

  bool blocked = blockSignals( true );
  removeTab( from );

  // Work-around kmdi brain damage which calls showPage() in insertTab()
  insertTab( to, w, tablabel );
  if ( d->m_automaticResizeTabs ) {
    if ( to < 0 || to >= count() )
      d->m_tabNames.append( QString() );
    else
      d->m_tabNames.insert( to, QString() );
  }

  setTabIcon( to, tabiconset );
  setTabText( to, tablabel );
  setTabToolTip( to, tabtooltip );
  setTabTextColor( to, color );
  if ( current )
    setCurrentIndex( to );
  setTabEnabled( to, enabled );
  blockSignals( blocked );

  emit ( movedTab( from, to ) );
}

void QatTabWidget::removePage( QWidget *widget )
{
  QTabWidget::removeTab( indexOf( widget ) );
  if ( d->m_automaticResizeTabs )
    d->resizeTabs();
}

void QatTabWidget::removeTab( int index )
{
  QTabWidget::removeTab( index );
  if ( d->m_automaticResizeTabs )
    d->resizeTabs();
}

void QatTabWidget::setAutomaticResizeTabs( bool enabled )
{
  if ( d->m_automaticResizeTabs == enabled )
    return;

  d->m_automaticResizeTabs = enabled;
  if ( enabled ) {
    d->m_tabNames.clear();
    for ( int i = 0; i < count(); ++i )
      d->m_tabNames.append( tabBar()->tabText( i ) );
  } else
    for ( int i = 0; i < count(); ++i )
      tabBar()->setTabText( i, d->m_tabNames[ i ] );

  d->resizeTabs();
}

bool QatTabWidget::automaticResizeTabs() const
{
  return d->m_automaticResizeTabs;
}

void QatTabWidget::closeRequest( int index )
{
  emit( closeRequest( widget( index ) ) );
}

void QatTabWidget::resizeEvent( QResizeEvent *event )
{
  QTabWidget::resizeEvent( event );
  d->resizeTabs();
}

void QatTabWidget::tabInserted( int idx )
{
   d->m_tabNames.insert( idx, tabBar()->tabText( idx ) );
}

void QatTabWidget::tabRemoved( int idx )
{
   d->m_tabNames.removeAt( idx );
}
