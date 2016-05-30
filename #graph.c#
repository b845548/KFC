#include<stdio.h>
#include<stdlib.h>

enum { WIDTH = 16 };
enum { HEIGHT = 12 };
enum { ELMTSIZE = 34 };


void test(int level1[HEIGHT][WIDTH]){
  int M_HEIGHT; 
  int M_WIDTH; 
  int ADV_HEIGHT;
  int ADV_WIDTH;
  int diff_HEIGHT;
  int diff_WIDTH;
  int i,j;
  int plus_court;
  int WIDTHCOUNT=0;
  int HEIGHTCOUNT=0;
  char * direction[10];
  M_HEIGHT= 4;
  M_WIDTH= 6;
  ADV_HEIGHT=7;
  ADV_WIDTH=3;
  printf(" Mario HEIGHT %d, WIDTH %d\n",M_HEIGHT,M_WIDTH);
  printf(" Adversaire HEIGHT %d, WIDTH %d\n",ADV_HEIGHT,ADV_WIDTH);  
  printf("Graph \n0 = point depart\n77 = point arrive\n99 = mur \n");
   
   for(i=0 ; i < HEIGHT; i++){
     for(j=0 ; j < WIDTH; j++){
       if(level1[i][j]==1)level1[i][j]=99;
       else level1[i][j]= abs(ADV_HEIGHT-i)+ abs(ADV_WIDTH-j);
     }
   } 
     for(i=0 ; i < HEIGHT; i++){
       for(j=0 ; j < WIDTH; j++){
	 if(i==M_HEIGHT && j==M_WIDTH)printf(" X ");
       else printf("%2d ",level1[i][j]);
     }
     printf("\n");
   }
     
     printf("Plus court avec graph %d\n",level1[M_HEIGHT][M_WIDTH]);

     diff_HEIGHT= ADV_HEIGHT - M_HEIGHT;
     diff_WIDTH= ADV_WIDTH - M_WIDTH;

     HEIGHTCOUNT=0;
       WIDTHCOUNT=0;
     // le cas avec une coresspondance
        while(diff_HEIGHT || diff_WIDTH){
	   if(diff_HEIGHT > 0){
	     HEIGHTCOUNT--;
	     diff_HEIGHT--;
	   }
	   if(diff_HEIGHT < 0){
	     HEIGHTCOUNT++;
	     diff_HEIGHT++;
	   }
	   if(diff_WIDTH > 0){
	     WIDTHCOUNT--;
	     diff_WIDTH--;
	   }
	   if(diff_WIDTH < 0){
	     WIDTHCOUNT++;
	     diff_WIDTH++;
	   }
	 }
      
      printf("HEIGHT %d WIDTH %d \n",HEIGHTCOUNT,WIDTHCOUNT);
      while(HEIGHTCOUNT){
	if(HEIGHTCOUNT<0){
	  HEIGHTCOUNT++;
	  printf("NORTH\n"); 
	}
	if(HEIGHTCOUNT>0){
	  HEIGHTCOUNT--;
	  printf("SOUTH\n");
	}
      }
      while(WIDTHCOUNT){
      	if(WIDTHCOUNT<0){
	  WIDTHCOUNT++;
	  printf("WEST\n");
	}
      	if(WIDTHCOUNT>0){
	  WIDTHCOUNT--;
	  printf("EAST\n");
	}
      }



      
}
int main(){

int level1[HEIGHT][WIDTH] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 0, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};//9 = MARIO , 5 = ADV

 test(level1);
}
