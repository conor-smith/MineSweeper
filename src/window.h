#ifndef WINDOW_H
#define WINDOW_H

/* This only generates the data for the current game
 * Do not call before setting a new game*/
void getNewWindowSize(int *width, int *height);

// This must be called after the window has been set up with it's new size
void setNewPositionData();

#endif