
#include "GamePCH.h"
#include "Layouts.h"

using TT = TileMap::TileType;

//const IVector2 Level_1_Dimensions(10, 10);
//const TileMap::TileType Level_1_TileLayout[] = {
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
//};

const IVector2 Level_1_Dimensions(22, 10);
const TileMap::TileType Level_1_TileLayout[] = {
TT::Wall, TT::Wall  , TT::Wall , TT::Wall , TT::Door , TT::Door , TT::Wall , TT::Empty, TT::Empty, TT::Empty, TT::Empty, TT::Empty, TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Door , TT::Door ,  TT::Wall , TT::Wall ,TT::Wall ,  TT::Wall ,
TT::Wall, TT::Floor , TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Empty, TT::Empty, TT::Wall , TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,  TT::Floor, TT::Floor,TT::Floor,  TT::Wall ,
TT::Wall, TT::Floor , TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Wall , TT::Wall , TT::Empty, TT::Wall , TT::Floor, TT::Box  , TT::Floor, TT::Floor, TT::Floor,  TT::Floor, TT::Box  ,TT::Floor,  TT::Wall ,
TT::Wall, TT::Box   , TT::Floor, TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Floor, TT::Floor, TT::Wall , TT::Empty, TT::Wall , TT::Floor, TT::Floor, TT::Box  , TT::Floor, TT::Floor,  TT::Box  , TT::Floor,TT::Floor,  TT::Wall ,
TT::Wall, TT::Floor , TT::Floor, TT::Wall , TT::Empty, TT::Empty, TT::Empty, TT::Wall , TT::Wall , TT::Floor, TT::Wall , TT::Wall , TT::Wall , TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,  TT::Floor, TT::Floor,TT::Floor,  TT::Wall ,
TT::Wall, TT::Floor , TT::Floor, TT::Wall , TT::Wall , TT::Empty, TT::Empty, TT::Empty, TT::Wall , TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,  TT::Floor, TT::Floor,TT::Floor,  TT::Wall ,
TT::Wall, TT::Floor , TT::Floor, TT::Floor, TT::Wall , TT::Wall , TT::Door , TT::Wall , TT::Wall , TT::Floor, TT::Wall , TT::Wall , TT::Wall , TT::Floor, TT::Floor, TT::Box  , TT::Floor, TT::Floor,  TT::Box  , TT::Floor,TT::Floor,  TT::Wall ,
TT::Wall, TT::Column, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Wall , TT::Empty, TT::Wall , TT::Floor, TT::Box  , TT::Floor, TT::Floor, TT::Floor,  TT::Floor, TT::Box  ,TT::Floor,  TT::Wall ,
TT::Wall, TT::Column, TT::Floor, TT::Floor, TT::Floor, TT::Box  , TT::Floor, TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Empty, TT::Wall , TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,  TT::Floor, TT::Floor,TT::Floor,  TT::Wall ,
TT::Wall, TT::Wall  , TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Empty, TT::Empty, TT::Empty, TT::Empty, TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Wall , TT::Wall ,  TT::Wall , TT::Wall ,TT::Wall ,  TT::Wall ,
};


const IVector2 Level_2_Dimensions(6, 3);
const TileMap::TileType Level_2_TileLayout[] = {
TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
TT::Floor, TT::Wall,  TT::Wall,  TT::Floor, TT::Wall,  TT::Floor,
TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor, TT::Floor,
};

const Layout k_Layout_1(Level_1_TileLayout, Level_1_Dimensions);
const Layout k_Layout_2(Level_2_TileLayout, Level_2_Dimensions);