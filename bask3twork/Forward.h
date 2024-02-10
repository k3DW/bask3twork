#pragma once
#include <cstdint>
#include <vector>



// Main
class App;
class MainWindow;
struct Version;



// wx
class wxBoxSizer;
class wxGridBagSizer;
class wxStaticText;
class wxStatusBar;
class wxString;
class wxTextCtrl;



// Controls
class ExportDialog;
class MenuBar;
class RegenDialog;
class RegenDialogTextBox;



// Grid
class DisplayGrid;
class Knot;

enum class TileState;
class Tile;
using Tiles = std::vector<std::vector<Tile>>;



// Pure
enum class Connection : uint8_t;
struct ConnectionTransformations;
using ConnectionFn = Connection(*)(Connection);
struct Connections;

template <class T>
class Grid;
template <class T, bool is_const>
class GridIterator;
template <class T, bool is_const>
class GridRange;

enum class GlyphFlag;
struct Glyph;
using Glyphs = Grid<const Glyph*>;
struct GlyphsTransformed;
using CodePoint = int32_t;

struct GridSize;

struct Point;
struct Selection;

class SelectionIterator;
class SelectionZipIterator;
class SelectionZipRange;

enum class Symmetry;
class SymmetryChecker;



// Regions
class GenerateRegion;
class GenerateRegionButton;
class SelectRegion;
