#include "minidraw.h"


MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedSize(2000, 1200);
	this->setWindowTitle("MiniDraw");
	this->setWindowIcon(QIcon(":/Resource/icon.png"));
	this->setStatusBar(nullptr);

	view_widget_ = new ViewWidget();
	Creat_Action();
	Creat_ToolBar();
	Creat_Menu();

	setMouseTracking(true);
	setCentralWidget(view_widget_);
}

void MiniDraw::Creat_Action() {
	group = new QActionGroup(this);

	Action_About = new QAction(tr("&About"), this);
	Action_About->setIcon(QIcon(":/Resource/about.png"));//图标
	Action_About->setShortcut(QKeySequence(tr("Ctrl+A")));//热键
	Action_About->setCheckable(true);
	connect(Action_About, &QAction::triggered, this, &MiniDraw::AboutBox);


	Action_Save = new QAction(tr("&Save"), this);
	Action_Save->setIcon(QIcon(":/Resource/save.png"));//图标
	Action_Save->setShortcut(QKeySequence(tr("Ctrl+S")));//热键
	Action_Save->setToolTip(tr("Save"));
	Action_Save->setCheckable(true);
	connect(Action_Save, SIGNAL(triggered()), view_widget_, SLOT(savePic()));

	Action_Line = new QAction(tr("&Line"), this);
	Action_Line->setIcon(QIcon(":/Resource/line.png"));//图标
	Action_Line->setToolTip(tr("Draw a line"));
	Action_Line->setCheckable(true);
	connect(Action_Line, SIGNAL(triggered()), view_widget_, SLOT(setLine()));

	Action_Rect = new QAction(tr("&Rect"), this);
	Action_Rect->setToolTip(tr("Draw a Rect"));
	Action_Rect->setIcon(QIcon(":/Resource/rect.png"));//图标
	Action_Rect->setCheckable(true);
	connect(Action_Rect, &QAction::triggered, view_widget_, &ViewWidget::setRect);

	Action_Ellipse = new QAction(tr("&Ellipse"), this);
	Action_Ellipse->setToolTip(tr("Draw an Ellipse"));
	Action_Ellipse->setIcon(QIcon(":/Resource/ellipse.png"));//图标
	Action_Ellipse->setCheckable(true);
	connect(Action_Ellipse, &QAction::triggered, view_widget_, &ViewWidget::setEllipse);
	
	Action_Pen = new QAction(tr("&Pencil"), this);
	Action_Pen->setToolTip(tr("Pencil"));
	Action_Pen->setIcon(QIcon(":/Resource/pen.png"));//图标
	Action_Pen->setCheckable(true);
	connect(Action_Pen, &QAction::triggered, view_widget_, &ViewWidget::setPen);
	
	Action_Clean = new QAction(tr("&Clean"), this);
	Action_Clean->setToolTip(tr("Clean All"));
	Action_Clean->setIcon(QIcon(":/Resource/clean.png"));//图标
	connect(Action_Clean, &QAction::triggered, view_widget_, &ViewWidget::cleanAll);

	Action_Back = new QAction(tr("&Back"), this);
	Action_Back->setToolTip(tr("Back"));
	Action_Back->setIcon(QIcon(":/Resource/back.png"));//图标
	connect(Action_Back, &QAction::triggered, view_widget_, &ViewWidget::backOnce);

	Action_Quit = new QAction(tr("&Quit"), this);
	Action_Quit->setShortcut(QKeySequence(tr("ALT+F4")));//热键
	Action_Quit->setToolTip(tr("Quit"));
	Action_Quit->setIcon(QIcon(":/Resource/quit.png"));//图标
	connect(Action_Quit, &QAction::triggered, this, &MiniDraw::quit);

}

void MiniDraw::Creat_ToolBar() {
	pToolBar = addToolBar(tr("&Main"));
	pToolBar->setMovable(false);
	pToolBar->setIconSize(QSize(100, 60));
	pToolBar->setStyleSheet("background-color:rgb(247,249,254)");
	pToolBar->addAction(Action_About);
	pToolBar->addAction(Action_Save);
	pToolBar->addAction(Action_Line);
	pToolBar->addAction(Action_Rect);
	pToolBar->addAction(Action_Ellipse);
	pToolBar->addAction(Action_Pen);
	pToolBar->addAction(Action_Clean);
	pToolBar->addAction(Action_Back);

	
	group->addAction(Action_About);
	group->addAction(Action_Line);
	group->addAction(Action_Save);
	group->addAction(Action_Rect);
	group->addAction(Action_Ellipse);
	group->addAction(Action_Pen);
}

void MiniDraw::Creat_Menu() {
	pMenu = menuBar()->addMenu(tr("&Menu"));
	
	pToolBar->setStyleSheet("background-color:rgb(247,249,254)");//设置背景色
	pMenu->addAction(Action_About);
	pMenu->addAction(Action_Save);//添加菜单动作
	pMenu->addAction(Action_Quit);//添加菜单动作


}

void MiniDraw::AboutBox() {
	QMessageBox::about(this, tr("About"), tr("MiniDraw"));
}

void MiniDraw::quit() {
	qApp->quit();
}

MiniDraw::~MiniDraw() {}
