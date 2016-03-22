#pragma once

#ifndef _TRAININGWINDOW_H_
#define _TRAININGWINDOW_H_

// Includes
#include "Graphics/AbstractWindow.hpp"

// Forward declarations

class TrainingWindow : public AbstractWindow
{
private:
	wxPanel* createNNColumn(wxWindow* parent);
	wxPanel* createTrainingColumn(wxWindow* parent);
	wxPanel* createRunningTrainingColumn(wxWindow* parent);
	wxPanel* createDetailsPanel(wxWindow* parent);
	void createMenuBar();
protected:
	DECLARE_EVENT_TABLE();
public:
	TrainingWindow();
	void OnClick(wxCommandEvent& event);
};

#endif