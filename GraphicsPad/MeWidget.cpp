#include "MeWidget.h"
#include <Qt\qdebug.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <MeGlWindow.h>
#include <GuiComponents\DebugSlider.h>
#include <QtGui\QRadioButton>
#include <QtGui\qlabel.h>


MeWidget::MeWidget()
{
	// contains the controllers and the GL widget (screen)
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);
	
	// contains the controllers (seleect object, light intensity, ...)
	QVBoxLayout* controlsLayout;
	mainLayout->addLayout(controlsLayout = new QVBoxLayout);
	
	// the GL widget (screen)
	mainLayout->addWidget(meGlWindow = new MeGlWindow(&theModel));
	
	// the light intensity layout
	QHBoxLayout* lightintensityLayout;
	controlsLayout->addLayout(lightintensityLayout = new QHBoxLayout);
	//  the light intensity layout's widgets
	lightintensityLayout->addWidget(lightIntensityText = new QLabel("Light intensity")); // the text
	lightintensityLayout->addWidget(lightXSlider = new DebugSlider(0.5f, 0, 1, true, 40)); // the slider
	connect(lightXSlider, SIGNAL(valueChanged(float)), this, SLOT(sliderValueChanged()));

	// the checkboxes layout 
	QHBoxLayout* checkboxesLayout;
	controlsLayout->addLayout(checkboxesLayout = new QHBoxLayout);
	// the checkboxes layout's widgets
	checkboxesLayout->addWidget(pickObjectText = new QLabel("Pick an object to move"));
	checkboxesLayout->addWidget(pickLightButton = new QRadioButton("Light source"));
	checkboxesLayout->addWidget(pickViechleButton = new QRadioButton("viechle"));
	checkboxesLayout->addWidget(pickTeapotButton = new QRadioButton("Teapot"));
	checkboxesLayout->addWidget(pickArrowButton = new QRadioButton("Arrow"));
	checkboxesLayout->addWidget(pickCameraButton = new QRadioButton("Camera"));

	connect(pickLightButton, SIGNAL(toggled(bool)), this, SLOT(lightPicked()));
	connect(pickViechleButton, SIGNAL(toggled(bool)), this, SLOT(viechlePicked()));
	connect(pickTeapotButton, SIGNAL(toggled(bool)), this, SLOT(teapotPicked()));
	connect(pickArrowButton, SIGNAL(toggled(bool)), this, SLOT(arrowPicked()));
	connect(pickCameraButton, SIGNAL(toggled(bool)), this, SLOT(cameraPicked()));




	// contains the controllers (seleect object, light intensity, ...)
	QVBoxLayout* shadersLayout;
	mainLayout->addLayout(shadersLayout = new QVBoxLayout);

	// the shader's checkbox layout
	QHBoxLayout* shader_checkboxesLayout;
	controlsLayout->addLayout(shader_checkboxesLayout = new QHBoxLayout);
	// the the shader's checkbox layout widgets
	shader_checkboxesLayout->addWidget(pick_shading_type_text = new QLabel("Pick a shading type"));
	shader_checkboxesLayout->addWidget(per_vertex_shading = new QRadioButton("per vertex shading"));
	shader_checkboxesLayout->addWidget(per_fragment_shading = new QRadioButton("per fragment shading"));

	connect(per_vertex_shading, SIGNAL(toggled(bool)), this, SLOT(per_vertex_shading_picked()));
	connect(per_fragment_shading, SIGNAL(toggled(bool)), this, SLOT(per_fragment_shading_picked()));

}

void MeWidget::sliderValueChanged()
{
	theModel.lightIntensity = lightXSlider->value();
	meGlWindow->repaint();
}
void MeWidget::lightPicked() {
	theModel.pickedObject = 0;
	meGlWindow->repaint();
}
void MeWidget::viechlePicked() {
	theModel.pickedObject = 1;
	meGlWindow->repaint();
}
void MeWidget::teapotPicked() {
	theModel.pickedObject = 2;
	meGlWindow->repaint();
}
void MeWidget::arrowPicked() {
	theModel.pickedObject = 3;
	meGlWindow->repaint();
}
void MeWidget::cameraPicked() {
	theModel.pickedObject = 4;
	meGlWindow->repaint();
}
void MeWidget::per_vertex_shading_picked() {
	theModel.shadingType = 1;
	meGlWindow->repaint();
}
void MeWidget::per_fragment_shading_picked() {
	theModel.shadingType = 0;
	meGlWindow->repaint();
}