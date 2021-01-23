#include "MainWindow.h"

MainWindow::MainWindow(int h, int w, wxString title) : wxFrame(nullptr, wxID_ANY, title), h(h), w(w), iMin(0), jMin(0), iMax(h - 1), jMax(w - 1) {
	textFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	
	/*
	wxMessageBox(
		intWX(AllGlyphs[0].connectToMirrorRight) +
		intWX(AllGlyphs[0].connectToMirrorLeft) +
		intWX(AllGlyphs[0].connectToMirrorDown) +
		intWX(AllGlyphs[0].connectToMirrorUp) +
		intWX(AllGlyphs[0].sameAfterMirrorY) +
		intWX(AllGlyphs[0].sameAfterMirrorX) +
		intWX(AllGlyphs[0].sameAfterRotate) + " " +
		intWX(AllGlyphs[0].right) + " " +
		intWX(AllGlyphs[0].left) + " " +
		intWX(AllGlyphs[0].down) + " " +
		intWX(AllGlyphs[0].up)

		+ "\r\n" + intWX(AllGlyphs[0].flags)
	);
	wxMessageBox(
		intWX(sizeof(unsigned int))
		+ "\r\n" +
		intWX(sizeof(test))
	);//*/

	this->CreateStatusBar();
	this->SetBackgroundColour(BACKGROUND_COLOUR);
	this->initSizerLayout();

	wxSize windowSize = this->GetBestSize();
	this->SetMinSize(windowSize);
	this->SetSize(windowSize);
}
MainWindow::~MainWindow() {
	this->Hide();
}
void MainWindow::initSizerLayout() {
	this->initDispSizer();

	// These are in a weird order to avoid dereferencing `nullptr`s
	this->initGenerateRegion();
	this->initGridRegion();
	this->initSelectRegion();
	this->initExportRegion();

	buttonSizer = new wxBoxSizer(wxVERTICAL);
	buttonSizer->AddStretchSpacer();
	buttonSizer->Add(gridRegionSizer, 0, wxDOWN, GAP_2);
	buttonSizer->Add(selectRegionSizer, 0, wxDOWN, GAP_2);
	buttonSizer->Add(generateRegionSizer, 0, wxDOWN, GAP_2);
	buttonSizer->Add(exportRegionSizer);
	buttonSizer->AddStretchSpacer();

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(dispSizer, 0, wxEXPAND | wxALL, GAP_1);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(buttonSizer, 0, wxEXPAND | (wxALL ^ wxLEFT), GAP_1);
	this->SetSizer(mainSizer);
}
void MainWindow::initDispSizer() {
	if (!knot) { 
		dispSizer = new wxBoxSizer(wxVERTICAL);
		dispSizer->AddStretchSpacer();
		dispSizer->AddStretchSpacer();
	}
	else {
		delete knot;
		disp->Destroy();
	}
	knot = new Knot(h, w, this->GetStatusBar());
	disp = new DisplayGrid(this, knot);
	dispSizer->Insert(1, disp, 0, wxEXPAND);
}
void MainWindow::initGridRegion() {
	gridHeight = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%i"), h), wxDefaultPosition, wxSize(42, 24), wxTE_CENTER);
	gridHeight->SetMaxLength(2);
	gridHeight->SetFont(textFont);
	
	gridWidth = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%i"), w), wxDefaultPosition, wxSize(42, 24), wxTE_CENTER);
	gridWidth->SetMaxLength(2);
	gridWidth->SetFont(textFont);
	
	gridInputSizer = new wxBoxSizer(wxHORIZONTAL);
	gridInputSizer->Add(gridHeight, 0, wxEXPAND);
	gridInputSizer->Add(new wxStaticText(this, wxID_ANY, " by "), 0, wxALIGN_CENTER);
	gridInputSizer->Add(gridWidth, 0, wxEXPAND);
	
	initButton(gridRegen, "Regenerate Grid");
	gridRegenButton->SetToolTip("TEST");

	gridRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Grid");
	gridRegionSizer->Add(gridInputSizer, 0, wxEXPAND | wxDOWN, GAP_3);
	gridRegionSizer->Add(gridRegenButton, 0, wxEXPAND);
}
void MainWindow::initSelectRegion() {
	selectToggleButton = new wxButton(this, wxID_ANY, "Show", wxDefaultPosition, wxSize(65,23));
	selectToggleButton->Bind(wxEVT_BUTTON, &MainWindow::selectToggleFunction, this);
	selectResetButton = new wxButton(this, wxID_ANY, "Reset", wxDefaultPosition, wxSize(65,23));
	selectResetButton->Bind(wxEVT_BUTTON, &MainWindow::selectResetFunction, this);

	selectToggleButton->SetSize(wxSize(200, 200));

	selectCoord = new wxStaticText(this, wxID_ANY, "");
	selectCoord->SetFont(textFont);

	selectButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	selectButtonSizer->Add(selectToggleButton);
	selectButtonSizer->Add(selectResetButton);

	selectRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Select");
	selectRegionSizer->Add(selectCoord, 0, wxALIGN_CENTER | wxDOWN, GAP_3);
	selectRegionSizer->Add(selectButtonSizer, 0, wxEXPAND);

	this->resetSelectCoord();
}
void MainWindow::initGenerateRegion() {
	generateRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Generate");
	initGenerateButton(NoSym, "No Symmetry");
	initGenerateButton(HoriSym, "Horizontal Reflection");
	initGenerateButton(VertSym, "Vertical Reflection");
	initGenerateButton(HoriVertSym, "Vertical + Horizontal");
	initGenerateButton(Rot2Sym, "2-way Rotational");
	initGenerateButton(Rot4Sym, "4-way Rotational");
	enableGenerateButtons(false);
}
void MainWindow::initExportRegion() {
	exportRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Export");
	exportFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	this->regenExportBox();
}

void MainWindow::gridRegenFunction(wxCommandEvent& evt) {
	// Checking if the values in the boxes are numbers
	wxString heightString = gridHeight->GetValue();
	wxString widthString = gridWidth->GetValue();
	if (!(heightString.IsNumber() && widthString.IsNumber())) {
		wxMessageBox("Please enter only numbers for the new grid size.", "Error: Non-numerical grid size");
		return;
	}
	// Checking if the numbers in the boxes are positive integers
	int heightNum = wxAtoi(heightString);
	int widthNum = wxAtoi(widthString);
	if (!(heightNum > 0 && widthNum > 0)) {
		wxMessageBox("Please enter positive whole numbers for the new grid size.", "Error: Non-integer grid size");
		return;
	}

	// Updating the height and width, reinitializing the DisplayGrid
	this->h = heightNum;
	this->w = widthNum;
	this->initDispSizer();
	this->SetMinSize(wxDefaultSize);

	// Reseting selectCoord and exportBox
	this->resetSelectCoord();
	this->regenExportBox();
	Layout();

	// Changing the minimum size of the window to fit the new DisplayGrid
	// GetBestSize() will not return a proper value unless the MinSize is lowered (or reset)
	wxSize newSize = this->GetBestSize();
	this->SetMinSize(newSize);
	if (!this->IsMaximized()) this->SetSize(newSize);

	evt.Skip();
}

void MainWindow::updateSelectCoord() {
	selectCoord->SetLabelText("(" + intWX(iMin + 1) + "," + intWX(jMin + 1) + ") to (" + intWX(iMax + 1) + "," << intWX(jMax + 1) + ")");
	selectRegionSizer->Layout();
	disp->clearHighlight();
	selectToggleButton->SetLabelText("Show");
	enableGenerateButtons(false);
}
void MainWindow::changeSelectCoord(const int iMin_, const int jMin_, const int iMax_, const int jMax_) {
	if (iMin_ > -1) iMin = iMin_;
	if (jMin_ > -1) jMin = jMin_;
	if (iMax_ > -1) iMax = iMax_;
	if (jMax_ > -1) jMax = jMax_;

	updateSelectCoord();
}
void MainWindow::fixSelectCoord() { // This function "fixes" the selection coordinates, such that the left coordinate is to the upper-left of the right coordinate.
	if (iMin > iMax) std::swap(iMin, iMax);
	if (jMin > jMax) std::swap(jMin, jMax);
	updateSelectCoord();
}
void MainWindow::resetSelectCoord() {
	changeSelectCoord(0, 0, h - 1, w - 1);
}
void MainWindow::selectToggleFunction(wxCommandEvent& evt) {
	if (selectToggleButton->GetLabelText() == wxString("Show")) {
		fixSelectCoord();
		disp->highlightSelection(iMin, jMin, iMax, jMax);
		selectToggleButton->SetLabelText("Hide");
		enableGenerateButtons(true);
	}
	else {
		disp->clearHighlight();
		selectToggleButton->SetLabelText("Show");
		enableGenerateButtons(false);
	}
	evt.Skip();
}
void MainWindow::selectResetFunction(wxCommandEvent& evt) {
	resetSelectCoord();
	evt.Skip();
}

void MainWindow::enableGenerateButtons(bool enable) {
	if (enable) {
		//bool hasVertSym = knot->checkVertSym(iMin, jMin, iMax, jMax);
		//bool hasHoriSym = knot->checkHoriSym(iMin, jMin, iMax, jMax);
		generateNoSymButton->Enable();
		generateHoriSymButton->Enable();// hasHoriSym);
		generateVertSymButton->Enable();// hasVertSym);
		generateHoriVertSymButton->Enable();// hasVertSym&& hasHoriSym);
	}
	else {
		generateNoSymButton->Disable();
		generateHoriSymButton->Disable();
		generateVertSymButton->Disable();
		generateHoriVertSymButton->Disable();
	}
}
void MainWindow::generateKnot(wxCommandEvent& evt) {
	int id = evt.GetId();
	const wxString oldStatus = GetStatusBar()->GetStatusText();

	/*if (
		hasSymmetryType(NoSym)			||
		hasSymmetryType(VertSym)		||
		hasSymmetryType(HoriSym)		||
		hasSymmetryType(HoriVertSym)	||
		hasSymmetryType(Rot2Sym)
	)*/
	if(
		(id == NoSym		&& knot->generateNoSym(iMin, jMin, iMax, jMax))			||
		(id == HoriSym		&& knot->generateHoriSym(iMin, jMin, iMax, jMax))		||
		(id == VertSym		&& knot->generateVertSym(iMin, jMin, iMax, jMax))		||
		(id == HoriVertSym	&& knot->generateHoriVertSym(iMin, jMin, iMax, jMax))
	) {
		disp->drawKnot();
		this->showExportBox();
	}
	else
		wxMessageBox("The specified knot was not able to be generated in " + MAX_ATTEMPTS_STR + " attempts.", "Error: Knot failed");

	GetStatusBar()->SetStatusText(oldStatus);
	evt.Skip();
}

void MainWindow::regenExportBox() {
	if(exportBox)
		exportBox->Destroy();
	exportBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(9 * w + 14, 19 * h + 7), wxTE_MULTILINE | wxTE_NO_VSCROLL | wxTE_READONLY);
	exportBox->SetFont(exportFont);
	exportRegionSizer->Prepend(exportBox, 0, wxALIGN_CENTER | wxDOWN | wxUP, GAP_3);
}
void MainWindow::showExportBox() {
	wxString toExport;
	bool display = false;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
			toExport << knot->get(i, j);
		if (i < h - 1)
			toExport << "\r\n";
	}
	exportBox->SetLabel(toExport);
}