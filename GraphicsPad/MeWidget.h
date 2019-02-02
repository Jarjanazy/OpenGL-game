#pragma once
#include <QtGui\qwidget.h>
#include <MyModel.h>
#include <QtGui\QRadioButton>
#include <QtGui\qlabel.h>


class DebugSlider;
class MeGlWindow;

class MeWidget : public QWidget
{
	Q_OBJECT

	DebugSlider* lightXSlider;
	DebugSlider* lightYSlider;
	DebugSlider* lightZSlider;

	QLabel* lightIntensityText;
	QLabel* pickObjectText;

	QRadioButton* pickLightButton;
	QRadioButton* pickViechleButton;
	QRadioButton* pickTeapotButton;
	QRadioButton* pickArrowButton;
	QRadioButton* pickCameraButton;


	QLabel* pick_shading_type_text;
	QRadioButton* per_vertex_shading;
	QRadioButton* per_fragment_shading;


	MeGlWindow* meGlWindow;

	MyModel theModel;

private slots:
	void sliderValueChanged();
	void lightPicked();
	void viechlePicked();
	void arrowPicked();
	void teapotPicked();
	void cameraPicked();
	void per_vertex_shading_picked();
	void per_fragment_shading_picked();


public:
	MeWidget();
};

