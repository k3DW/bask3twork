#pragma once
#include "wx/wx.h"
#include "Constants.h"
#include "DisplayGrid.h"
#include "Knot.h"

class MainWindow : public wxFrame {

public:
	MainWindow(int h, int w, wxString title);
	~MainWindow();

	void updateSelectCoord();
	void changeSelectCoord(int x1, int y1, int x2, int y2);
	void fixSelectCoord();

	void enableGenerateButtons(bool enable = true);

	void regenExportBox();

private:
	int h, w;
	int selectNums[4]; // The "selection numbers", which are the numbers displayed in selectCoord
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
		wxStaticText* selectCoord;		// The "selection coordinates", which denote the top-left and bottom-right corners of the selection area
		wxBoxSizer* selectButtonSizer;	// The selection buttons
			declareButton(selectToggle);	// This button is a show/hide button, which shows and hides the selection
			declareButton(selectReset);	// This button resets the selection coordinates

	void initGenerateRegion();
	void generateKnot(wxCommandEvent& evt);
	wxStaticBoxSizer* generateRegionSizer;
		declareGenerateButton(NoSym);
		declareGenerateButton(VertSym);
		declareGenerateButton(HoriSym);
		declareGenerateButton(VertHoriSym);
		declareGenerateButton(Rot2Sym);
		declareGenerateButton(Rot4Sym);
		// ...

	void initExportRegion();
	void showExportBox();
	wxStaticBoxSizer* exportRegionSizer;
		wxTextCtrl* exportBox;
		wxFont exportFont;
};
