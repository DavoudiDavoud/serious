#include "window.h"
#include "adcreader.h"

#include <cmath>  // for sine stuff


Window::Window() : count(0)
{
	chnum = 0;
	// set up labels
	label = new QLabel("Channel0");
	label->show();
	// set up button
	ch1b = new QPushButton("switch channel");
	connect(ch1b, SIGNAL(clicked()), SLOT(ch1bc()));

	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = sin( M_PI * index/50 );
	}

	curve = new QwtPlotCurve;
	plot = new QwtPlot;
	// make a plot curve from the data and attach it to the plot
	curve->setSamples(xData, yData, plotDataSize);
	curve->attach(plot);

	plot->replot();
	plot->show();



	// plot to the left of knob and thermometer
	hLayout = new QHBoxLayout;
	hLayout->addWidget(ch1b);
	hLayout->addWidget(label);
	hLayout->addWidget(plot);

	setLayout(hLayout);

	// This is a demo for a thread which can be
	// used to read from the ADC asynchronously.
	// At the moment it doesn't do anything else than
	// running in an endless loop and which prints out "tick"
	// every second.
	adcreader = new ADCreader();
	adcreader->start();
}

Window::~Window() {
	// tells the thread to no longer run its endless loop
	adcreader->quit();
	// wait until the run method has terminated
	adcreader->wait();
	delete adcreader;
}

void Window::timerEvent( QTimerEvent * )
{
	double inVal = opch(chnum,count);
	++count;

	// add the new input to the plot
	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );
	yData[plotDataSize-1] = inVal;
	curve->setSamples(xData, yData, plotDataSize);
	plot->replot();


}

void Window::ch1bc(void){
	if(chnum){
	chnum = 0;
    	label->setText("channel0");
    	killTimer(id);
    	id = startTimer(40);
	}
	else{
	chnum = 1;
	label->setText("channel1");
	killTimer(id);
	id = startTimer(40);
	}
}

double Window::opch(bool ch, int c){
	if (ch)
		return cos( M_PI * c/50.0 );
	else 
		return sin( M_PI * c/50.0 );
	
}
