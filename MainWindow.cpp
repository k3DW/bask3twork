#include "MainWindow.h"

MainWindow::MainWindow(int h, int w, wxString title) : wxFrame(nullptr, wxID_ANY, title) {
	this->h = h; this->w = w;
	selectNums[0] = 1; selectNums[1] = 1; selectNums[2] = h; selectNums[3] = w;
	textFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	
	this->CreateStatusBar();
	this->SetBackgroundColour(BACKGROUND_COLOUR);
	this->initSizerLayout();

	wxSize windowSize = this->GetBestSize();
	this->SetMinSize(windowSize);
	this->SetSize(windowSize);

	//wxString temp;
	//temp << MAX_SIZE.GetHeight() << "," << MAX_SIZE.GetWidth();
	//wxMessageBox(temp);
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

	this->changeSelectCoord(1, 1, h, w);
}
void MainWindow::initGenerateRegion() {
	generateRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Generate");
	initGenerateButton(NoSym, "No Symmetry");
	initGenerateButton(VertSym, "Vertical Reflection");
	initGenerateButton(HoriSym, "Horizontal Reflection");
	initGenerateButton(VertHoriSym, "Vertical + Horizontal");
	initGenerateButton(Rot2Sym, "2-way Rotational");
	initGenerateButton(Rot4Sym, "4-way Rotational");
	enableGenerateButtons(false);

	initButton(waveCollapse, "Wave Collapse NoSym");
	generateRegionSizer->Add(waveCollapseButton);
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
	this->changeSelectCoord(1, 1, h, w);
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
	int* n = selectNums;
	selectCoord->SetLabelText("(" + intWX(n[0]) + "," + intWX(n[1]) + ") to (" + intWX(n[2]) + "," << intWX(n[3]) + ")");
	selectRegionSizer->Layout();
	disp->clearHighlight();
	selectToggleButton->SetLabelText("Show");
	enableGenerateButtons(false);
}
void MainWindow::changeSelectCoord(int x1, int y1, int x2, int y2) {
	int* n = selectNums;
	if (x1) n[0] = x1;
	if (y1) n[1] = y1;
	if (x2) n[2] = x2;
	if (y2) n[3] = y2;
	updateSelectCoord();
}
void MainWindow::fixSelectCoord() { // This function "fixes" the selection coordinates, such that the left coordinate is to the upper-left of the right coordinate.
	if (selectNums[0] > selectNums[2]) std::swap(selectNums[0], selectNums[2]);
	if (selectNums[1] > selectNums[3]) std::swap(selectNums[1], selectNums[3]);
	updateSelectCoord();
}
void MainWindow::selectToggleFunction(wxCommandEvent& evt) {
	if (selectToggleButton->GetLabelText() == wxString("Show")) {
		fixSelectCoord();
		disp->highlightSelection(selectNums[0] - 1, selectNums[1] - 1, selectNums[2] - 1, selectNums[3] - 1);
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
	changeSelectCoord(1, 1, h, w);
	evt.Skip();
}

void MainWindow::enableGenerateButtons(bool enable) {
	if (enable) {
		bool hasVertSym = knot->checkVertSym(selectNums[0] - 1, selectNums[1] - 1, selectNums[2] - 1, selectNums[3] - 1);
		bool hasHoriSym = knot->checkHoriSym(selectNums[0] - 1, selectNums[1] - 1, selectNums[2] - 1, selectNums[3] - 1);
		generateNoSymButton->Enable();
		generateVertSymButton->Enable(hasVertSym);
		generateHoriSymButton->Enable(hasHoriSym);
		generateVertHoriSymButton->Enable(hasVertSym && hasHoriSym);
	}
	else {
		generateNoSymButton->Disable();
		generateVertSymButton->Disable();
		generateHoriSymButton->Disable();
		generateVertHoriSymButton->Disable();
	}
}
#define hasSymmetryType(SymType) (id == SymType && knot->generate##SymType##(selectNums[0] - 1, selectNums[1] - 1, selectNums[2] - 1, selectNums[3] - 1))
void MainWindow::generateKnot(wxCommandEvent& evt) {
	int id = evt.GetId();
	if (
		hasSymmetryType(NoSym)			||
		hasSymmetryType(VertSym)		||
		hasSymmetryType(HoriSym)		||
		hasSymmetryType(VertHoriSym)	||
		hasSymmetryType(Rot2Sym)
	){
		disp->drawKnot();
		this->showExportBox();
	}
	else
		wxMessageBox("The specified knot was not able to be generated in " + MAX_ATTEMPTS_STR + " attempts.", "Error: Knot failed");
	evt.Skip();
}

void MainWindow::waveCollapseFunction(wxCommandEvent& evt) {

	const wxString oldStatus = this->GetStatusBar()->GetStatusText();

	/*
	knot->waveCollapseNoSym(selectNums);
	/* /
	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			this->GetStatusBar()->SetStatusText("Generating no sym WFC... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (knot->waveCollapseNoSym(selectNums)) {
			success = true;
			wxMessageBox(intWX(attempts));
		}
	}//* /

	this->GetStatusBar()->SetStatusText(oldStatus);

	disp->drawKnot();
	this->showExportBox();
	//*/

	evt.Skip();
}

void MainWindow::regenExportBox() {
	if(exportBox)
		exportBox->Destroy();
	exportBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(9 * w + 14, 19 * h + 7), wxTE_MULTILINE | wxTE_NO_VSCROLL | wxTE_READONLY);
	exportBox->SetFont(exportFont);
	exportRegionSizer->Prepend(exportBox, 0, wxALIGN_CENTER | wxDOWN | wxUP, GAP_3);
}
/* need to fix */ void MainWindow::showExportBox() {

	//wxString test = glyphs[186];
	//wxMessageBox(test + " " + test + " " + test);

	//wxMessageBox(glyphs[1] + glyphs[1] + test + glyphs[1] + glyphs[1]);

	wxString toExport;
	bool display = false;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (knot->get(i, j) == glyphs[186])					// fix this later?
				toExport << wxString::FromUTF8("\xE2\x80\xA1");
			else
				toExport << knot->get(i, j);
		}
		if (i < h - 1)
			toExport << "\r\n";
	}
	exportBox->SetLabel(toExport);
	//if(display) wxMessageBox(toExport);
}