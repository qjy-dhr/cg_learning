﻿#include "mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	setWindowTitle("ImageEdit");
	setWindowIcon(QIcon(":/MainWindow/Resources/images/icon.png"));

	mdi_area_ = new QMdiArea;
	mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdi_area_);

	window_mapper_ = new QSignalMapper(this);
	connect(window_mapper_, SIGNAL(mapped(QWidget*)), this, SLOT(SetActiveSubWindow(QWidget*)));

	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateActions()
{
	// 	action_new_ = new QAction(QIcon(":/MainWindow/Resources/images/new.png"), tr("&New"), this);
	// 	action_new_->setShortcut(QKeySequence::New);
	// 	action_new_->setStatusTip(tr("Create a new file"));

		// File IO
	action_open_ = new QAction(QIcon(":/MainWindow/Resources/images/open.png"), tr("&Open..."), this);
	action_open_->setShortcuts(QKeySequence::Open);
	action_open_->setStatusTip(tr("Open an existing file"));
	connect(action_open_, SIGNAL(triggered()), this, SLOT(Open()));

	action_save_ = new QAction(QIcon(":/MainWindow/Resources/images/save.png"), tr("&Save"), this);
	action_save_->setShortcuts(QKeySequence::Save);
	action_save_->setStatusTip(tr("Save the document to disk"));
	connect(action_save_, SIGNAL(triggered()), this, SLOT(Save()));

	action_saveas_ = new QAction(tr("Save &As..."), this);
	action_saveas_->setShortcuts(QKeySequence::SaveAs);
	action_saveas_->setStatusTip(tr("Save the document under a new name"));
	connect(action_saveas_, SIGNAL(triggered()), this, SLOT(SaveAs()));

	// Image processing
	action_invert_ = new QAction(tr("Inverse"), this);
	action_invert_->setStatusTip(tr("Invert all pixel value in the image"));
	connect(action_invert_, SIGNAL(triggered()), this, SLOT(Invert()));

	action_mirror_ = new QAction(tr("Mirror"), this);
	action_mirror_->setStatusTip(tr("Mirror image vertically or horizontally"));
	connect(action_mirror_, SIGNAL(triggered()), this, SLOT(Mirror()));

	action_gray_ = new QAction(tr("Grayscale"), this);
	action_gray_->setStatusTip(tr("Gray-scale map"));
	connect(action_gray_, SIGNAL(triggered()), this, SLOT(GrayScale()));

	action_restore_ = new QAction(tr("Restore"), this);
	action_restore_->setStatusTip(tr("Show origin image"));
	connect(action_restore_, SIGNAL(triggered()), this, SLOT(Restore()));

	// Poisson image editting
	action_choose_circle_ = new QAction(QIcon(":/MainWindow/Resources/images/choose.png"), tr("Choose the area"), this);
	connect(action_choose_circle_, SIGNAL(triggered()), this, SLOT(ChooseCircle()));

	action_paste_ = new QAction(tr("Paste"), this);
	connect(action_paste_, SIGNAL(triggered()), this, SLOT(Paste()));

	action_poisson_ = new QAction(tr("Poisson"), this);
	connect(action_poisson_, SIGNAL(triggered()), this, SLOT(Poisson()));

	action_mix_poisson_ = new QAction(tr("MixPoisson"), this);
	connect(action_mix_poisson_, SIGNAL(triggered()), this, SLOT(MixPoisson()));
}

void MainWindow::CreateMenus()
{
	menu_file_ = menuBar()->addMenu(tr("&File"));
	menu_file_->setStatusTip(tr("File menu"));
	//	menu_file_->addAction(action_new_);
	menu_file_->addAction(action_open_);
	menu_file_->addAction(action_save_);
	menu_file_->addAction(action_saveas_);

	//	menu_file_->addAction(action_choose_polygon_);

	menu_edit_ = menuBar()->addMenu(tr("&Edit"));
	menu_edit_->setStatusTip(tr("Edit menu"));
	menu_edit_->addAction(action_invert_);
	menu_edit_->addAction(action_mirror_);
	menu_edit_->addAction(action_gray_);
	menu_edit_->addAction(action_restore_);
}

void MainWindow::CreateToolBars()
{
	toolbar_file_ = addToolBar(tr("File"));
	// toolbar_file_->addAction(action_new_);
	toolbar_file_->addAction(action_open_);
	toolbar_file_->addAction(action_save_);

	// Add separator in toolbar 
	toolbar_file_->addSeparator();
	toolbar_file_->addAction(action_invert_);
	toolbar_file_->addAction(action_mirror_);
	toolbar_file_->addAction(action_gray_);
	toolbar_file_->addAction(action_restore_);

	// Poisson Image Editing
	toolbar_file_->addSeparator();
	toolbar_file_->addAction(action_choose_circle_);
	toolbar_file_->addAction(action_paste_);
	toolbar_file_->addAction(action_paste_);
	toolbar_file_->addAction(action_poisson_);
	toolbar_file_->addAction(action_mix_poisson_);
}

void MainWindow::CreateStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::Open()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty())
	{
		QMdiSubWindow* existing = FindChild(filename);

		if (existing)
		{
			mdi_area_->setActiveSubWindow(existing);
			return;
		}

		ChildWindow* child = CreateChildWindow();
		if (child->LoadFile(filename))
		{
			statusBar()->showMessage(tr("File loaded"), 2000);
			child->show();

			// Change size of the child window so it can fit image size
			int x = child->geometry().x();
			int y = child->geometry().y();
			int width = child->imagewidget_->ImageWidth();
			int height = child->imagewidget_->ImageHeight();
			mdi_area_->activeSubWindow()->setFixedSize(width + 2 * x, height + x + y);
		}
		else
		{
			child->close();
		}
	}
}

void MainWindow::Save()
{
	SaveAs();
}

ChildWindow* MainWindow::GetChildWindow() {
	QMdiSubWindow* activeSubWindow = mdi_area_->activeSubWindow();
	if (!activeSubWindow)
		return nullptr;

	return qobject_cast<ChildWindow*>(activeSubWindow->widget());
}

void MainWindow::SaveAs()
{
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->SaveAs();
}

ChildWindow* MainWindow::CreateChildWindow()
{
	ChildWindow* child = new ChildWindow;
	mdi_area_->addSubWindow(child);

	return child;
}

void MainWindow::SetActiveSubWindow(QWidget* window)
{
	if (!window)
	{
		return;
	}

	mdi_area_->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

void MainWindow::Invert()
{
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->Invert();
}

void MainWindow::Mirror()
{
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->Mirror();
}

void MainWindow::GrayScale()
{
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->TurnGray();
}

void MainWindow::Restore()
{
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->Restore();
}

void MainWindow::ChooseCircle()
{
	// Set source child window
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->set_draw_status_to_choose();
	child_source_ = window;
}

void MainWindow::Paste()
{
	// Paste image rect region to object image
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->set_draw_status_to_paste();
	window->imagewidget_->set_source_window(child_source_);
	window->imagewidget_->set_paste_status_to_normal();
}

void MainWindow::Poisson()
{
	// Paste image rect region to object image
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->set_draw_status_to_paste();
	window->imagewidget_->set_source_window(child_source_);
	window->imagewidget_->set_paste_status_to_poisson();
}

void MainWindow::MixPoisson()
{
	// Paste image rect region to object image
	ChildWindow* window = GetChildWindow();
	if (!window)
		return;
	window->imagewidget_->set_draw_status_to_paste();
	window->imagewidget_->set_source_window(child_source_);
	window->imagewidget_->set_paste_status_to_mixpoisson();

}

QMdiSubWindow *MainWindow::FindChild(const QString &filename)
{
	QString canonical_filepath = QFileInfo(filename).canonicalFilePath();

	foreach (QMdiSubWindow *window, mdi_area_->subWindowList())
	{
		ChildWindow *child = qobject_cast<ChildWindow *>(window->widget());
		if (child->current_file() == canonical_filepath)
		{
			return window;
		}
	}

	return 0;
}
