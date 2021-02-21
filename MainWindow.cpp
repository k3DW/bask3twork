#include "MainWindow.h"

MainWindow::MainWindow(int h, int w, wxString title) : wxFrame(nullptr, wxID_ANY, title), h(h), w(w), iMin(0), jMin(0), iMax(h - 1), jMax(w - 1) {
	CreateStatusBar();
	SetBackgroundColour(BACKGROUND_COLOUR);
	initMenuBar();

	initSizerLayout();

	wxSize windowSize = GetBestSize();
	SetMinSize(windowSize);
	SetSize(windowSize);
}
MainWindow::~MainWindow() {
	Hide();
}
void MainWindow::initMenuBar() {
	menuFile = new wxMenu();
	menuFile->Append(wxID_OPEN, "&Open\tCtrl-O", "Open a knot file.");
	menuFile->Append(wxID_SAVE, "&Save\tCtrl-S", "Save a knot file.");
	menuFile->Bind(wxEVT_MENU, &MainWindow::fileEventHandler, this);

	//menuGenerate = new wxMenu();
	//menuGenerateNoSym = new wxMenuItem(nullptr, 100, "test");
	//menuGenerate->Bind()
	//menuGenerate->Append(100, "&No Symmetry");
	//menuGenerate->Append(101, "&Horizontal Symmetry");
	//menuGenerate->Bind(wxEVT_MENU, &MainWindow::generateKnot, this);
	//Connect(100, wxEVT_MENU, wxCommandEventHandler(MainWindow::generateKnot));

	menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	//menuBar->Append(menuGenerate, "&Generate");
	SetMenuBar(menuBar);
}
void MainWindow::initSizerLayout() {
	initDispSizer();

	// These are in a weird order to avoid dereferencing `nullptr`s
	initGenerateRegion();
	initGridRegion();
	initSelectRegion();
	initExportRegion();

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
	SetSizer(mainSizer);
}
void MainWindow::initDispSizer() {
	/// \b Method

	/// First, check to see if the Knot has been initialized already.
	/// If it has not, then neither has the DisplayGrid or its sizer, so the sizer is initialized with 2 stretch spacers.
	/// It the Knot has been initialized, then \c delete the Knot and \c Destroy() the DisplayGrid.
	if (!knot) { 
		dispSizer = new wxBoxSizer(wxVERTICAL);
		dispSizer->AddStretchSpacer();
		dispSizer->AddStretchSpacer();
	}
	else {
		delete knot;
		disp->Destroy();
	}

	/// Regardless of the above, initialize the Knot with the member variables \c h and \c w and the status bar.
	/// Then initialize the DisplayGrid with the newly generated Knot, and insert it between the stretch spacers in its sizer.
	knot = new Knot(h, w, GetStatusBar());
	disp = new DisplayGrid(this, knot);
	dispSizer->Insert(1, disp, 0, wxEXPAND);
}
void MainWindow::initGridRegion() {
	gridHeight = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%i"), h), wxDefaultPosition, wxSize(42, 24), wxTE_CENTER);
	gridHeight->SetMaxLength(2);
	gridHeight->SetFont(TEXT_FONT);
	
	gridWidth = new wxTextCtrl(this, wxID_ANY, wxString::Format(wxT("%i"), w), wxDefaultPosition, wxSize(42, 24), wxTE_CENTER);
	gridWidth->SetMaxLength(2);
	gridWidth->SetFont(TEXT_FONT);
	
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
	selectCoord->SetFont(TEXT_FONT);

	selectButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	selectButtonSizer->Add(selectToggleButton);
	selectButtonSizer->Add(selectResetButton);

	selectRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Select");
	selectRegionSizer->Add(selectCoord, 0, wxALIGN_CENTER | wxDOWN, GAP_3);
	selectRegionSizer->Add(selectButtonSizer, 0, wxEXPAND);

	resetSelectCoord();
}
void MainWindow::initGenerateRegion() {
	generateRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Generate");
	#define XX(Sym, desc) \
		generate##Sym##Button = new wxButton(this, static_cast<unsigned int>(Symmetry::Sym), desc); \
		generate##Sym##Button->Bind(wxEVT_BUTTON, &MainWindow::generateKnot, this); \
		generateRegionSizer->Add(generate##Sym##Button);
	SYMMETRIES
	#undef XX
	enableGenerateButtons(false);
}
void MainWindow::initExportRegion() {
	exportRegionSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Export");
	exportFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	regenExportBox();
}

void MainWindow::fileEventHandler(wxCommandEvent& evt) {
	if (evt.GetId() == wxID_OPEN)
		openFile(evt);
	else if (evt.GetId() == wxID_SAVE)
		saveFile(evt);
	evt.Skip();
}
void MainWindow::openFile(wxCommandEvent& evt) {
	// Open a wxFileDialog to get the name of the file.
	wxFileDialog openFileDialog(this, "Open Knot file", "", "", "k3DW Knot Files (*.k3knot)|*.k3knot|Text files (*.txt*)|*.txt*", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	// If the wxFileDialog gets closed, stop the function.
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	// Uses a wxTextFile to read from this file; could also use wxFileInputStream.
	wxTextFile file(openFileDialog.GetPath());
	file.Open();

	// Check the row count, must be 1 <= rows <= MAX_H
	if (file.GetLineCount() > MAX_H) {
		wxMessageBox("Please choose a smaller file.", "Error: File size is too large");
		return;
	}
	if (file.GetLineCount() == 0) {
		wxMessageBox("Please choose a non-empty file.", "Error: File is empty");
		return;
	}

	size_t newH = file.GetLineCount();
	size_t newW = 0;

	GlyphVec2 glyphs;
	glyphs.reserve(newH);

	for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
		if (newW > 0 && line.size() != newW) {
			wxMessageBox("Please choose a valid knot file. The number of characters in every row must be equal.", "Error: File has jagged rows");
			return;
		}
		if (newW == 0)
			newW = line.size();

		GlyphVec1 glyphRow;
		glyphRow.reserve(newW);
		for (wxUniChar c : line) {
			// TODO: fix the Glyph pointer pushed to `glyphRow`
			glyphRow.push_back(&AllGlyphs[0]);
		}
		glyphs.push_back(glyphRow);
	}

	// TODO:
	//		call `gridRegenFunction`, but with a new Knot from `GlyphVec2 glyphs` above
	//		or maybe rewrite the function
	
	file.Close();
}
void MainWindow::saveFile(wxCommandEvent& evt) {
	// Open a wxFileDialog to get the name of the file.
	wxFileDialog saveFileDialog(this, "Save Knot file", "", "", "k3DW Knot Files (*.k3knot)|*.k3knot|Text files (*.txt*)|*.txt*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

	// If the wxFileDialog gets closed, stop the function.
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	// Uses a wxTextFile to write to this file; could also use wxFileOutputStream
	wxTextFile file(saveFileDialog.GetPath());

	// Clear the file if it exists, or create a file if it doesn't exist
	if (file.Exists()) {
		file.Open();
		file.Clear();
	}
	else file.Create();

	// Write the knot to the file, line by line
	for (int i = 0; i < knot->h; i++) {
		wxString line = "";
		for (int j = 0; j < knot->w; j++)
			line << knot->get(i, j);
		file.AddLine(line);
	}

	// Save changes, then close
	file.Write();
	file.Close();
}

void MainWindow::gridRegenFunction(wxCommandEvent& evt) {
	/// \b Method
	
	/// First, check if the values in the boxes are numbers. If not, send a \c mxMessageBox with an error message and return.
	/// Then check if the numbers in the boxes are positive integers. If not, send a different error message and return.
	wxString heightString = gridHeight->GetValue();
	wxString widthString = gridWidth->GetValue();
	if (!(heightString.IsNumber() && widthString.IsNumber())) {
		wxMessageBox("Please enter only numbers for the new grid size.", "Error: Non-numerical grid size");
		return;
	}
	int heightNum = wxAtoi(heightString);
	int widthNum = wxAtoi(widthString);
	if (!(heightNum > 0 && widthNum > 0)) {
		wxMessageBox("Please enter positive whole numbers for the new grid size.", "Error: Non-integer grid size");
		return;
	}
	if (heightNum > MAX_H || widthNum > MAX_W) {
		wxMessageBox("Please enter sizes that are, at most, " + intWX(MAX_H) + " by " + intWX(MAX_W) + ".", "Error: Knot size too large");
		return;
	}

	/// Next, update \c h and \c w member variables, then reinitialize the DisplayGrid using MainWindow::initDispSizer().
	h = heightNum;
	w = widthNum;
	initDispSizer();

	/// Then, reset the select coordinates with MainWindow::resetSelectCoord()
	/// and regenerate and export textbox with MainWindow::regenExportBox().
	resetSelectCoord();
	regenExportBox();
	Layout(); // To make sure everything is displayed properly.

	/// Lastly, change the size and minimum size of the window to fit the new DisplayGrid.
	/// To do this, the minimum size must first be set to \c wxDefaultSize, before finding the new size with \c GetBestSize().
	/// The minimum size is set to this new size, but the current size is only changed if the window is not maixmized.
	SetMinSize(wxDefaultSize);
	wxSize newSize = GetBestSize();
	SetMinSize(newSize);
	if (!IsMaximized())
		SetSize(newSize);

	evt.Skip();
}

void MainWindow::updateSelectCoord() {
	/// This function uses the stored values of \c iMin, \c jMin, \c iMax, and \c jMax to update the displayed coordinates.
	/// Each of these stored values is zero - indexed, so the displayed value is incremented by 1.
	selectCoord->SetLabelText("(" + intWX(iMin + 1) + "," + intWX(jMin + 1) + ") to (" + intWX(iMax + 1) + "," << intWX(jMax + 1) + ")");
	selectRegionSizer->Layout();
	/// This function also
	/// (1) calls DisplayGrid::clearHighlight() to remove all possible highlighting in the grid,
	/// (2) sets the \c show/hide button to "Show", and
	/// (3) calls MainWindow::enableGenerateButtons() with parameter \c false to disable all of the generating buttons
	disp->clearHighlight();
	selectToggleButton->SetLabelText("Show");
	enableGenerateButtons(false);
}
void MainWindow::changeSelectCoord(ijSignature) {
	if (iMin > -1) this->iMin = iMin;
	if (jMin > -1) this->jMin = jMin;
	if (iMax > -1) this->iMax = iMax;
	if (jMax > -1) this->jMax = jMax;

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
	/// \b Method

	/// If the text in the \c show/hide button is "Show",
	/// then first fix the selection coordinates with MainWindow::fixSelectCoords(),
	/// call DisplayGrid::highlightSelection() with the index member variables,
	/// set the button text to be "Hide",
	/// and call MainWindow::enableGenerateButtons() to enable the buttons.
	if (selectToggleButton->GetLabelText() == wxString("Show")) {
		fixSelectCoord();
		disp->highlightSelection(iMin, jMin, iMax, jMax);
		selectToggleButton->SetLabelText("Hide");
		enableGenerateButtons(true);
	}
	/// If the text in the \c show/hide button is otherwise (which can only be "Hide"),
	/// then first call DisplayGrid::clearHighlight(),
	/// set the button text to be "Show",
	/// and call MainWindow::enableGenerateButtons() with paramater \c false to disable all buttons.
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
	/// This function allows the Knot \c generate functions to have no conditional operation, and just assume that the parameters are valid.
	/// Instead of checking for symmetry conditions within the generating function itself, the user is disallowed from pressing the button.

	/// \param enable Tells the function whether to enable (conditionally) or disable (fully) the generating buttons, has a default value of \c true

	/// \b Method

	/// To conditionally enable the buttons, first call the \c Knot::check____Sym() functions on the current selection and store the outputs.
	/// Then enable each of the buttons is the proper symmetry condition has been met.
	if (enable) {
		bool hasHoriSym = knot->checkHoriSym(iMin, jMin, iMax, jMax);
		bool hasVertSym = knot->checkVertSym(iMin, jMin, iMax, jMax);
		bool hasRot2Sym = knot->checkRot2Sym(iMin, jMin, iMax, jMax);
		bool hasRot4Sym = knot->checkRot4Sym(iMin, jMin, iMax, jMax);
		bool hasFwdDiag = knot->checkFwdDiag(iMin, jMin, iMax, jMax);
		bool hasBackDiag = knot->checkBackDiag(iMin, jMin, iMax, jMax);
		generateNoSymButton->Enable();
		generateHoriSymButton->Enable(hasHoriSym);
		generateVertSymButton->Enable(hasVertSym);
		generateHoriVertSymButton->Enable(hasHoriSym && hasVertSym);
		generateRot2SymButton->Enable(hasRot2Sym);
		generateRot4SymButton->Enable(hasRot4Sym);
		generateFwdDiagButton->Enable(hasFwdDiag);
		generateBackDiagButton->Enable(hasBackDiag);
		generateFullSymButton->Enable(hasHoriSym && hasVertSym && hasRot4Sym);
	}
	/// If \c enable is \c false, then disable each of the buttons.
	else {
		#define XX(Sym, desc) generate##Sym##Button->Disable();
		SYMMETRIES
		#undef XX
	}
}
void MainWindow::generateKnot(wxCommandEvent& evt) {
	/// Each of the \c Knot::generate____Sym() functions uses the status bar, so first store the current displayed message.
	const wxString oldStatus = GetStatusBar()->GetStatusText();

	/// Each of the generating buttons has its symmetry type as its ID value. Get this symmetry from the event call.
	/// Then, check the symmetry against each of the posible symmetries.
	/// Once there is a match, call the appropriate \c Knot::generate____Sym() function.
	/// If this function returns \c true, then the Knot has been generated successfully,
	/// so update the DisplayGrid with DisplayGrid::drawKnot() and update the export text box with MainWindow::showExportBox.
	/// If the generate function returns \c false, then display an error message as a \c wxMessageBox.
	Symmetry id = static_cast<Symmetry>(evt.GetId());
	#define XX(Sym, desc) (id == Symmetry::Sym && knot->generate##Sym##(iMin, jMin, iMax, jMax)) ||
	if(	SYMMETRIES false ){
	#undef XX
		disp->drawKnot();
		showExportBox();
	}
	else
		wxMessageBox("The specified knot was not able to be generated in " + intWX(MAX_ATTEMPTS) + " attempts.", "Error: Knot failed");

	/// At the end, set the status bar back to the message which was displayed at the beginning of the function.
	GetStatusBar()->SetStatusText(oldStatus);
	evt.Skip();
}

void MainWindow::showExportBox() {
	wxString toExport;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
			toExport << knot->get(i, j);
		if (i < h - 1)
			toExport << "\r\n";
	}
	exportBox->SetLabel(toExport);
}
void MainWindow::regenExportBox() {
	if(exportBox)
		exportBox->Destroy();
	exportBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(9 * w + 14, 19 * h + 7), wxTE_MULTILINE | wxTE_NO_VSCROLL | wxTE_READONLY);
	exportBox->SetFont(exportFont);
	exportRegionSizer->Prepend(exportBox, 0, wxALIGN_CENTER | wxDOWN | wxUP, GAP_3);
}