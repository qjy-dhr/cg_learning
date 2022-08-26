#pragma once

#include <ui_minidraw.h>
#include <QActionGroup>
#include <QToolBar>
#include "viewwidget.h"

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>

class MiniDraw : public QMainWindow {
	Q_OBJECT

public:
	MiniDraw(QWidget* parent = 0);
	~MiniDraw();

	QMenu* pMenu;
	QToolBar* pToolBar;
	QAction* Action_About;
	QAction* Action_Line;
	QAction* Action_Rect;
	QAction* Action_Ellipse;
	QAction* Action_Pen;
	QAction* Action_Save;
	QAction* Action_Clean;
	QAction* Action_Back;
	QAction* Action_Quit;

	QActionGroup* group;


	void Creat_Menu();
	void Creat_ToolBar();
	void Creat_Action();

	void AboutBox();
	void quit();

private:
	Ui::MiniDrawClass ui;
	ViewWidget* view_widget_;
};
