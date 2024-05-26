#include<Wire.h> 
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h> 

#define larg 128 
#define haut 64 
#define adresse 0x3D 
#define pin_reset 2 

Adafruit_SSD1306 aff(larg,haut,&Wire,pin_reset) ;

int pin_bd=3 ;
int pin_bg=4 ;
int pin_bh=5 ;
int pin_bb=6 ;
int pin_ok =7 ;

bool etat_bd=false ;
bool etat_bg=false ;
bool etat_bh=false ;
bool etat_bb=false ;
bool etat_ok=false ;

const int centre_x=larg/2 ;
const int centre_y=haut/2 ;


char symbole_uti ;
char symbole_robot ;

int ligne_uti;
int col_uti ;


int pos_x[3]={30,60,90} ;
int pos_y[3]={2,22,42} ;

char mat[3][3]={{' ',' ',' '} ,
                {' ',' ',' '} , 
                {' ',' ',' '} } ;

int x=pos_x[0] ;
int y=pos_y[0] ;

int ligne_robot ;
int col_robot ; 

int ligne_first_symbole_uti ;
int col_first_symbole_uti ;

bool end_game=true ;

int liste_cor[4]={} ;


void setup() {
  pinMode(pin_bd,INPUT_PULLUP) ;
  pinMode(pin_bg,INPUT_PULLUP) ;
  pinMode(pin_bh,INPUT_PULLUP) ;
  pinMode(pin_bb,INPUT_PULLUP) ;
  pinMode(pin_ok,INPUT_PULLUP) ;
  
  aff.begin(SSD1306_SWITCHCAPVCC,adresse) ;
  aff.display() ;
  delay(1000) ;
  aff.clearDisplay() ;
  symbole_uti='X' ;
  symbole_robot='O' ;
  affichage_choix_droite() ;
}
void loop() {
  if(end_game==true) { 
    for (int i=0;i<3;i++) { 
      for (int j=0;j<3;j++) { 
        mat[i][j]=' ' ;
      }
    }
    initialisation() ;
    etat_ok=digitalRead(pin_ok) ; 
    delay(100) ;
    if (etat_ok==true) { 
      end_game=false ;
    }
  }
  else if  (end_game==false) {
    uti_jouer() ;
    affichage_plateau() ;
    if (gagne(symbole_uti)==true) { 
      delay(500) ;
      aff.drawLine(liste_cor[0],liste_cor[1],liste_cor[2],liste_cor[3],WHITE) ;
      aff.display() ;
      delay(1000) ;
      affichage_fin_de_jeu(symbole_uti) ;
      delay(1000) ;
      affichage_choix_droite() ;
      end_game=true  ;
      return ;
    }
    else if(mat_complet()==true) {
      affichage_match_null() ;
      delay(1000);
      affichage_choix_droite() ;
      end_game=true ;
      return ;
    }
    robot_jouer() ;
    delay(400) ;
    affichage_plateau() ;
    if (gagne(symbole_robot)==true) { 
      delay(500) ;
      aff.drawLine(liste_cor[0],liste_cor[1],liste_cor[2],liste_cor[3],WHITE) ;
      aff.display() ;
      delay(1000) ;
      affichage_fin_de_jeu(symbole_robot) ;
      delay(1000) ;
      affichage_choix_droite() ;
      end_game=true  ;
      return ;
    }
    else if(mat_complet()==true) {
      affichage_match_null() ;
      delay(1000) ;
      affichage_choix_droite() ;
      end_game=true ;
      return ;
    }
  }
}
bool mat_complet(){ 
  int s=0 ;
  for (int i=0;i<3;i++) { 
    for (int j=0;j<3;j++) { 
      if (mat[i][j]=='X' or mat[i][j]=='O') {
        s++ ;
      }
    }
  }
  if (s==9) { 
    return(true) ;
  }
  else { 
    return(false) ;
  }
}
bool gagne(char f) {
  char liste_gagne[3]={f,f,f} ;
  char ligne1[3]={} ;
  char ligne2[3]={};
  char ligne3[3]={} ;
  char col1[3]={} ;
  char col2[3]={} ;
  char col3[3]={} ;
  char d1[3]={} ;
  char d2[3]={} ;
  for (int i=0;i<3;i++) { 
    ligne1[i]=mat[0][i] ;
    ligne2[i]=mat[1][i] ;
    ligne3[i]=mat[2][i] ;
    col1[i]=mat[i][0] ;
    col2[i]=mat[i][1] ;
    col3[i]=mat[i][2] ;
    d1[i]=mat[i][i] ;
    d2[i]=mat[i][2-i] ;
  }
  if (egale(ligne1,liste_gagne)==true) {
    liste_cor[0]=20 ;
    liste_cor[1]=10 ;
    liste_cor[2]=110 ;
    liste_cor[3]=10 ;
    return(true) ;
  }
  
  else if (egale(ligne2,liste_gagne)==true) {
    liste_cor[0]=20 ;
    liste_cor[1]=30 ;
    liste_cor[2]=110 ;
    liste_cor[3]=30 ;
    return(true) ;
  }
  else if (egale(ligne3,liste_gagne)==true) {
    liste_cor[0]=20 ;
    liste_cor[1]=50 ;
    liste_cor[2]=110 ;
    liste_cor[3]=50 ;
    return(true) ;
  }
  else if (egale(col1,liste_gagne)==true) {
    liste_cor[0]=35 ;
    liste_cor[1]=0 ;
    liste_cor[2]=35 ;
    liste_cor[3]=60 ;
    return(true) ;
  }
  else if (egale(col2,liste_gagne)==true) {
    liste_cor[0]=65 ;
    liste_cor[1]=0 ;
    liste_cor[2]=65 ;
    liste_cor[3]=60 ;
    return(true) ;
  }
  else if (egale(col3,liste_gagne)==true) {
     liste_cor[0]=95 ;
    liste_cor[1]=0 ;
    liste_cor[2]=95 ;
    liste_cor[3]=60 ;
    return(true) ;
  }
  else if (egale(d1,liste_gagne)==true) {
     liste_cor[0]= 20;
    liste_cor[1]=0 ;
    liste_cor[2]=110 ;
    liste_cor[3]=60;
    return(true) ;
  }
  else if (egale(d2,liste_gagne)==true) {
    liste_cor[0]=20 ;
    liste_cor[1]=60 ;
    liste_cor[2]=110 ;
    liste_cor[3]=0;
    return(true) ;
  }
  else {
    return(false) ;
  }
}





void robot_jouer() { 
  if(one_symbole_in_grille()==true) {
    ligne_robot=random(0,3) ;
    col_robot=random(0,3) ;
    while(ligne_robot==ligne_first_symbole_uti and col_robot==col_first_symbole_uti) { 
      ligne_robot=random(0,3) ;
      col_robot=random(0,3) ;
    }
    mat[ligne_robot][col_robot]=symbole_robot ;
  }
    else if (chance_gagne(symbole_uti)==true) { 
      mat[ligne_robot][col_robot]=symbole_robot ;
  
  }
  else if (chance_gagne(symbole_robot)==true) {
    mat[ligne_robot][col_robot]=symbole_robot ;
  }

  else {
    ligne_robot=random(0,3) ;
    col_robot=random(0,3) ; 
    while (mat[ligne_robot][col_robot]!=' ') {
      ligne_robot=random(0,3) ;
      col_robot=random(0,3) ;
    }
    mat[ligne_robot][col_robot]=symbole_robot ;
  }
}

bool chance_gagne(char c) { 
  char per1[3]={c,' ',c} ;
  char per2[3]={c,c,' '} ;
  char per3[3]={' ',c,c} ;

  char ligne1[3]={} ;
  char ligne2[3]={} ;
  char ligne3[3]={} ;
  char col1[3]={} ;
  char col2[3]={} ;
  char col3[3]={} ;
  char d1[3]={} ;
  char d2[3]={} ;
  for (int i=0;i<3;i++) { 
    ligne1[i]=mat[0][i] ;
    ligne2[i]=mat[1][i] ;
    ligne3[i]=mat[2][i] ;
    col1[i]=mat[i][0] ;
    col2[i]=mat[i][1] ;
    col3[i]=mat[i][2] ;
    d1[i]=mat[i][i] ;
    d2[i]=mat[i][2-i] ;
  }
  if (egale(ligne1,per1) or egale(ligne1,per2) or egale(ligne1,per3)) { 
    for (int k=0;k<3;k++) { 
      if (ligne1[k]==' ') { 
        ligne_robot=0 ;
        col_robot=k ;
      }
    }
    return(true) ;
  }
  if (egale(ligne2,per1) or egale(ligne2,per2) or egale(ligne2,per3)) { 
    for (int k=0;k<3;k++) { 
      if (ligne2[k]==' ') { 
        ligne_robot=1 ;
        col_robot=k ;
      }
    }
    return(true) ;
  }
  if (egale(ligne3,per1) or egale(ligne3,per2) or egale(ligne3,per3)) { 
    for (int k=0;k<3;k++) { 
      if (ligne3[k]==' ') { 
        ligne_robot=2 ;
        col_robot=k ;
      }
    }
    return(true) ;
  }
  if (egale(col1,per1) or egale(col1,per2) or egale(col1,per3)) { 
    for (int k=0;k<3;k++) { 
      if (col1[k]==' ') { 
        ligne_robot=k ;
        col_robot=0 ;
      }
    }
    return(true) ;
  }
  if (egale(col2,per1) or egale(col2,per2) or egale(col2,per3)) { 
    for (int k=0;k<3;k++) { 
      if (col2[k]==' ') { 
        ligne_robot=k ;
        col_robot=1;
      }
    }
    return(true) ;
  }
  if (egale(col3,per1) or egale(col3,per2) or egale(col3,per3)) { 
    for (int k=0;k<3;k++) { 
      if (col3[k]==' ') { 
        ligne_robot=k;
        col_robot=2;
      }
    }
    return(true) ;
  }
  if (egale(d1,per1) or egale(d1,per2) or egale(d1,per3)) { 
    for (int k=0;k<3;k++) { 
      if (d1[k]==' ') { 
        ligne_robot=k ;
        col_robot=k ;
      }
    }
    return(true) ;
  }
  if (egale(d2,per1) or egale(d2,per2) or egale(d2,per3)) { 
    for (int k=0;k<3;k++) { 
      if (d2[k]==' ') { 
        ligne_robot=k ;
        col_robot=2-k ;
      }
    }
    return(true) ;
  }
}

bool egale (char l1[3],char l2[3]) { 
  bool test=true ;
  for (int h=0;h<3;h++) { 
    if (l1[h]==l2[h]) {
      test = test and true ;
    }
    else {
      test=test and false ;
    }
  }
  return(test) ;
}




bool one_symbole_in_grille() { 
  int s=0 ;
  for (int ligne=0;ligne<3;ligne++) { 
    for (int col=0;col<3;col++) { 
      if (mat[ligne][col]==symbole_uti) { 
        s++ ;
        ligne_first_symbole_uti =ligne ;
        col_first_symbole_uti=col ;
      }
    }
  }
  if (s==1) { 
    return(true) ;
  }
  else {
    return(false) ;
  }
}
void uti_jouer() { 
  etat_ok=digitalRead(etat_ok) ;
  delay(100) ;
  while (etat_ok==false or not(mat[col_uti][ligne_uti]==' ')) {
    etat_bd=digitalRead(pin_bd) ;
    etat_bg=digitalRead(pin_bg) ;
    etat_bh=digitalRead(pin_bh) ;
    etat_bb=digitalRead(pin_bb) ;
    delay(150) ; 
    if (etat_bd==true ) { 
      delay(50) ;
      ligne_uti++ ;
      if (ligne_uti==3) { 
        ligne_uti=0 ;
      }
    }
    else if (etat_bg==true ) { 
      delay(50) ;
      ligne_uti--;
      if (ligne_uti==-1) { 
        ligne_uti=2;
      }
    }
     else if (etat_bh==true) { 
      delay(50) ;
      col_uti--;
      if (col_uti==-1) { 
        col_uti=2 ;
      }
    }
     else if (etat_bb==true) { 
      delay(50) ;
      col_uti++ ;
      if (col_uti==3) { 
        col_uti=0;
      }
     }
     x=pos_x[ligne_uti]  ;
     y=pos_y[col_uti] ;
     etat_ok=digitalRead(pin_ok)  ;
     affichage_plateau() ;
    }
    mat[col_uti][ligne_uti]=symbole_uti ;
    affichage_plateau() ;
}
void initialisation() {
  etat_bd=digitalRead(pin_bd) ;
  etat_bg=digitalRead(pin_bg) ;
  delay(100) ;
  if (etat_bd==true ) { 
    affichage_choix_droite() ;
    symbole_uti='X' ;
    symbole_robot='O' ;
  }
  else if (etat_bg==true) {
    affichage_choix_gauche() ;
    symbole_uti='O' ;
    symbole_robot='X' ;
  }
}

void affichage_plateau() { 
  aff.clearDisplay() ;
  aff.drawLine(20,0,110,0,WHITE) ;
  aff.drawLine(20,0,20,60,WHITE) ;
  aff.drawLine(110,0,110,60,WHITE) ;
  aff.drawLine(20,60,110,60,WHITE ) ; 
  aff.drawLine(20,20,110,20,WHITE) ;
  aff.drawLine(20,40,110,40,WHITE) ;

  aff.drawLine(50,0,50,60,WHITE) ;
  aff.drawLine(80,0,80,60,WHITE) ;

  aff.drawRect(x-5,y,19,16,WHITE) ;

  for (int i=0;i<3;i++) { 
    for(int j=0;j<3;j++) {
      aff.setTextSize(2) ;
      aff.setTextColor(WHITE) ;
      aff.setCursor(pos_x[j],pos_y[i]) ;
      aff.print(mat[i][j]) ;
    }
  }
  aff.display() ;
}

void affichage_choix_droite() { 
  aff.clearDisplay() ;
  aff.setTextSize(1) ;
  aff.setTextColor(WHITE) ;
  aff.setCursor(10,10) ;
  aff.print("choisir X ou O : ") ;

  aff.drawLine(centre_x,20,centre_x,haut,WHITE) ;

  aff.drawRect(74,25,44,20,WHITE) ;
  
  aff.setTextSize(1) ;
  aff.setTextColor(WHITE) ;
  aff.setCursor(centre_x+30,centre_y) ;
  aff.print("X") ;
  aff.setCursor(centre_x-30,centre_y) ;
  aff.print("O") ;
  aff.display() ;
}
void affichage_choix_gauche() { 
  aff.clearDisplay() ;
  aff.setTextSize(1) ;
  aff.setTextColor(WHITE) ;
  aff.setCursor(10,10) ;
  aff.print("choisir X ou O : ") ;

  aff.drawLine(centre_x,20,centre_x,haut,WHITE) ;

  aff.drawRect(10,25,44,20,WHITE) ;
  
  aff.setTextSize(1) ;
  aff.setTextColor(WHITE) ;
  aff.setCursor(centre_x+30,centre_y) ;
  aff.print("X") ;
  aff.setCursor(centre_x-30,centre_y) ;
  aff.print("O") ;
  aff.display() ;
}
void affichage_fin_de_jeu(char x) {
  aff.clearDisplay()  ;

  aff.drawRect(20,20,100,30,WHITE) ;
  
  aff.setTextSize(1) ;
  aff.setTextColor(WHITE) ;
  aff.setCursor(30,30) ;
  aff.print(x) ;
  aff.print(" est gagne !!") ;
  aff.display() ;
}
void affichage_match_null() { 
  aff.clearDisplay() ;

  aff.drawRect(15,25,90,15,WHITE) ;
  aff.setTextSize(1) ;
  aff.setTextColor(WHITE) ;
  aff.setCursor(20,30) ;
  aff.print("Match null") ;

  aff.display() ;
}
