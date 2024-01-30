#pragma once
#include <cstdint>
#include <vector>



// Main
class App;
class MainWindow;
class MainSizer;
struct Version;



// wx
class wxBoxSizer;
class wxGridBagSizer;
class wxStaticText;
class wxStatusBar;
class wxString;
class wxTextCtrl;



// Controls
class MenuBar;
class RegenDialog;
class RegenDialogTextBox;



// Grid
class DisplayGrid;
class GridSizer;
class Knot;

class Tile;
using Tiles = std::vector<std::vector<Tile*>>;
class AxisLabel;



// Pure
enum class Connection : uint8_t;
struct ConnectionTransformations;
using ConnectionFn = Connection(*)(Connection);
struct Connections;

enum class GlyphFlag;
struct Glyph;
using Glyphs = std::vector<std::vector<const Glyph*>>;
struct GlyphsTransformed;

struct GridSize;

struct Point;
struct Selection;

enum class Symmetry;
class SymmetryChecker;



// Regions
class ExportRegion;
class GenerateRegion;
class GenerateRegionButton;
class SelectRegion;
