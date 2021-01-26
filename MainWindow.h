#pragma once
#include "Constants.h"
#include "DisplayGrid.h"
#include "Knot.h"

#define declareButton(buttonName) \
	void buttonName##Function(wxCommandEvent& evt); \
	wxButton* buttonName##Button
#define initButton(buttonName, displayText) \
	buttonName##Button = new wxButton(this, wxID_ANY, displayText); \
	buttonName##Button->Bind(wxEVT_BUTTON, &MainWindow::##buttonName##Function, this)

#define declareGenerateButton(SymType) \
	wxButton* generate##SymType##Button
#define initGenerateButton(SymType, displayText) \
	generate##SymType##Button = new wxButton(this, static_cast<unsigned int>(Symmetry::SymType), displayText); \
	generate##SymType##Button->Bind(wxEVT_BUTTON, &MainWindow::generateKnot, this); \
	generateRegionSizer->Add(generate##SymType##Button)

class MainWindow : public wxFrame {

public:
	MainWindow(int h, int w, wxString title);
	~MainWindow();

	void updateSelectCoord();
	void changeSelectCoord(const int iMin_, const int jMin_, const int iMax_, const int jMax_);
	void fixSelectCoord();
	void resetSelectCoord();

	void enableGenerateButtons(bool enable = true);

	void regenExportBox();

private:
	int h, w;
	int iMin, jMin, iMax, jMax; // The "selection numbers", i.e. the current grid selection
	wxFont textFont;

	void initSizerLayout();
	void initDispSizer();
	DisplayGrid* disp;
	Knot* knot;
	wxBoxSizer* mainSizer;
	wxBoxSizer* dispSizer;
	wxBoxSizer* buttonSizer;

	void initGridRegion();
	wxStaticBoxSizer* gridRegionSizer;
		wxBoxSizer* gridInputSizer;
			wxTextCtrl* gridHeight;
			wxTextCtrl* gridWidth;
		declareButton(gridRegen);

	void initSelectRegion();
	wxStaticBoxSizer* selectRegionSizer;
		wxStaticText* selectCoord;		// The display of "selection coordinates", i.e. top-left to bottom-right
		wxBoxSizer* selectButtonSizer;	// The selection buttons
			declareButton(selectToggle);	// This button is a show/hide button, which highlights the selection
			declareButton(selectReset);		// This button resets the selection coordinates

	void initGenerateRegion();
	void generateKnot(wxCommandEvent& evt);
	wxStaticBoxSizer* generateRegionSizer;
		declareGenerateButton(NoSym);
		declareGenerateButton(HoriSym);
		declareGenerateButton(VertSym);
		declareGenerateButton(HoriVertSym);
		declareGenerateButton(Rot2Sym);
		declareGenerateButton(Rot4Sym);
		// ...

	void initExportRegion();
	void showExportBox();
	wxStaticBoxSizer* exportRegionSizer;
		wxTextCtrl* exportBox;
		wxFont exportFont;

	static constexpr int GAP_1 = 20;
	static constexpr int GAP_2 = 10;
	static constexpr int GAP_3 =  5;
};