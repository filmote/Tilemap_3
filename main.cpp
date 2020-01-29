// Copyright (C) 2019 Hannu Viitala
// 
// The source code in this file is released under the MIT license.
// Go to http://opensource.org/licenses/MIT for the full license details.
// 
// Converted to C++ and extended by Filmote

// *** A TILEMAP DEMO FOR THE POKITTO ***

#include "Pokitto.h"
#include "Tilemap.hpp"
#include "Data.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

namespace Constants {
    
    const uint8_t tileWidth = 16;
    const uint8_t tileHeight = 16;
    
    const uint8_t mapTileWidth = 16;                            // Map width in tiles ..
    const uint8_t mapTileHeight = 16;                           // Map height in tiles ..
    
    const uint16_t mapWidth = mapTileWidth * tileWidth;         // 16 tiles of 16 pixels
    const uint16_t mapHeight = mapTileHeight * tileHeight;      // 16 tiles of 16 pixels
    
    const uint16_t screenCentreX = PD::width / 2; 
    const uint16_t screenCentreY = PD::height / 2;

}



// ---------------------------------------------------------------------------------------


enum TileType {
    
	Water = 0,
	Green = 1,
	Tree = 2,
	Grass = 3,

}; 


enum Direction {
    
	Up,
	Down,
	Left,
	Right
	
}; 


struct Entity {
    
    int16_t x;
    int16_t y;
    int16_t xOffset;
    int16_t yOffset;

    const uint8_t width = 12;
    const uint8_t height = 15;
    
    int16_t getPositionInMapX() { return Constants::screenCentreX - x - xOffset; }
    int16_t getPositionInMapY() { return Constants::screenCentreY - y - yOffset; }

};


// ---------------------------------------------------------------------------------------


Tilemap tilemap;
Entity player;


// ---------------------------------------------------------------------------------------
//
//  Check to see if the move the entity is about to make is into a green (empty) tile.  As 
//  the player is 12 x 15 pixels and the tiles are bigger (16 x 16) its possible that the 
//  player could be straddling two tiles in either direction ..
//
bool checkMovement(Entity &entity, int16_t x, int16_t y, Direction direction) {

    int8_t tileId1 = 0;
    int8_t tileId2 = 0;

    switch (direction) {
        
        case Direction::Left:
            tileId1 = tilemap.GetTileId(x, y, 16);
            tileId2 = tilemap.GetTileId(x, y + entity.height, 16);
            break;
        
        case Direction::Right:
            tileId1 = tilemap.GetTileId(x + entity.width, y, 16);
            tileId2 = tilemap.GetTileId(x + entity.width, y + entity.height, 16);
            break;

        case Direction::Up:
            tileId1 = tilemap.GetTileId(x, y, 16);
            tileId2 = tilemap.GetTileId(x + entity.width, y, 16);
            break;
        
        case Direction::Down:
            tileId1 = tilemap.GetTileId(x, y + player.height, 16);
            tileId2 = tilemap.GetTileId(x + entity.width, y + entity.height, 16);
            break;
            
    }
    


    // If either tile is not green, do not move.
    
    if (tileId1 != TileType::Green || tileId2 != TileType::Green) {
        return false;
        
    }

    return true;
        
}



// ---------------------------------------------------------------------------------------
//
//  Handle the player movements ..
//
void handlePlayerMovements() {

    int16_t positionInMapX = player.getPositionInMapX();
    int16_t positionInMapY = player.getPositionInMapY();

    if (PC::buttons.pressed(BTN_LEFT) || PC::buttons.repeat(BTN_LEFT, 1))    { 


        // Can we move to the left?
        
        if (checkMovement(player, positionInMapX - 1, positionInMapY, Direction::Left)) {

    
            // If we are already on the right hand side of the screen, come back to the centre ..

            if (player.xOffset < 0) {     
                
                player.xOffset++;
    
            }
            
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (player.x < 0) {
                
                player.x++;

            }
            
            
            // If we cannot scroll the screen, move the player to the left ..
            
            else if (player.x <= (PD::width / 2)) {     
                
                player.xOffset++;

            }
            
        }

    }
    
    if (PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, 1))   { 


        // Can we move to the right?
        
        if (checkMovement(player, positionInMapX + 1, positionInMapY, Direction::Right)) {
                
    
            // If we are already on the left hand side of the screen, come back to the centre ..
            
            if (player.xOffset > 0) {
                player.xOffset--;
            }
                 
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (player.x > (PD::width - Constants::mapWidth) && player.x <= 0) {
                player.x--;
            }
            
            
            // If we cannot scroll the screen, move the player to the right ..
            
            else if (player.x >= (PD::width - Constants::mapWidth)) {
                player.xOffset--;
            }
            
        }

    }
    
    
    if (PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, 1))      { 


        // Can we move up?
        
        if (checkMovement(player, positionInMapX, positionInMapY - 1, Direction::Up)) {

            
            // If we are already in the bottom section of the screen, come back to the centre ..
            
            if (player.yOffset < 0) {                                  
                player.yOffset++;
            }
            
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (player.y < 0) {
                player.y++;
            }
            
            
            // If we cannot scroll the screen, move the player up ..
            
            else if (player.y <= (PD::height / 2)) {     
                player.yOffset++;
            }
        
        }
        
    }
    
    if (PC::buttons.pressed(BTN_DOWN) || PC::buttons.repeat(BTN_DOWN, 1))    { 


        // Can we move down?
        
        if (checkMovement(player, positionInMapX, positionInMapY + 1, Direction::Down)) {

    
            // If we are already in the top section of the screen, come back to the centre ..
            
            if (player.yOffset > 0) {
                player.yOffset--;
            }
                 
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (player.y > (PD::height - Constants::mapHeight) && player.y <= 0) {
                player.y--;
            }
            
            
            // If we cannot scroll the screen, move the player down ..
            
            else if (player.y >= (PD::height - Constants::mapHeight)) {
                player.yOffset--;
            }
            
        }

    }
    
}


// ---------------------------------------------------------------------------------------


int main(){

    PC::begin();
    PD::persistence = true;
    PD::invisiblecolor = 0;
    PD::loadRGBPalette(palettePico);   
    PD::setFont(fontC64);



    // Initialise the map ..
    
    tilemap.set(16, 16, Data::mapPixels);
    tilemap.tiles[TileType::Green] = Data::green16;
    tilemap.tiles[TileType::Tree] = Data::tree16;
    tilemap.tiles[TileType::Grass] = Data::grass16;
    tilemap.tiles[TileType::Water] = Data::water16;


    // Position the player into a vacant spot on the map ..
    
    player.x = -20;
    player.y = -50;
    
    
    while (PC::isRunning()) {
        
        if (!PC::update()) continue;

        PC::buttons.pollButtons();        



        // Handle player movements ..

        handlePlayerMovements();


        // Render screen ..

        tilemap.draw(player.x, player.y);
        
        
        // Render player ..
        
        PD::drawBitmapData(Constants::screenCentreX - player.xOffset, Constants::screenCentreY - player.yOffset, player.width, player.height, Data::girl12x15Pixels);

    }
    
    return 0;
    
}

