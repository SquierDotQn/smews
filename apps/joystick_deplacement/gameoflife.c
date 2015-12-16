#define LONGUEUR 8
#define LARGEUR 8



static int grid[LONGUEUR][LARGEUR];

void init(void) {
	int i,j;
	for(i = 0 ; i < LONGUEUR ; i++) {
		for(j = 0 ; j < LARGEUR ; j++) {
			grid[LONGUEUR][LARGEUR] = 0;	
		}
	}
}

void nextGeneration(void) {
	int i,j;
	int nbCellules = 0;
	for(i = 0 ; i < LONGUEUR -2 ; i++) {
		for(j = 0 ; j < LARGEUR -2 ; j++) {
			for(x = -1 ; x < 2 ; x++) {
				for( y = -1 ; y < 2 ; y++) {
					if(grid[i +x][j + y] == 1) {
						nbCellules++;
					}
				}
			}
			if(nbCellules == 3 && grid[i][j] == 0){
				grid[i][j] = 1;
			} else if (nbCellules < 2 || nbCellules > 3) {
				grid[i][j] = 0;
			}
		}
	}

}

void createCell(int x , int y) {
	grid[y][x] = 1;
}

void destroyCell(int x , int y) {
	grid[y][x] = 0;
}
